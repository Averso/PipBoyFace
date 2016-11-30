#pragma once
#include <pebble.h>

//dimensions & positions
#define LAYER_DATE_X 7
#define LAYER_DATE_Y 134
#define LAYER_DATE_W 87
#define LAYER_DATE_H 26

#define LAYER_WEEKDAY_X 106
#define LAYER_WEEKDAY_Y 137
#define LAYER_WEEKDAY_W 32
#define LAYER_WEEKDAY_H 25

#define LAYER_TIME_X 7
#define LAYER_TIME_Y 5
#define LAYER_TIME_W 100
#define LAYER_TIME_H 37

#define LAYER_ICON_X 117
#define LAYER_ICON_W 17
#define LAYER_ICON_H 18

#define LAYER_BATTERY_Y 7
#define LAYER_BT_Y 30

#define LAYER_BAR_W 11
#define LAYER_BAR_H 2

#define LAYER_VAULTBOY_X 33
#define LAYER_VAULTBOY_Y 47
#define LAYER_VAULTBOY_W 78
#define LAYER_VAULTBOY_H 88

#define BATTERY_LOW_DEFAULT 20
#define BATTERY_MED_DEFAULT 50

Window *main_window;                                    
TextLayer *layer_time, *layer_weekday,*layer_date, *layer_text_time, *layer_text_date, *layer_text_battery;     
GFont font_time, font_weekday,font_date,font_others, font_battery; 
Layer *layer_bars[6];
BitmapLayer *layer_background, *layer_vaultboy, *layer_bt, *layer_charge;                          
GBitmap *bitmap_background, *bitmap_vaultboy[3],*bitmap_bt,*bitmap_charge;
GColor color_light;

#if defined(PBL_PLATFORM_BASALT)
GColor color_medium, color_dark;
#endif

int battery_level;

#if defined(PBL_PLATFORM_BASALT)
typedef enum {WHITE,GREEN,BLUE,AMBER} ScreenColor;
#endif


//settings struct
#if defined(PBL_PLATFORM_BASALT)
typedef struct ClaySettings {
  ScreenColor screen_color;
  bool vibe_on_disconnect;
  int battery_medium_level;
  int battery_low_level;
} ClaySettings;
#else
typedef struct ClaySettings {
  bool vibe_on_disconnect;
  int battery_medium_level;
  int battery_low_level;
} ClaySettings;
#endif

// An instance of the struct
ClaySettings settings;
