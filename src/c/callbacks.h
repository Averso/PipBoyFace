#pragma once
#include <pebble.h>

 
void battery_callback(BatteryChargeState state);
void bluetooth_callback(bool connected); 
void battery_update_vaultboy();    //change vaultboy image depending on battery level
void battery_bar_update_proc(Layer *layer, GContext *ctx);    //function to update battery bars
