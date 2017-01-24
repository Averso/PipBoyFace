#pragma once
#include <pebble.h>

//dimensions & positions
#if PBL_DISPLAY_HEIGHT == 180 //CHALK
#define LAYER_DATE_X 35
#define LAYER_DATE_Y 135
#define LAYER_DATE_W 84
#define LAYER_DATE_H 26

#define LAYER_WEEKDAY_X 125
#define LAYER_WEEKDAY_Y 135
#define LAYER_WEEKDAY_W 24
#define LAYER_WEEKDAY_H 17

#define LAYER_TIME_X 38
#define LAYER_TIME_Y 14
#define LAYER_TIME_W 85
#define LAYER_TIME_H 35

#define LAYER_LABEL_X 70
#define LAYER_LABEL_W 40
#define LAYER_LABEL_H 20

#define LAYER_LABEL_TIME_Y 0
#define LAYER_LABLE_DATE_Y 162


#define LAYER_VAULTBOY_X 51
#define LAYER_VAULTBOY_Y 49

#define BAR_LEFT_X 54
#define BAR_CENTER_X 84
#define BAR_RIGHT_X 115

#define BAR1_CENTER_Y 51
#define BAR2_CENTER_Y 89

#define BAR_LR_TOP_Y 71
#define BAR_LR_BOTTOM_Y 117

#define LAYER_BATTERY_X 127
#define LAYER_BATTERY_Y 30
#define LAYER_BATTERY_W 21
#define LAYER_BATTERY_H 11

#define LAYER_CHARGE_X 129
#define LAYER_CHARGE_Y 27

#define LAYER_BT_X 147
#define LAYER_BT_Y 60

#define LAYER_QUIETTIME_X 147
#define LAYER_QUIETTIME_Y 85


#else // BASALT AND DIORITE

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

#define LAYER_LABEL_X 23
#define LAYER_LABEL_W 39
#define LAYER_LABEL_H 20

#define LAYER_LABEL_TIME_Y -4
#define LAYER_LABLE_DATE_Y 154

#define LAYER_VAULTBOY_X 33
#define LAYER_VAULTBOY_Y 47

#define BAR_LEFT_X 36
#define BAR_CENTER_X 66
#define BAR_RIGHT_X 97

#define BAR1_CENTER_Y 49
#define BAR2_CENTER_Y 87

#define BAR_LR_TOP_Y 69
#define BAR_LR_BOTTOM_Y 115

#define LAYER_BATTERY_X 112
#define LAYER_BATTERY_Y 7
#define LAYER_BATTERY_W 26
#define LAYER_BATTERY_H 20

#define LAYER_CHARGE_X 117
#define LAYER_CHARGE_Y 7

#define LAYER_BT_X 117
#define LAYER_BT_Y 30

#define LAYER_QUIETTIME_X 117
#define LAYER_QUIETTIME_Y 54

#endif

#define LAYER_BT_W 17
#define LAYER_BT_H 18

#define LAYER_CHARGE_W 18
#define LAYER_CHARGE_H 17

#define LAYER_QUIETTIME_W 17
#define LAYER_QUIETTIME_H 18

#define LAYER_BAR_W 11
#define LAYER_BAR_H 2

#define LAYER_VAULTBOY_W 78
#define LAYER_VAULTBOY_H 88

#define BATTERY_LOW_DEFAULT 20
#define BATTERY_MED_DEFAULT 50



Window *main_window;                                    
TextLayer *layer_time, *layer_weekday,*layer_date, *layer_label_time, *layer_label_date, *layer_text_battery;     
GFont font_time, font_weekday,font_date,font_labels, font_battery; 
Layer *layer_bars[6];
BitmapLayer *layer_background, *layer_vaultboy, *layer_bt, *layer_charge, *layer_quiettime;                          
GBitmap *bitmap_background, *bitmap_vaultboy[3],*bitmap_bt,*bitmap_charge, *bitmap_quiettime;
GColor color_light;

#ifdef PBL_COLOR
GColor color_medium, color_dark;
#endif

int battery_level;

#ifdef PBL_COLOR
typedef enum {WHITE,GREEN,BLUE,AMBER} ScreenColor;
#endif


//settings struct

typedef struct ClaySettings {
  #ifdef PBL_COLOR
  ScreenColor screen_color;
  #endif
  bool vibe_on_disconnect;
  int battery_medium_level;
  int battery_low_level;
  bool date_format;
} ClaySettings;

// An instance of the struct
ClaySettings settings;
