#include <pebble.h>
#include "global.h"
#include "settings.h"
#include "window.h"

void save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));  
  //refresh window
  window_update();
}

void load_default_settings() {
  #if defined(PBL_PLATFORM_BASALT)
  settings.screen_color = WHITE; 
  #endif
  settings.vibe_on_disconnect = false;
  settings.battery_medium_level=BATTERY_MED_DEFAULT;
  settings.battery_low_level=BATTERY_LOW_DEFAULT;
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  //assign the values from settings to struct
  //vibration on disconnect
  Tuple *vibe_enabled = dict_find(iter, MESSAGE_KEY_vibe_disconnect);
  if(vibe_enabled) {
   settings.vibe_on_disconnect = vibe_enabled->value->int32 == 1;
  }  
  
  #if defined(PBL_PLATFORM_BASALT)
  //color
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
  #endif
  
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
    //if is, set them to default
    settings.battery_low_level = BATTERY_LOW_DEFAULT;
    settings.battery_medium_level = BATTERY_MED_DEFAULT;
  }
  else 
  {
    settings.battery_low_level = low_lvl;
    settings.battery_medium_level = med_lvl;
  }
 
  
  //save loaded settings
  save_settings();
}

void load_settings() {
  // Load the default settings
  load_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}


