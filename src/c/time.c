#include <pebble.h>
#include "time.h"
#include "global.h"

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
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

void update_time()
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get time 
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);  
  
  //display time on text layer
  text_layer_set_text(layer_time, s_buffer);
  
   //show/hide quiet time icon depending if it's on
    layer_set_hidden(bitmap_layer_get_layer(layer_quiettime), !quiet_time_is_active());
}

void update_weekday() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //get weekday
  static char s_buffer[4];
  strftime(s_buffer, sizeof(s_buffer),"%a", tick_time);
  
  //change text on weekday text layers
  text_layer_set_text(layer_weekday, s_buffer);
  
  
}

void update_date() 
{
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char s_buffer[10];
  
  
    strftime(s_buffer, sizeof(s_buffer),settings.date_format ? "%m.%d.%y" : "%d.%m.%y", tick_time);
  
  text_layer_set_text(layer_date, s_buffer);
}
