#include <pebble.h>
#include "global.h"
#include "window.h"
#include "callbacks.h"
#include "gbitmap_color_palette_manipulator.h"

//colors in vaultboy
//GColorWhite, GColorLightGray, GColorDarkGray | white
//GColorMalachite, GColorIslamicGreen, GColorDarkGreen | green
//GColorVividCerulean, GColorCobaltBlue, GColorOxfordBlue | blue
//GColorRajah, GColorWindsorTan, GColorBulgarianRose | amber




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

  //colors
  color_light=GColorWhite;
  color_medium=GColorLightGray;
  color_dark=GColorDarkGray;
  
  //BACKGROUND 
  
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);  
  layer_background = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(layer_background,bitmap_background);
  
  //VAULTBOY  
  bitmap_vaultboy[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE);
  bitmap_vaultboy[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM);
  bitmap_vaultboy[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD);
  
  layer_vaultboy = bitmap_layer_create(
    GRect(LAYER_VAULTBOY_X, LAYER_VAULTBOY_Y, LAYER_VAULTBOY_W, LAYER_VAULTBOY_H));
  bitmap_layer_set_bitmap(layer_vaultboy,bitmap_vaultboy[0]);
  
  
  //BLUETOOTH ICON
  bitmap_bt = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT);
  
  layer_bt = bitmap_layer_create(GRect(LAYER_ICON_X, LAYER_BT_Y, LAYER_ICON_W, LAYER_ICON_H));
  bitmap_layer_set_bitmap(layer_bt, bitmap_bt);
  
  //CHARGE ICON
  bitmap_charge = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE);
  
  //h and w switched as charge image has diffrent dimensions
  layer_charge = bitmap_layer_create(GRect(LAYER_ICON_X, LAYER_BATTERY_Y, LAYER_ICON_H, LAYER_ICON_W));
  bitmap_layer_set_bitmap(layer_charge, bitmap_charge);
  
  
  //HEALTH-BATTERY BARS
  layer_bars[0] = layer_create(GRect(66, 49, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[1] = layer_create(GRect(66, 87, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[2] = layer_create(GRect(36, 69, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[3] = layer_create(GRect(36, 115, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[4] = layer_create(GRect(97, 69, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[5] = layer_create(GRect(97, 115, LAYER_BAR_W, LAYER_BAR_H));
  
  //assign bars to layer update proc
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
  
  

  set_up_text_layer(layer_time, GColorClear, color_light, "00:00", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday, GColorClear, color_light, "Mon", font_weekday,GTextAlignmentLeft);
  set_up_text_layer(layer_date, GColorClear, color_light, "11.11.16", font_date,GTextAlignmentCenter);
  set_up_text_layer(layer_text_time, GColorClear, color_light, "TIME", font_others,GTextAlignmentCenter);
  set_up_text_layer(layer_text_date, GColorClear, color_light, "DATE", font_others,GTextAlignmentCenter);
  set_up_text_layer(layer_text_battery, GColorClear, color_light, "100%", font_battery,GTextAlignmentCenter);
  
      
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_background));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_vaultboy));
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
  bitmap_layer_set_compositing_mode(layer_vaultboy, GCompOpSet); 
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
  
  
    gbitmap_destroy(bitmap_background);      
    gbitmap_destroy(bitmap_bt);  
    gbitmap_destroy(bitmap_charge);  
  
  
  //destroy vaultboys bitmaps
  for(int i=0; i<3;i++)
  {  
    gbitmap_destroy(bitmap_vaultboy[i]);      
    
  }
  
  bitmap_layer_destroy(layer_background);
  bitmap_layer_destroy(layer_vaultboy);
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
      change_layout_colors(GColorWhite,GColorLightGray,GColorDarkGray);     
      break;
    case GREEN:
      change_layout_colors(GColorMalachite, GColorIslamicGreen, GColorDarkGreen);
      break;
    case BLUE:
      change_layout_colors(GColorVividCerulean, GColorCobaltBlue, GColorOxfordBlue);
      break;
    case AMBER:
      change_layout_colors(GColorRajah, GColorWindsorTan, GColorBulgarianRose);
      break;      
  }      
  //refresh vault boy
  battery_update_vaultboy();
  
  change_texts_color(color_light);
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


void change_layout_colors(GColor new_light, GColor new_medium, GColor new_dark)
{
  //bg          
  replace_gbitmap_color(color_light, new_light,bitmap_background, layer_background);
  replace_gbitmap_color(color_medium, new_medium,bitmap_background, layer_background);
  replace_gbitmap_color(color_dark, new_dark,bitmap_background, layer_background);
  //vaultboy
  for(int i=0; i<3;i++)
  {
    replace_gbitmap_color(color_light, new_light, bitmap_vaultboy[i], layer_vaultboy);
    replace_gbitmap_color(color_medium, new_medium, bitmap_vaultboy[i], layer_vaultboy);           
  }
  //bluetooth
  replace_gbitmap_color(color_light, new_light,bitmap_bt, layer_bt);
  //charge
  replace_gbitmap_color(color_light, new_light,bitmap_charge, layer_charge);
  //new colors
  color_light = new_light;
  color_medium = new_medium;
  color_dark = new_dark;
}
