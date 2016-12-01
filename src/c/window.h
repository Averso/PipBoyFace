#pragma once
#include <pebble.h>

void window_load(Window *window);
void window_unload(Window *window);
void window_update();
void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment);
void load_resources();
void create_bitmap_layers(GRect bounds);
void create_text_layers();
void add_layers_to_window(Layer *window_layer);
#ifdef PBL_COLOR
void change_texts_color(GColor text_color);  //change color of all text layers
void change_layout_colors(GColor new_light, GColor new_medium, GColor new_dark);
#endif