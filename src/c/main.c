#include <pebble.h>
#include "global.h"
#include "window.h"
#include "settings.h"
#include "callbacks.h"
#include "time.h"
static void init(void);
static void deinit(void);



int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init(void) {  
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
  
 // register for bluetooth connection updates
   connection_service_subscribe((ConnectionHandlers) {
   .pebble_app_connection_handler = bluetooth_callback
   });
  
  //register for timer tick service
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  
  //get time/date/battery for the first time
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





