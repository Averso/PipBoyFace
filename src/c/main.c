#include <pebble.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Persistent storage key
#define SETTINGS_KEY 1


static Window *main_window;                                    
static TextLayer *layer_time, *layer_weekday,*layer_date, *layer_text_time, *layer_text_date, *layer_text_battery;     
static GFont font_time, font_weekday,font_date,font_others, font_battery; 
static Layer *layer_bars[6];
static BitmapLayer *layer_background, *layer_battery_vaultboy, *layer_bt, *layer_charge;                          
static GBitmap *bitmap_background[4], *bitmap_battery_vaultboy_white[3],*bitmap_battery_vaultboy_green[3]
                                    ,*bitmap_battery_vaultboy_blue[3],*bitmap_battery_vaultboy_amber[3]
                                    ,*bitmap_bt[4],*bitmap_charge[4];
static GBitmap **current_battery_vaultboy_bitmaps; //pointer to array with cetrain color of vaultboy
GColor text_color;

static int battery_level;

//positions and dimensions of layers 
static int layer_date_x = 7;
static int layer_date_y = 134;
static int layer_date_w = 87;
static int layer_date_h = 26;
static int layer_weekday_x = 106;
static int layer_weekday_y = 137;
static int layer_weekday_w = 32;
static int layer_weekday_h = 25;
static int layer_time_x = 7;
static int layer_time_y = 5;
static int layer_time_w = 100;
static int layer_time_h = 37;

static int layer_icon_x=118;
static int layer_icon_w=17;
static int layer_icon_h=18;

static int layer_charge_y=28;
static int layer_bt_y=53;

static int layer_bar_w=11;
static int layer_bar_h=3;

static int layer_vaultboy_x=33;
static int layer_vaultboy_y=47;
static int layer_vaultboy_w=78;
static int layer_vaultboy_h=88;

//enums for settings
typedef enum {WHITE,GREEN,BLUE,AMBER} ScreenColor;


void init(void);
void deinit(void);
static void window_load(Window *window);
static void window_unload(Window *window);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void update_time();  //handle fetching time/date data
static void update_weekday();
static void update_date();
static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment);
static void inbox_received_handler(DictionaryIterator *iter, void *context);
static void load_default_settings();
static void save_settings();
static void load_settings();
static void change_background(ScreenColor color);
static void change_texts_color(GColor text_color);
static void window_update();
static void battery_callback(BatteryChargeState state);
static void bluetooth_callback(bool connected);
static void battery_update_vaultboy();
static void battery_bar_update_proc(Layer *layer, GContext *ctx);

//settings struct
typedef struct ClaySettings {
  ScreenColor screen_color;
  bool vibe_on_disconnect;
  int battery_medium_level;
  int battery_low_level;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;


int main(void) {
  init();
  app_event_loop();
  deinit();
}

void init(void) {
  
  //try to load settings
  load_settings();
  
  //listen for appmessages
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
  
  main_window = window_create();
  
  window_set_window_handlers(main_window,(WindowHandlers) {
        .load = window_load,
        .unload = window_unload
 });

  window_stack_push(main_window, true);
  
  //register for battery level updates
  battery_state_service_subscribe(battery_callback);
  
  connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bluetooth_callback
  });
  
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  update_time();
  update_weekday();
  update_date();
  battery_callback(battery_state_service_peek());
  
    
}

void deinit(void) {
  //destroy window
  if (main_window) {
    window_destroy(main_window);
 }
}

static void window_load(Window *window)  
{
  //get window layer and bounds
  Layer *window_layer = window_get_root_layer(window);  
  GRect bounds = layer_get_bounds(window_layer);
  
  //FONTS
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_TIME_36));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_DAY_18));
  font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_DATE_19));
  font_others = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_OTHRS_14));
  font_battery = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_BTTRY_12));
  //font color
  text_color=GColorWhite;
  
  
  
  //BACKGROUND 
  
  bitmap_background[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_WHITE);
  bitmap_background[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_GREEN);
  bitmap_background[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_BLUE);
  bitmap_background[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_AMBER);
  
  layer_background = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(layer_background,bitmap_background[0]);
  
  //VAULTBOY
  current_battery_vaultboy_bitmaps = bitmap_battery_vaultboy_white;
  
  bitmap_battery_vaultboy_white[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE_WHITE);
  bitmap_battery_vaultboy_white[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM_WHITE);
  bitmap_battery_vaultboy_white[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD_WHITE);
  bitmap_battery_vaultboy_green[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE_GREEN);
  bitmap_battery_vaultboy_green[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM_GREEN);
  bitmap_battery_vaultboy_green[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD_GREEN);
  bitmap_battery_vaultboy_blue[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE_BLUE);
  bitmap_battery_vaultboy_blue[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM_BLUE);
  bitmap_battery_vaultboy_blue[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD_BLUE);
  bitmap_battery_vaultboy_amber[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE_AMBER);
  bitmap_battery_vaultboy_amber[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM_AMBER);
  bitmap_battery_vaultboy_amber[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD_AMBER);
  
  layer_battery_vaultboy = bitmap_layer_create(
    GRect(layer_vaultboy_x, layer_vaultboy_y, layer_vaultboy_w, layer_vaultboy_h));
  bitmap_layer_set_bitmap(layer_battery_vaultboy,*current_battery_vaultboy_bitmaps);
  
  
  //BLUETOOTH ICON
  bitmap_bt[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_WHITE);
  bitmap_bt[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_GREEN);
  bitmap_bt[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_BLUE);
  bitmap_bt[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_AMBER);
  
  layer_bt = bitmap_layer_create(GRect(layer_icon_x, layer_bt_y, layer_icon_w, layer_icon_h));
  bitmap_layer_set_bitmap(layer_bt, bitmap_bt[0]);
  
  //CHARGE ICON
  bitmap_charge[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_WHITE);
  bitmap_charge[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_GREEN);
  bitmap_charge[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_BLUE);
  bitmap_charge[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_AMBER);
  
  layer_charge = bitmap_layer_create(GRect(layer_icon_x, layer_charge_y, layer_icon_w, layer_icon_h));
  bitmap_layer_set_bitmap(layer_charge, bitmap_charge[0]);
  
  
  //HEALTH-BATTERY BARS
  layer_bars[0] = layer_create(GRect(66, 49, layer_bar_w, layer_bar_h));
  layer_bars[1] = layer_create(GRect(66, 87, layer_bar_w, layer_bar_h));
  layer_bars[2] = layer_create(GRect(36, 69, layer_bar_w, layer_bar_h));
  layer_bars[3] = layer_create(GRect(36, 115, layer_bar_w, layer_bar_h));
  layer_bars[4] = layer_create(GRect(97, 69, layer_bar_w, layer_bar_h));
  layer_bars[5] = layer_create(GRect(97, 115, layer_bar_w, layer_bar_h));
  
  //assign to layer update proc
  for(int i=0;i<6;i++)
    layer_set_update_proc(layer_bars[i], battery_bar_update_proc);
  
  //TEXT    
  //weekday 
  layer_time = text_layer_create(
    GRect(layer_time_x, layer_time_y, layer_time_w, layer_time_h));  
  layer_weekday = text_layer_create(
    GRect(layer_weekday_x, layer_weekday_y, layer_weekday_w, layer_weekday_h));   
  layer_date = text_layer_create(
    GRect(layer_date_x, layer_date_y,layer_date_w, layer_date_h));
  layer_text_time = text_layer_create(GRect(23,-4, 39, 20));  
  layer_text_date = text_layer_create(GRect(23, 154,39, 20));
  layer_text_battery = text_layer_create(GRect(112, 7, 26, 20));
  
  

  set_up_text_layer(layer_time, GColorClear, text_color, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday, GColorClear, text_color, "Mon", font_weekday,GTextAlignmentLeft);
  set_up_text_layer(layer_date, GColorClear, text_color, "11.11.16", font_date,GTextAlignmentCenter);
  set_up_text_layer(layer_text_time, GColorClear, text_color, "TIME", font_others,GTextAlignmentCenter);
  set_up_text_layer(layer_text_date, GColorClear, text_color, "DATE", font_others,GTextAlignmentCenter);
  set_up_text_layer(layer_text_battery, GColorClear, text_color, "100%", font_battery,GTextAlignmentCenter);
  
      
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_background));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_battery_vaultboy));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_bt));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_charge));
  layer_add_child(window_layer,text_layer_get_layer(layer_text_time));
  layer_add_child(window_layer,text_layer_get_layer(layer_text_date));
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday));
  layer_add_child(window_layer,text_layer_get_layer(layer_time));  
  layer_add_child(window_layer,text_layer_get_layer(layer_date));
  layer_add_child(window_layer,text_layer_get_layer(layer_text_battery));
  
  for(int i=0;i<6;i++)
   layer_add_child(window_layer,layer_bars[i]);
  
  
  //turn on transparency
  bitmap_layer_set_compositing_mode(layer_battery_vaultboy, GCompOpSet); 
  bitmap_layer_set_compositing_mode(layer_bt, GCompOpSet);   
  bitmap_layer_set_compositing_mode(layer_charge, GCompOpSet); 
  
  //hide bt&charge icon
  layer_set_hidden(bitmap_layer_get_layer(layer_bt), true);
  layer_set_hidden(bitmap_layer_get_layer(layer_charge), true);
  
  window_update();
  
}

static void window_unload(Window *window)
{
  //destroy text layers
  text_layer_destroy(layer_time);
  text_layer_destroy(layer_weekday);
  text_layer_destroy(layer_date);  
  text_layer_destroy(layer_text_time);
  text_layer_destroy(layer_text_date);
  text_layer_destroy(layer_text_battery);
  
  
  //unload fonts
  fonts_unload_custom_font(font_time);
  fonts_unload_custom_font(font_weekday);
  fonts_unload_custom_font(font_date);
  fonts_unload_custom_font(font_others); 
  fonts_unload_custom_font(font_battery);
 
  //destroy bitmaps
  
  for(int i=0; i<4;i++)
  {
    gbitmap_destroy(bitmap_background[i]);      
    gbitmap_destroy(bitmap_bt[i]);  
    gbitmap_destroy(bitmap_charge[i]);  
  }
  
  //destroy vaultboys bitmaps
  for(int i=0; i<3;i++)
  {  
    gbitmap_destroy(bitmap_battery_vaultboy_white[i]);      
    gbitmap_destroy(bitmap_battery_vaultboy_green[i]);      
    gbitmap_destroy(bitmap_battery_vaultboy_blue[i]);    
  }
  
  
  bitmap_layer_destroy(layer_background);
  bitmap_layer_destroy(layer_battery_vaultboy);
  bitmap_layer_destroy(layer_bt);
  
  for(int i=0; i<6;i++)
  {  
    layer_destroy(layer_bars[i]);
  }
  
}

static void window_update()
{
  
  //vibration
   if(settings.vibe_on_disconnect)
     bluetooth_callback(connection_service_peek_pebble_app_connection());
  
  
  
  //pip boy color
//  gbitmap_destroy(bitmap_background);
  switch(settings.screen_color)
  {
    case WHITE:
      text_color = GColorWhite;   
      bitmap_layer_set_bitmap(layer_background,bitmap_background[0]); //background
      current_battery_vaultboy_bitmaps=bitmap_battery_vaultboy_white;    //vaoultboy
      bitmap_layer_set_bitmap(layer_bt, bitmap_bt[0]);
      bitmap_layer_set_bitmap(layer_charge, bitmap_charge[0]);
      break;
    case GREEN:
      text_color = GColorMalachite;    
      bitmap_layer_set_bitmap(layer_background,bitmap_background[1]);
      current_battery_vaultboy_bitmaps=bitmap_battery_vaultboy_green;
      bitmap_layer_set_bitmap(layer_bt, bitmap_bt[1]);    
      bitmap_layer_set_bitmap(layer_charge, bitmap_charge[1]);
      break;
    case BLUE:
      text_color = GColorVividCerulean;    
      bitmap_layer_set_bitmap(layer_background,bitmap_background[2]);
      current_battery_vaultboy_bitmaps=bitmap_battery_vaultboy_blue; 
      bitmap_layer_set_bitmap(layer_bt, bitmap_bt[2]);
      bitmap_layer_set_bitmap(layer_charge, bitmap_charge[2]);
      break;
    case AMBER:
      text_color = GColorRajah;
      bitmap_layer_set_bitmap(layer_background,bitmap_background[3]);
      current_battery_vaultboy_bitmaps=bitmap_battery_vaultboy_amber;      
      bitmap_layer_set_bitmap(layer_bt, bitmap_bt[3]);
      bitmap_layer_set_bitmap(layer_charge, bitmap_charge[3]);
      break;        
  }      
  //refresh vault boy
  battery_update_vaultboy();
  //layer_mark_dirty(bitmap_layer_get_layer(layer_battery_vaultboy));
  change_texts_color(text_color);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
   if (units_changed & MINUTE_UNIT)
  {
    update_time();
  }
  if(units_changed & DAY_UNIT)
  {
    update_weekday();    
    update_date();
  }
  
}

static void update_time()
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get time 
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);  
  
  //display time on text layer
  text_layer_set_text(layer_time, s_buffer);
}

static void update_weekday() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get weekday
  static char s_buffer[4];
  strftime(s_buffer, sizeof(s_buffer),"%a", tick_time);
  
  //change text on weekday text layers
  text_layer_set_text(layer_weekday, s_buffer);
  
  
}

static void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[10];
  strftime(s_buffer, sizeof(s_buffer), "%d.%m.%g", tick_time);
  
  text_layer_set_text(layer_date, s_buffer);
}

//SETTINGS
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  //assign the values from settings to struct
  Tuple *vibe_enabled = dict_find(iter, MESSAGE_KEY_vibe_disconnect);
  if(vibe_enabled) {
   settings.vibe_on_disconnect = vibe_enabled->value->int32 == 1;
  }  
  
  //time position
  Tuple *screen_color = dict_find(iter, MESSAGE_KEY_screen_color);
  if(screen_color) {   
    
    if(strcmp(screen_color->value->cstring, "white") == 0)    
       settings.screen_color=WHITE;    
    else if (strcmp(screen_color->value->cstring, "green") == 0)   
       settings.screen_color=GREEN;   
    else if (strcmp(screen_color->value->cstring, "blue") == 0)   
       settings.screen_color=BLUE;   
    else if (strcmp(screen_color->value->cstring, "amber") == 0)   
       settings.screen_color=AMBER;   
  }
  
  
 //battery levels
  
  int low_lvl=0, med_lvl=0;
  Tuple *bat_med_lvl = dict_find(iter, MESSAGE_KEY_battery_medium_lvl);  
  if(bat_med_lvl) {
    med_lvl = atoi(bat_med_lvl->value->cstring);    
  }    
  Tuple *bat_low_lvl = dict_find(iter, MESSAGE_KEY_battery_low_lvl); 
  if(bat_low_lvl) {
    low_lvl = atoi(bat_low_lvl->value->cstring);    
  }  
  
  //check if low value isnt higher than med
  if(low_lvl>=med_lvl)
  {
    settings.battery_low_level = 20;
    settings.battery_medium_level = 50;
  }
  else
  {
    settings.battery_low_level = low_lvl;
    settings.battery_medium_level = med_lvl;
  }
 
  
  //save loaded settings
  save_settings();
}

static void save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  //refresh window
 window_update();
}

static void load_settings() {
  // Load the default settings
  load_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void load_default_settings() {
  settings.screen_color = WHITE; 
  settings.vibe_on_disconnect = false;
  settings.battery_medium_level=50;
  settings.battery_low_level=20;
}

//OTHERS
static void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

static void change_texts_color(GColor text_color)
{  
  text_layer_set_text_color(layer_time, text_color);
  text_layer_set_text_color(layer_weekday, text_color);
  text_layer_set_text_color(layer_date, text_color);
  text_layer_set_text_color(layer_text_time, text_color);
  text_layer_set_text_color(layer_text_date, text_color);
  text_layer_set_text_color(layer_text_battery, text_color);
 
}


//BLUETOOTH
static void bluetooth_callback(bool connected) {
  
   layer_set_hidden(bitmap_layer_get_layer(layer_bt), connected);
  
   if(!connected && settings.vibe_on_disconnect) {
    // Issue a vibrating alert        
    vibes_double_pulse();
  }
}


//BATTERY
static void battery_callback(BatteryChargeState state)
{
  
  //mark helath-battery bars dirty
  for(int i=0;i<6;i++)
    layer_mark_dirty(layer_bars[i]);
  
  //show/hide charging icon
  layer_set_hidden(bitmap_layer_get_layer(layer_charge), !(state.is_charging));
  // change text in text layer
  static char s_buffer[5];
  snprintf(s_buffer, sizeof(s_buffer), "%d%%", state.charge_percent);
  
  text_layer_set_text(layer_text_battery, s_buffer);  
  
  
  battery_level = state.charge_percent;
  //change vault boy
  battery_update_vaultboy();  
  //TODO: is charging
  
 
}

static void battery_update_vaultboy()
{
   APP_LOG(APP_LOG_LEVEL_DEBUG, "battery level %d",battery_level);
  
  if(battery_level<=100 && battery_level>settings.battery_medium_level)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "image fine");
    bitmap_layer_set_bitmap(layer_battery_vaultboy, *current_battery_vaultboy_bitmaps);
  }
  else if (battery_level<=settings.battery_medium_level && battery_level>settings.battery_low_level)
  {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "image medium");
    bitmap_layer_set_bitmap(layer_battery_vaultboy, *(current_battery_vaultboy_bitmaps+1));
  }
  else    
    bitmap_layer_set_bitmap(layer_battery_vaultboy, *(current_battery_vaultboy_bitmaps+2));
    layer_mark_dirty(bitmap_layer_get_layer(layer_battery_vaultboy));
}


static void battery_bar_update_proc(Layer *layer, GContext *ctx)
{
  
  GRect bounds = layer_get_bounds(layer);

  int width = (battery_level * layer_bar_w) / 100;
  // Draw the background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_fill_color(ctx, text_color);
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone);
  
}
