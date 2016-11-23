#include <pebble.h>
#include "global.h"
#include "window.h"
#include "callbacks.h"

void window_load(Window *window)  
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
    GRect(LAYER_VAULTBOY_X, LAYER_VAULTBOY_Y, LAYER_VAULTBOY_W, LAYER_VAULTBOY_H));
  bitmap_layer_set_bitmap(layer_battery_vaultboy,*current_battery_vaultboy_bitmaps);
  
  
  //BLUETOOTH ICON
  bitmap_bt[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_WHITE);
  bitmap_bt[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_GREEN);
  bitmap_bt[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_BLUE);
  bitmap_bt[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_AMBER);
  
  layer_bt = bitmap_layer_create(GRect(LAYER_ICON_X, LAYER_BT_Y, LAYER_ICON_W, LAYER_ICON_H));
  bitmap_layer_set_bitmap(layer_bt, bitmap_bt[0]);
  
  //CHARGE ICON
  bitmap_charge[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_WHITE);
  bitmap_charge[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_GREEN);
  bitmap_charge[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_BLUE);
  bitmap_charge[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE_AMBER);
  
  //h and w switched as charge image has diffrent dimensions
  layer_charge = bitmap_layer_create(GRect(LAYER_ICON_X, LAYER_BATTERY_Y, LAYER_ICON_H, LAYER_ICON_W));
  bitmap_layer_set_bitmap(layer_charge, bitmap_charge[0]);
  
  
  //HEALTH-BATTERY BARS
  layer_bars[0] = layer_create(GRect(66, 49, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[1] = layer_create(GRect(66, 87, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[2] = layer_create(GRect(36, 69, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[3] = layer_create(GRect(36, 115, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[4] = layer_create(GRect(97, 69, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[5] = layer_create(GRect(97, 115, LAYER_BAR_W, LAYER_BAR_H));
  
  //assign to layer update proc
  for(int i=0;i<6;i++)
    layer_set_update_proc(layer_bars[i], battery_bar_update_proc);
  
  //TEXT    
  //weekday 
  layer_time = text_layer_create(
    GRect(LAYER_TIME_X, LAYER_TIME_Y, LAYER_TIME_W, LAYER_TIME_H));  
  layer_weekday = text_layer_create(
    GRect(LAYER_WEEKDAY_X, LAYER_WEEKDAY_Y, LAYER_WEEKDAY_W, LAYER_WEEKDAY_H));   
  layer_date = text_layer_create(
    GRect(LAYER_DATE_X, LAYER_DATE_Y,LAYER_DATE_W, LAYER_DATE_H));
  layer_text_time = text_layer_create(GRect(23,-4, 39, 20));  
  layer_text_date = text_layer_create(GRect(23, 154,39, 20));
  layer_text_battery = text_layer_create(GRect(112, LAYER_BATTERY_Y, 26, 20));
  
  

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

void window_unload(Window *window)
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

void window_update()
{  
  
  //check bt status
  bluetooth_callback(connection_service_peek_pebble_app_connection());  
  
  
  //change colors of layout
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

void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

void change_texts_color(GColor text_color)
{  
  text_layer_set_text_color(layer_time, text_color);
  text_layer_set_text_color(layer_weekday, text_color);
  text_layer_set_text_color(layer_date, text_color);
  text_layer_set_text_color(layer_text_time, text_color);
  text_layer_set_text_color(layer_text_date, text_color);
  text_layer_set_text_color(layer_text_battery, text_color);
 
}
