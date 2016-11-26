#pragma once
#include <pebble.h>

void window_load(Window *window);
void window_unload(Window *window);
void window_update();
void set_up_text_layer(TextLayer *layer, GColor background, GColor text_color, const char * text,GFont font,GTextAlignment alignment);
void change_texts_color(GColor text_color);  //change color of all text layers
void change_layout_colors(GColor new_light, GColor new_medium, GColor new_dark);