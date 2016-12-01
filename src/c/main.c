#include <pebble.h>
#include "global.h"
#include "window.h"
#include "settings.h"
#include "callbacks.h"

void init(void);
void deinit(void);

static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
//handle fetching time/date data
static void update_time();  
static void update_weekday();
static void update_date();


int main(void) {
  init();
  app_event_loop();
  deinit();
}

void init(void) {
  
  //try to load settings
  load_settings();
  
  //listen for appmessages (manage settings)
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 128);
    
  //manage window
  main_window = window_create();
  
  window_set_window_handlers(main_window,(WindowHandlers) {
        .load = window_load,
        .unload = window_unload
  });

  window_stack_push(main_window, true);
  
  //register for battery level updates
  battery_state_service_subscribe(battery_callback);
  
  //register for bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bluetooth_callback
  });
  
  //register for timer tick service
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  //get tima/date/battery for the first time
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

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  if (units_changed & MINUTE_UNIT)
  {
     update_time();
     //show/hide quiet time icon depending if it's on
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Quiet time: %d", quiet_time_is_active());
     layer_set_hidden(bitmap_layer_get_layer(layer_quiettime), !quiet_time_is_active());
  
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





