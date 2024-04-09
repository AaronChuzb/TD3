/*
 * @Date: 2024-04-07 23:43:56
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 12:56:59
 */

#include "StatusBar.h"

// 状态栏对象
lv_obj_t *panel = NULL;

// 状态栏容器的样式
static lv_style_t obj_layout_style;

// 记住wifi的状态，防止页面切换丢失
static char *wifi_status = "";

void status_bar_style_init()
{
  // 初始化状态栏
  lv_style_init(&obj_layout_style);
  lv_style_set_pad_all(&obj_layout_style, 0);
  // lv_style_set_bg_opa(&obj_layout_style, 50);
  lv_style_set_text_font(&obj_layout_style, &lv_font_montserrat_18);
  lv_style_set_border_side(&obj_layout_style, LV_BORDER_SIDE_NONE);
  lv_style_set_radius(&obj_layout_style, 0);
  lv_style_set_text_color(&obj_layout_style, lv_color_hex(0xffffff));
}
// 状态栏动画
static void anim_y_cb(void *var, int32_t v)
{
  lv_obj_set_y((lv_obj_t *)var, v);
}
void status_bar_in(void)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, panel);
  lv_anim_set_values(&a, -25, 0);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_y_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  // 页面切换动画执行完毕之后再执行出现动画
  // 避开卡顿加载动画
  lv_anim_set_delay(&a, 300);
  lv_anim_start(&a);
}

void status_bar_out(void)
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, panel);
  lv_anim_set_values(&a, 0, -25);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_y_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  lv_anim_start(&a);
}

// wifi消息接收回调
static void wifi_msg_event_cb(lv_event_t *e)
{
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);
  switch (lv_msg_get_id(m))
  {
  case MSG_WIFI_IS_CONNECTED:
    wifi_status = LV_SYMBOL_WIFI;
    lv_label_set_text(label, LV_SYMBOL_WIFI);
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

static void time_msg_event_cb(lv_event_t *e)
{
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);
  lv_label_set_text(label, lv_msg_get_payload(m));
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
  lv_obj_set_style_pad_left(panel, 35, 0);
  lv_obj_set_style_pad_right(panel, 35, 0);

  lv_obj_t *label_wifi = lv_label_create(panel);
  lv_label_set_text(label_wifi, wifi_status);
  lv_obj_set_size(label_wifi, 50, 25);
  // 设置消息回调
  lv_obj_add_event_cb(label_wifi, wifi_msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_WIFI_IS_CONNECTED, label_wifi, NULL);
  lv_msg_subsribe_obj(MSG_WIFI_NOT_CONNECTED, label_wifi, NULL);

  lv_obj_t *label_time = lv_label_create(panel);
  lv_label_set_text(label_time, "");

  // 设置消息回调
  lv_obj_add_event_cb(label_time, time_msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_TIME_SET, label_time, NULL);

  lv_obj_t *label_bat_group = lv_label_create(panel);
  lv_label_set_text(label_bat_group, "");
  lv_obj_set_size(label_bat_group, 50, 25);
  lv_obj_center(label_bat_group);
  // lv_obj_set_align(label_bat_group, LV_ALIGN_CENTER);
  lv_obj_t *label_bat = lv_label_create(label_bat_group);

  lv_label_set_text(label_bat, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_center(label_bat);

  lv_obj_t *label_batchar = lv_label_create(label_bat_group);
  lv_label_set_text(label_batchar, LV_SYMBOL_CHARGE);
  lv_obj_center(label_batchar);
}