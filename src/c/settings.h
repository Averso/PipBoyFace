#pragma once
#include <pebble.h>

// Persistent storage key
#define SETTINGS_KEY 1

void inbox_received_handler(DictionaryIterator *iter, void *context); //receive settings from pebble
void load_default_settings();  //load default settings
void save_settings();   //save settings
void load_settings();


