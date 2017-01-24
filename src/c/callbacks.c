#include <pebble.h>
#include "global.h"
#include "callbacks.h"

static bool is_connected = true; //to vibrate once

//BLUETOOTH
void bluetooth_callback(bool connected) { 
 
  if(persist_exists(12))
  {
     is_connected = persist_read_bool(12);
  }
  
  //show/hide bluetooth icon depending on connection
  layer_set_hidden(bitmap_layer_get_layer(layer_bt), connected);
  
  
  //vibrate just once based on previous status
  if((is_connected && !connected) && settings.vibe_on_disconnect)
    vibes_double_pulse();
  
  is_connected = connected;
  persist_write_bool(12,is_connected);
  
}


//BATTERY
void battery_callback(BatteryChargeState state)
{
  
  //mark helath-battery bars dirty
  for(int i=0;i<6;i++)
    layer_mark_dirty(layer_bars[i]);
  
  //show/hide battery level layer and charging icon
  layer_set_hidden(text_layer_get_layer(layer_text_battery), state.is_charging);  
  layer_set_hidden(bitmap_layer_get_layer(layer_charge), !(state.is_charging));
  
  // change text in text layer
  static char s_buffer[5];
  snprintf(s_buffer, sizeof(s_buffer), "%d%%", state.charge_percent);  
  text_layer_set_text(layer_text_battery, s_buffer);    
  
  //save battery level
  battery_level = state.charge_percent;
  //change voultboy apperance
  battery_update_vaultboy();   
  
}

void battery_update_vaultboy()
{ 
  //change vaultboy image depending on battery lvl  
  if(battery_level<=100 && battery_level>settings.battery_medium_level)
  {  
    bitmap_layer_set_bitmap(layer_vaultboy, bitmap_vaultboy[0]);
  }
  else if (battery_level<=settings.battery_medium_level && battery_level>settings.battery_low_level)
  {  
    bitmap_layer_set_bitmap(layer_vaultboy, bitmap_vaultboy[1]);
  }
  else    
    bitmap_layer_set_bitmap(layer_vaultboy, bitmap_vaultboy[2]);
  
  //not sure if necessary
  layer_mark_dirty(bitmap_layer_get_layer(layer_vaultboy));
}

void battery_bar_update_proc(Layer *layer, GContext *ctx)
{
  
  GRect bounds = layer_get_bounds(layer);

  int width = (battery_level * LAYER_BAR_W) / 100;
  // Draw the background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  graphics_context_set_fill_color(ctx, color_light);
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone);
  
}
