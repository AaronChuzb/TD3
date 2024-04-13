/*
 * @Date: 2024-04-07 23:43:56
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-13 16:25:57
 */

#include "StatusBar.h"

// 状态栏对象
lv_obj_t *panel = NULL;
lv_obj_t *label_wifi = NULL;
lv_obj_t *label_time = NULL;
lv_obj_t *label_batchar = NULL;
lv_obj_t *label_sdcard = NULL;
lv_obj_t *label_level = NULL;
// 状态栏动画
lv_anim_t panel_in;
lv_anim_t panel_out;
lv_anim_t charge_opacity;

// 状态栏容器的样式
static lv_style_t obj_layout_style;

// 记住wifi的状态，防止页面切换丢失
static char *wifi_status = "";
// 记住sd卡的状态，防止页面切换丢失
static char *sdcard_status = "";

// 记住时间状态
static char *time_str = "00:00";

// 记住电量状态
static char *battery_str = LV_SYMBOL_BATTERY_EMPTY;

void status_bar_style_init()
{
  // 初始化状态栏
  lv_style_init(&obj_layout_style);
  lv_style_set_pad_all(&obj_layout_style, 0);
  lv_style_set_text_font(&obj_layout_style, &lv_font_montserrat_18);
  lv_style_set_border_side(&obj_layout_style, LV_BORDER_SIDE_NONE);
  lv_style_set_radius(&obj_layout_style, 0);
  lv_style_set_text_color(&obj_layout_style, lv_color_hex(0xffffff));
}

// Y轴运动
static void anim_y_cb(void *var, int32_t v)
{
  lv_obj_set_y((lv_obj_t *)var, v);
}

void anim_opacity_cb(void *var, int32_t v)
{
  lv_obj_set_style_opa((lv_obj_t *)var, v, 0);
}

static void anim_finish_cb(lv_anim_t *a)
{
  lv_anim_del(a->var, NULL); // 删除动画(官方文档并没有说会不会释放内存)
}

void status_bar_in(void)
{
  lv_anim_init(&panel_in);
  lv_anim_set_ready_cb(&panel_in, anim_finish_cb);
  lv_anim_set_var(&panel_in, panel);
  lv_anim_set_values(&panel_in, -25, 0);
  lv_anim_set_time(&panel_in, 150);
  lv_anim_set_exec_cb(&panel_in, anim_y_cb);
  lv_anim_set_path_cb(&panel_in, lv_anim_path_linear);
  lv_anim_set_delay(&panel_in, 300);
  lv_anim_start(&panel_in);
}

void status_bar_out(void)
{
  // 清除订阅
  lv_msg_unsubscribe_obj(MSG_WIFI_IS_CONNECTED, label_wifi);
  lv_msg_unsubscribe_obj(MSG_WIFI_NOT_CONNECTED, label_wifi);
  lv_msg_unsubscribe_obj(MSG_TIME_SET, label_time);
  lv_msg_unsubscribe_obj(MSG_SDCARD_MOUNT, label_sdcard);
  lv_msg_unsubscribe_obj(MSG_CHARGE_SET, label_batchar);
  lv_msg_unsubscribe_obj(MSG_BAT_SET, label_level);
  lv_anim_init(&panel_out);
  lv_anim_set_ready_cb(&panel_out, anim_finish_cb);
  lv_anim_set_var(&panel_out, panel);
  lv_anim_set_values(&panel_out, -25, 0);
  lv_anim_set_time(&panel_out, 150);
  lv_anim_set_exec_cb(&panel_out, anim_y_cb);
  lv_anim_set_path_cb(&panel_out, lv_anim_path_linear);
  lv_anim_set_delay(&panel_out, 300);
  lv_anim_start(&panel_out);
  // lv_anim_del(&charge_opacity, NULL);
}

static void msg_event_cb(lv_event_t *e)
{
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);
  switch (lv_msg_get_id(m))
  {
  case MSG_SDCARD_MOUNT:
    sdcard_status = lv_msg_get_payload(m);
    lv_label_set_text(label, sdcard_status);
    break;
  case MSG_CHARGE_SET:
    lv_label_set_text(label, lv_msg_get_payload(m));
    break;
  case MSG_BAT_SET:
    battery_str = lv_msg_get_payload(m);
    lv_label_set_text(label, battery_str);
    break;
  case MSG_TIME_SET:
    time_str = lv_msg_get_payload(m);
    lv_label_set_text(label, time_str);
    break;
  case MSG_WIFI_IS_CONNECTED:
    wifi_status = LV_SYMBOL_WIFI;
    lv_label_set_text(label, LV_SYMBOL_WIFI);
    lv_obj_set_size(label_wifi, 30, 20);
    break;
  case MSG_WIFI_NOT_CONNECTED:
    wifi_status = "";
    lv_label_set_text(label, "");
    // lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREEN), 0);
    break;
  default:
    break;
  }
}

// 页面状态栏创建
void status_bar_init(lv_obj_t *pageContent)
{

  panel = lv_obj_create(pageContent);
  lv_obj_set_size(panel, LV_HOR_RES, 25);
  lv_obj_add_style(panel, &obj_layout_style, 0);
  lv_obj_set_pos(panel, 0, -25);
  lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_left(panel, 30, 0);
  lv_obj_set_style_pad_right(panel, 30, 0);

  // 硬件模块
  lv_obj_t *label_wifi_group = lv_label_create(panel);
  lv_label_set_text(label_wifi_group, "");
  lv_obj_set_size(label_wifi_group, 50, 25);
  lv_obj_set_flex_flow(label_wifi_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(label_wifi_group, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  // wifi
  label_wifi = lv_label_create(label_wifi_group);
  lv_label_set_text(label_wifi, wifi_status);
  if (strcmp(wifi_status, "") == 0)
  {
    lv_obj_set_size(label_wifi, 0, 0);
  }
  else
  {
    lv_obj_set_size(label_wifi, 30, 20);
  }
  // lv_obj_set_flex_grow(label_wifi, 2);
  // 设置消息回调
  lv_obj_add_event_cb(label_wifi, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_WIFI_IS_CONNECTED, label_wifi, NULL);
  lv_msg_subsribe_obj(MSG_WIFI_NOT_CONNECTED, label_wifi, NULL);
  // SD卡
  label_sdcard = lv_label_create(label_wifi_group);
  lv_label_set_text(label_sdcard, sdcard_status);
  // lv_obj_set_flex_grow(label_sdcard, 2);
  // 设置消息回调
  lv_obj_add_event_cb(label_sdcard, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_SDCARD_MOUNT, label_sdcard, NULL);
  

  // 时间模块
  label_time = lv_label_create(panel);
  lv_label_set_text(label_time, time_str);
  // 设置消息回调
  lv_obj_add_event_cb(label_time, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_TIME_SET, label_time, NULL);
  

  // 电量模块
  lv_obj_t *label_bat_group = lv_label_create(panel);
  lv_label_set_text(label_bat_group, "");
  lv_obj_set_size(label_bat_group, 50, 25);
  lv_obj_center(label_bat_group);
  lv_obj_set_layout(label_bat_group, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(label_bat_group, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(label_bat_group, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  // lv_obj_set_align(label_bat_group, LV_ALIGN_CENTER);



  label_batchar = lv_label_create(label_bat_group);
 
  lv_label_set_text(label_batchar, "");
  // 设置消息回调
  lv_obj_add_event_cb(label_batchar, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_CHARGE_SET, label_batchar, NULL);
  //  lv_obj_set_align(label_batchar, LV_ALIGN_CENTER);
  // 设置充电图标的闪烁动画

  // lv_anim_init(&charge_opacity);
  // lv_anim_set_time(&charge_opacity, 250);
  // lv_anim_set_var(&charge_opacity, label_batchar);
  // lv_anim_set_exec_cb(&charge_opacity, anim_opacity_cb);
  // lv_anim_set_values(&charge_opacity, 0, 255);
  // lv_anim_set_path_cb(&charge_opacity, lv_anim_path_linear);
  // lv_anim_set_delay(&charge_opacity, 0);
  // lv_anim_set_playback_time(&charge_opacity, 250);
  // lv_anim_set_playback_delay(&charge_opacity, 250);
  // lv_anim_set_repeat_count(&charge_opacity, LV_ANIM_REPEAT_INFINITE);
  // lv_anim_set_repeat_delay(&charge_opacity, 500);
  // lv_anim_set_early_apply(&charge_opacity, true);
  // lv_anim_start(&charge_opacity);


  label_level = lv_label_create(label_bat_group);
  lv_label_set_text(label_level, battery_str);

  // 设置消息回调
  lv_obj_add_event_cb(label_level, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_BAT_SET, label_level, NULL);

  
}
