// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Smart_Gadget

#ifndef _SMART_GADGET_UI_H
#define _SMART_GADGET_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
void Up_Animation(lv_obj_t * TargetObject, int delay);
void hour_Animation(lv_obj_t * TargetObject, int delay);
void min_Animation(lv_obj_t * TargetObject, int delay);
void sec_Animation(lv_obj_t * TargetObject, int delay);
void scrolldot_Animation(lv_obj_t * TargetObject, int delay);
// SCREEN: ui_Splash
void ui_Splash_screen_init(void);
void ui_event_Splash(lv_event_t * e);
extern lv_obj_t * ui_Splash;
extern lv_obj_t * ui_Demo;
extern lv_obj_t * ui_Smart_Gadget;
extern lv_obj_t * ui_SLS_Logo;
// SCREEN: ui_Weather
void ui_Weather_screen_init(void);
void ui_event_Weather(lv_event_t * e);
extern lv_obj_t * ui_Weather;
extern lv_obj_t * ui_Pary_Cloud;
extern lv_obj_t * ui_New_York;
extern lv_obj_t * ui_Cloud;
extern lv_obj_t * ui_Celsius;
extern lv_obj_t * ui_Weather_Icons;
extern lv_obj_t * ui_w1;
extern lv_obj_t * ui_w2;
extern lv_obj_t * ui_w3;
extern lv_obj_t * ui_W1_Num;
extern lv_obj_t * ui_W2_Num;
extern lv_obj_t * ui_W3_Num;
extern lv_obj_t * ui_Scrolldots4;
// SCREEN: ui_Clock
void ui_Clock_screen_init(void);
void ui_event_Clock(lv_event_t * e);
extern lv_obj_t * ui_Clock;
extern lv_obj_t * ui_Clock_Panel;
extern lv_obj_t * ui_Dot1;
extern lv_obj_t * ui_Dot2;
extern lv_obj_t * ui_Dot3;
extern lv_obj_t * ui_Dot4;
extern lv_obj_t * ui_Dot5;
extern lv_obj_t * ui_Dot6;
extern lv_obj_t * ui_Dot7;
extern lv_obj_t * ui_Dot8;
extern lv_obj_t * ui_Clock_Number1;
extern lv_obj_t * ui_Clock_Number2;
extern lv_obj_t * ui_Clock_Number3;
extern lv_obj_t * ui_Clock_Number4;
extern lv_obj_t * ui_Min;
extern lv_obj_t * ui_Hour;
extern lv_obj_t * ui_Sec;
extern lv_obj_t * ui_Clock_Center;
extern lv_obj_t * ui_Clock_Number;
extern lv_obj_t * ui_Date;
extern lv_obj_t * ui_Scrolldots;
// SCREEN: ui_Chat
void ui_Chat_screen_init(void);
void ui_event_Chat(lv_event_t * e);
extern lv_obj_t * ui_Chat;
extern lv_obj_t * ui_Chat_container;
extern lv_obj_t * ui_Chat_date;
extern lv_obj_t * ui_C1;
extern lv_obj_t * ui_Chat_Panel1;
extern lv_obj_t * ui_Chat1;
extern lv_obj_t * ui_Chat_Icon1;
extern lv_obj_t * ui_C2;
extern lv_obj_t * ui_Chat_Panel2;
extern lv_obj_t * ui_Chat2;
extern lv_obj_t * ui_Chat_Icon2;
extern lv_obj_t * ui_Delifered;
extern lv_obj_t * ui_C3;
extern lv_obj_t * ui_Chat_Panel3;
extern lv_obj_t * ui_Chat3;
extern lv_obj_t * ui_Chat_Icon3;
extern lv_obj_t * ui_Scrolldots2;
// SCREEN: ui_Music_Player
void ui_Music_Player_screen_init(void);
void ui_event_Music_Player(lv_event_t * e);
extern lv_obj_t * ui_Music_Player;
extern lv_obj_t * ui_Music_Title;
extern lv_obj_t * ui_Author;
extern lv_obj_t * ui_Play_btn;
extern lv_obj_t * ui_Play;
extern lv_obj_t * ui_Album;
extern lv_obj_t * ui_Backward;
extern lv_obj_t * ui_Forward;
extern lv_obj_t * ui_Scrolldots3;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_sls_logo_png);    // assets\sls_logo.png
LV_IMG_DECLARE(ui_img_pattern_png);    // assets\pattern.png
LV_IMG_DECLARE(ui_img_cloud_png);    // assets\cloud.png
LV_IMG_DECLARE(ui_img_weather_1_png);    // assets\weather_1.png
LV_IMG_DECLARE(ui_img_weather_2_png);    // assets\weather_2.png
LV_IMG_DECLARE(ui_img_weather_3_png);    // assets\weather_3.png
LV_IMG_DECLARE(ui_img_clock_min_png);    // assets\clock_min.png
LV_IMG_DECLARE(ui_img_clock_hour_png);    // assets\clock_hour.png
LV_IMG_DECLARE(ui_img_clock_sec_png);    // assets\clock_sec.png
LV_IMG_DECLARE(ui_img_chatbox_png);    // assets\chatbox.png
LV_IMG_DECLARE(ui_img_chatbox2_png);    // assets\chatbox2.png
LV_IMG_DECLARE(ui_img_play_png);    // assets\play.png
LV_IMG_DECLARE(ui_img_album_png);    // assets\album.png
LV_IMG_DECLARE(ui_img_backward_png);    // assets\backward.png
LV_IMG_DECLARE(ui_img_forward_png);    // assets\forward.png

LV_FONT_DECLARE(ui_font_Number);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
