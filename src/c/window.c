#include <pebble.h>
#include "global.h"
#include "window.h"
#include "callbacks.h"
#include "gbitmap_color_palette_manipulator.h"
#include "time.h"

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
   

  //colors  
  color_light=GColorWhite;
  #ifdef PBL_COLOR
  color_medium=GColorLightGray;
  color_dark=GColorDarkGray;
  #endif
  
  load_resources(); 
  create_bitmap_layers(bounds);
  create_text_layers();
  
  
  //HEALTH-BATTERY BARS
  layer_bars[0] = layer_create(GRect(BAR_CENTER_X, BAR1_CENTER_Y, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[1] = layer_create(GRect(BAR_CENTER_X, BAR2_CENTER_Y, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[2] = layer_create(GRect(BAR_LEFT_X, BAR_LR_TOP_Y, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[3] = layer_create(GRect(BAR_LEFT_X, BAR_LR_BOTTOM_Y, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[4] = layer_create(GRect(BAR_RIGHT_X, BAR_LR_TOP_Y, LAYER_BAR_W, LAYER_BAR_H));
  layer_bars[5] = layer_create(GRect(BAR_RIGHT_X, BAR_LR_BOTTOM_Y, LAYER_BAR_W, LAYER_BAR_H));
  
  //assign bars to layer update proc
  for(int i=0;i<6;i++)
    layer_set_update_proc(layer_bars[i], battery_bar_update_proc);
  
  add_layers_to_window(window_layer);
  
  //turn on transparency
  bitmap_layer_set_compositing_mode(layer_vaultboy, GCompOpSet); 
  bitmap_layer_set_compositing_mode(layer_bt, GCompOpSet);   
  bitmap_layer_set_compositing_mode(layer_charge, GCompOpSet); 
  bitmap_layer_set_compositing_mode(layer_quiettime, GCompOpSet); 
  
  //hide bt&charge icon
  layer_set_hidden(bitmap_layer_get_layer(layer_bt), true);
  layer_set_hidden(bitmap_layer_get_layer(layer_charge), true);
  layer_set_hidden(bitmap_layer_get_layer(layer_quiettime), true);
  
  window_update();
  
}

void window_unload(Window *window)
{
  //destroy text layers
  text_layer_destroy(layer_time);
  text_layer_destroy(layer_weekday);
  text_layer_destroy(layer_date);  
  text_layer_destroy(layer_label_time);
  text_layer_destroy(layer_label_date);
  text_layer_destroy(layer_text_battery);
  
  
  //unload fonts
  fonts_unload_custom_font(font_time);
  fonts_unload_custom_font(font_weekday);
  fonts_unload_custom_font(font_date);
  fonts_unload_custom_font(font_labels); 
  fonts_unload_custom_font(font_battery);
 
  //destroy bitmaps   
  gbitmap_destroy(bitmap_background);      
  gbitmap_destroy(bitmap_bt);  
  gbitmap_destroy(bitmap_charge);  
  gbitmap_destroy(bitmap_quiettime);  
  
  
  //destroy vaultboys bitmaps
  for(int i=0; i<3;i++)
  {  
    gbitmap_destroy(bitmap_vaultboy[i]);      
    
  }
  
  bitmap_layer_destroy(layer_background);
  bitmap_layer_destroy(layer_vaultboy);
  bitmap_layer_destroy(layer_bt);
  bitmap_layer_destroy(layer_quiettime);
  
  for(int i=0; i<6;i++)
  {  
    layer_destroy(layer_bars[i]);
  }
  
}

void window_update()
{  
  
  //check bt status
  bluetooth_callback(connection_service_peek_pebble_app_connection());  
  
  #ifdef PBL_COLOR
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
  change_texts_color(color_light);
  #endif
  
  //refresh vault boy
  battery_update_vaultboy();
  update_time();
  update_date();

}

void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment)
{
  text_layer_set_background_color(layer, background);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer,alignment); 
}

void load_resources()
{
  //FONTS
  #if PBL_DISPLAY_HEIGHT == 180
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_TIME_32));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_DAY_14));
  font_battery = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_BTTRY_10));
  #else
  font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_TIME_36));
  font_weekday = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_DAY_18));
  font_battery = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_BTTRY_12));
  #endif
    
  font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_DATE_19));
  font_labels = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MONOFONTO_LABELS_14));
  
  //BG
  bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG);  
  
  //VAULTBOYS
  bitmap_vaultboy[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_FINE);
  bitmap_vaultboy[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_MEDIUM);
  bitmap_vaultboy[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VAULTBOY_BAD);

  
  bitmap_bt = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT);
  bitmap_charge = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHARGE);
  bitmap_quiettime = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ICON_QUIET);
  
}

void create_bitmap_layers(GRect bounds)
{ 
  
  layer_background = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(layer_background,bitmap_background);    
  
  layer_vaultboy = bitmap_layer_create(
    GRect(LAYER_VAULTBOY_X, LAYER_VAULTBOY_Y, LAYER_VAULTBOY_W, LAYER_VAULTBOY_H));
  bitmap_layer_set_bitmap(layer_vaultboy,bitmap_vaultboy[0]);  
  
  
  layer_bt = bitmap_layer_create(GRect(LAYER_BT_X, LAYER_BT_Y, LAYER_BT_W, LAYER_BT_H));
  bitmap_layer_set_bitmap(layer_bt, bitmap_bt);
  
  
  layer_charge = bitmap_layer_create(GRect(LAYER_CHARGE_X, LAYER_CHARGE_Y, LAYER_CHARGE_W, LAYER_CHARGE_H));
  bitmap_layer_set_bitmap(layer_charge, bitmap_charge);
  
  layer_quiettime = bitmap_layer_create(GRect(LAYER_QUIETTIME_X,LAYER_QUIETTIME_Y,LAYER_QUIETTIME_W,LAYER_QUIETTIME_H));
  bitmap_layer_set_bitmap(layer_quiettime,bitmap_quiettime);
}

void create_text_layers()
{  
  layer_time = text_layer_create(
    GRect(LAYER_TIME_X, LAYER_TIME_Y, LAYER_TIME_W, LAYER_TIME_H));  
  layer_weekday = text_layer_create(
    GRect(LAYER_WEEKDAY_X, LAYER_WEEKDAY_Y, LAYER_WEEKDAY_W, LAYER_WEEKDAY_H));   
  layer_date = text_layer_create(
    GRect(LAYER_DATE_X, LAYER_DATE_Y,LAYER_DATE_W, LAYER_DATE_H));
  layer_label_time = text_layer_create(GRect(LAYER_LABEL_X,LAYER_LABEL_TIME_Y, LAYER_LABEL_W, LAYER_LABEL_H));  
  layer_label_date = text_layer_create(GRect(LAYER_LABEL_X, LAYER_LABLE_DATE_Y,LAYER_LABEL_W, LAYER_LABEL_H));
  
  layer_text_battery = text_layer_create(GRect(LAYER_BATTERY_X, LAYER_BATTERY_Y, LAYER_BATTERY_W, LAYER_BATTERY_H));


  set_up_text_layer(layer_time, GColorClear, color_light, "44:44", font_time,GTextAlignmentCenter);
  set_up_text_layer(layer_weekday, GColorClear, color_light, "Mon", font_weekday,GTextAlignmentCenter);
  set_up_text_layer(layer_date, GColorClear, color_light, "44.44.44", font_date,GTextAlignmentCenter);
  set_up_text_layer(layer_label_time, GColorClear, color_light, "TIME", font_labels,GTextAlignmentCenter);
  set_up_text_layer(layer_label_date, GColorClear, color_light, "DATE", font_labels,GTextAlignmentCenter);
  set_up_text_layer(layer_text_battery, GColorClear, color_light, "100%", font_battery,GTextAlignmentCenter);
}

void add_layers_to_window(Layer *window_layer)
{
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_background));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_vaultboy));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_bt));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_quiettime));
  layer_add_child(window_layer,bitmap_layer_get_layer(layer_charge));
  layer_add_child(window_layer,text_layer_get_layer(layer_label_time));
  layer_add_child(window_layer,text_layer_get_layer(layer_label_date));
  layer_add_child(window_layer,text_layer_get_layer(layer_weekday));
  layer_add_child(window_layer,text_layer_get_layer(layer_time));  
  layer_add_child(window_layer,text_layer_get_layer(layer_date));
  layer_add_child(window_layer,text_layer_get_layer(layer_text_battery));
  
  for(int i=0;i<6;i++)
   layer_add_child(window_layer,layer_bars[i]);
}

#ifdef PBL_COLOR
void change_texts_color(GColor color)
{  
  text_layer_set_text_color(layer_time, color);
  text_layer_set_text_color(layer_weekday, color);
  text_layer_set_text_color(layer_date, color);
  text_layer_set_text_color(layer_label_time, color);
  text_layer_set_text_color(layer_label_date, color);
  text_layer_set_text_color(layer_text_battery, color);
 
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
  //quiet time
  replace_gbitmap_color(color_light, new_light,bitmap_quiettime, layer_quiettime);
  //new colors
  color_light = new_light;
  color_medium = new_medium;
  color_dark = new_dark;
}
#endif
