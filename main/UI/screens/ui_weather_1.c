// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.4
// Project name: Smartwatch

#include "../ui.h"

void ui_weather_1_screen_init(void)
{
    ui_weather_1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_weather_1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_bg_4 = lv_img_create(ui_weather_1);
    lv_img_set_src(ui_bg_4, &ui_img_bg3_png);
    lv_obj_set_width(ui_bg_4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_bg_4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_bg_4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_bg_4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_bg_4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_city_gruop_2 = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_city_gruop_2, 200);
    lv_obj_set_height(ui_city_gruop_2, 50);
    lv_obj_set_x(ui_city_gruop_2, 20);
    lv_obj_set_y(ui_city_gruop_2, 20);
    lv_obj_set_flex_flow(ui_city_gruop_2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_city_gruop_2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_city_gruop_2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_city_gruop_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_city_gruop_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_city_2 = lv_label_create(ui_city_gruop_2);
    lv_obj_set_width(ui_city_2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_city_2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_city_2, -28);
    lv_obj_set_y(ui_city_2, 0);
    lv_obj_set_align(ui_city_2, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_city_2, "New York");
    lv_obj_set_style_text_color(ui_city_2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_city_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_city_2, &ui_font_Subtitle, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_date_2 = lv_label_create(ui_city_gruop_2);
    lv_obj_set_width(ui_date_2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_date_2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_date_2, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(ui_date_2, "03. 01. Monday");
    lv_obj_set_style_text_color(ui_date_2, lv_color_hex(0x6D6D6D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_date_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_date_2, &ui_font_Title, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_weather_title_group_4 = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_weather_title_group_4, 200);
    lv_obj_set_height(ui_weather_title_group_4, 50);
    lv_obj_set_x(ui_weather_title_group_4, 20);
    lv_obj_set_y(ui_weather_title_group_4, 60);
    lv_obj_set_align(ui_weather_title_group_4, LV_ALIGN_LEFT_MID);
    lv_obj_set_flex_flow(ui_weather_title_group_4, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_weather_title_group_4, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_weather_title_group_4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_weather_title_group_4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_weather_title_group_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_weather_text_4 = lv_label_create(ui_weather_title_group_4);
    lv_obj_set_width(ui_weather_text_4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_weather_text_4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_weather_text_4, -28);
    lv_obj_set_y(ui_weather_text_4, 0);
    lv_obj_set_align(ui_weather_text_4, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_weather_text_4, "Partly Cloudy");
    lv_obj_set_style_text_color(ui_weather_text_4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_weather_text_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_weather_text_4, &ui_font_Title, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_weather_degree_4 = lv_label_create(ui_weather_title_group_4);
    lv_obj_set_width(ui_weather_degree_4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_weather_degree_4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_weather_degree_4, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(ui_weather_degree_4, "Max: 18° Min: 12°");
    lv_obj_set_style_text_color(ui_weather_degree_4, lv_color_hex(0x6D6D6D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_weather_degree_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_weather_degree_4, &ui_font_Title, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_label_degree = lv_label_create(ui_weather_1);
    lv_obj_set_width(ui_label_degree, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_label_degree, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_label_degree, -64);
    lv_obj_set_y(ui_label_degree, -20);
    lv_obj_set_align(ui_label_degree, LV_ALIGN_CENTER);
    lv_label_set_text(ui_label_degree, "25°");
    lv_obj_set_style_text_color(ui_label_degree, lv_color_hex(0xEE1E1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_label_degree, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_label_degree, &ui_font_Number_big, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_rain_group = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_rain_group, 68);
    lv_obj_set_height(ui_rain_group, 33);
    lv_obj_set_x(ui_rain_group, 20);
    lv_obj_set_y(ui_rain_group, -50);
    lv_obj_set_align(ui_rain_group, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_flex_flow(ui_rain_group, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_rain_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_rain_group, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_rain_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_rain_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_rain_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_rain_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_rain_icon = lv_img_create(ui_rain_group);
    lv_img_set_src(ui_rain_icon, &ui_img_rain_png);
    lv_obj_set_width(ui_rain_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_rain_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_rain_icon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_rain_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_rain_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_rain_percent = lv_label_create(ui_rain_group);
    lv_obj_set_width(ui_rain_percent, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_rain_percent, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_rain_percent, LV_ALIGN_CENTER);
    lv_label_set_text(ui_rain_percent, "18%");
    lv_obj_set_style_text_color(ui_rain_percent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_rain_percent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_rain_percent, &ui_font_Title, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wind_group = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_wind_group, 115);
    lv_obj_set_height(ui_wind_group, 33);
    lv_obj_set_x(ui_wind_group, 185);
    lv_obj_set_y(ui_wind_group, -50);
    lv_obj_set_align(ui_wind_group, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_flex_flow(ui_wind_group, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_wind_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_wind_group, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_wind_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_wind_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_wind_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_wind_group, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_wind_icon = lv_img_create(ui_wind_group);
    lv_img_set_src(ui_wind_icon, &ui_img_wind_png);
    lv_obj_set_width(ui_wind_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wind_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_wind_icon, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_wind_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_wind_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_wind_speed = lv_label_create(ui_wind_group);
    lv_obj_set_width(ui_wind_speed, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_wind_speed, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_wind_speed, LV_ALIGN_CENTER);
    lv_label_set_text(ui_wind_speed, "54 km/h");
    lv_obj_set_style_text_color(ui_wind_speed, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_wind_speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_wind_speed, &ui_font_Title, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_weather_image_group = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_weather_image_group, 240);
    lv_obj_set_height(ui_weather_image_group, 306);
    lv_obj_set_x(ui_weather_image_group, 14);
    lv_obj_set_y(ui_weather_image_group, 0);
    lv_obj_set_align(ui_weather_image_group, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_weather_image_group, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_weather_image_group, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_weather_image_group, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_sun = lv_img_create(ui_weather_image_group);
    lv_img_set_src(ui_sun, &ui_img_sun_png);
    lv_obj_set_width(ui_sun, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_sun, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_sun, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_sun, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_sun, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_clouds = lv_img_create(ui_weather_image_group);
    lv_img_set_src(ui_clouds, &ui_img_clouds_png);
    lv_obj_set_width(ui_clouds, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_clouds, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_clouds, 38);
    lv_obj_set_y(ui_clouds, -13);
    lv_obj_set_align(ui_clouds, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_clouds, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_clouds, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_button_down4 = lv_obj_create(ui_weather_1);
    lv_obj_set_width(ui_button_down4, 70);
    lv_obj_set_height(ui_button_down4, 70);
    lv_obj_set_x(ui_button_down4, 145);
    lv_obj_set_y(ui_button_down4, 175);
    lv_obj_set_align(ui_button_down4, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_button_down4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_button_down4, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_button_down4, lv_color_hex(0xEE1E1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_button_down4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_button_down4, lv_color_hex(0xEE1E1E), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_button_down4, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_button_down4, lv_color_hex(0xEE1E1E), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_button_down4, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_button_down_icon4 = lv_img_create(ui_button_down4);
    lv_img_set_src(ui_button_down_icon4, &ui_img_house_png);
    lv_obj_set_width(ui_button_down_icon4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_button_down_icon4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_button_down_icon4, -12);
    lv_obj_set_y(ui_button_down_icon4, -9);
    lv_obj_set_align(ui_button_down_icon4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_button_down_icon4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_button_down_icon4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_button_down4, ui_event_button_down4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_weather_1, ui_event_weather_1, LV_EVENT_ALL, NULL);

}