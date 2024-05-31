/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditClock: 2024-05-30 16:30:08
 */

#include "Clock.h"
#include <math.h>

struct PageType Clock;

LV_IMG_DECLARE(ui_img_clockwise_sec_png);  // assets/clockwise_sec.png
LV_IMG_DECLARE(ui_img_clockwise_min_png);  // assets/clockwise_min.png
LV_IMG_DECLARE(ui_img_clockwise_hour_png); // assets/clockwise_hour.png

lv_obj_t *ui_sec;
lv_obj_t *ui_min;
lv_obj_t *ui_hour;

struct tm timechip;

typedef struct _ui_anim_user_data_t
{
  lv_obj_t *target;
  lv_img_dsc_t **imgset;
  int32_t imgset_size;
  int32_t val;
} ui_anim_user_data_t;

void _ui_anim_callback_set_image_angle(lv_anim_t *a, int32_t v)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  if (lv_obj_is_valid(usr->target))
  {
    lv_img_set_angle(usr->target, v);
  }
}

void ui_anim_callback_free_user_data(lv_anim_t *a)
{
  lv_mem_free(a->user_data);
  a->user_data = NULL;
}

int32_t _ui_anim_callback_get_image_angle(lv_anim_t *a)

{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  if (lv_obj_is_valid(usr->target))
  {
    return lv_img_get_angle(usr->target);
  }
  else
  {
    return 0;
  }
}

static void convert_time_to_angle(int hours, int minutes, int seconds)
{
  // 计算时针角度
  int hour_angle = (hours % 12) * 300 + (minutes * 300 / 60);
  lv_img_set_angle(ui_hour, hour_angle);

  // 计算分针角度
  int minute_angle = minutes * 60 + (seconds * 60 / 60);
  lv_img_set_angle(ui_min, minute_angle);

  // 计算秒针角度
  int second_angle = seconds * 60;
  lv_img_set_angle(ui_sec, second_angle);
}

void time_Animation(lv_obj_t *TargetObject, int delay, long duration, int value)
{
  ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_0_user_data->target = TargetObject;
  PropertyAnimation_0_user_data->val = -1;
  lv_anim_t PropertyAnimation_0;
  lv_anim_init(&PropertyAnimation_0);
  lv_anim_set_time(&PropertyAnimation_0, duration);
  lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
  lv_anim_set_values(&PropertyAnimation_0, 0, value);
  lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
  lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_0, ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_0, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
  lv_anim_set_early_apply(&PropertyAnimation_0, false);
  lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle);
  lv_anim_start(&PropertyAnimation_0);
}

static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);

  switch (lv_msg_get_id(m))
  {
  case MSG_SET_TIME:
    struct tm *data = lv_msg_get_payload(m);
    timechip = *data;
    char time_str[30];
    sprintf(time_str, "%d-%d-%d %d:%d:%d", timechip.tm_year, timechip.tm_mon, timechip.tm_mday, timechip.tm_hour, timechip.tm_min, timechip.tm_sec);
    convert_time_to_angle(timechip.tm_hour, timechip.tm_min, timechip.tm_sec);
    printf("The current date/time in pcf8563 is: %s \n", time_str);
    break;
  default:
    break;
  }
}

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 386
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2) + 10
#define RADIUS 150
#define NUMBERS 12
void create_clock_labels(lv_obj_t * parent) {
    for (int i = 1; i <= NUMBERS; i++) {
        // 计算数字角度
        float angle = (360.0 / NUMBERS) * i;
        // 角度转换为弧度
        float radian = (angle - 90) * M_PI / 180.0;

        // 计算数字位置
        int num_x = CENTER_X + (int)(RADIUS * cos(radian));
        int num_y = CENTER_Y + (int)(RADIUS * sin(radian));

        // 创建标签
        lv_obj_t * label = lv_label_create(parent);

        // 设置标签文本
        char num_str[3];
        snprintf(num_str, sizeof(num_str), "%d", i);
        lv_label_set_text(label, num_str);

        // 设置标签位置
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);  // 确保文本居中对齐
        lv_obj_align(label, LV_ALIGN_CENTER, num_x - CENTER_X, num_y - CENTER_Y);
    }
}

void create_clock_labels_and_dots(lv_obj_t * parent) {
    for (int i = 1; i <= NUMBERS; i++) {
        // 计算角度
        float angle = (360.0 / NUMBERS) * i;
        // 角度转换为弧度
        float radian = (angle - 90) * M_PI / 180.0;

        // 计算位置
        int pos_x = CENTER_X + (int)(RADIUS * cos(radian));
        int pos_y = CENTER_Y + (int)(RADIUS * sin(radian));

        // 创建标签或点
        if (i == 3 || i == 6 || i == 9 || i == 12) {
            // 创建标签
            lv_obj_t * label = lv_label_create(parent);
            lv_obj_add_style(label, &font_style_youshebiaotihei_24, 0);

            // 设置标签文本
            char num_str[3];
            snprintf(num_str, sizeof(num_str), "%d", i == 12 ? 12 : i);
            lv_label_set_text(label, num_str);

            // 设置标签位置
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);  // 确保文本居中对齐
            lv_obj_align(label, LV_ALIGN_CENTER, pos_x - CENTER_X, pos_y - CENTER_Y);
        } else {
            // 创建点（小圆点）
            lv_obj_t * dot = lv_obj_create(parent);
            lv_obj_set_size(dot, 5, 5); // 设置点的大小
            lv_obj_set_style_radius(dot, 5, 0); // 使点为圆形
            lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_RED), 0); // 设置点的颜色

            // 设置点的位置
            lv_obj_align(dot, LV_ALIGN_CENTER, pos_x - CENTER_X, pos_y - CENTER_Y);
        }
    }
}


static void Created()
{

  create_clock_labels_and_dots(Clock.PageContent);
  ui_sec = lv_img_create(Clock.PageContent);
  lv_img_set_src(ui_sec, &ui_img_clockwise_sec_png);
  lv_obj_set_width(ui_sec, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_sec, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_sec, 0);
  lv_obj_set_y(ui_sec, -47);
  lv_obj_set_align(ui_sec, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_sec, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_sec, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_pivot(ui_sec, 15, 155);
  lv_img_set_angle(ui_sec, 0);

  ui_min = lv_img_create(Clock.PageContent);
  lv_img_set_src(ui_min, &ui_img_clockwise_min_png);
  lv_obj_set_width(ui_min, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_min, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_min, 0);
  lv_obj_set_y(ui_min, -55);
  lv_obj_set_align(ui_min, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_min, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_min, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_pivot(ui_min, 9, 153);

  ui_hour = lv_img_create(Clock.PageContent);
  lv_img_set_src(ui_hour, &ui_img_clockwise_hour_png);
  lv_obj_set_width(ui_hour, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_hour, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_hour, 0);
  lv_obj_set_y(ui_hour, -25);
  lv_obj_set_align(ui_hour, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_hour, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_hour, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_pivot(ui_hour, 9, 93);
  // 获取一遍时间
  lv_msg_subsribe(MSG_SET_TIME, event_handle_cb, NULL);
  lv_msg_send(MSG_GET_TIME, NULL);
  // sec_Animation(ui_sec, 0);
  time_Animation(ui_sec, 0, 60000, 3600);
  time_Animation(ui_min, 0, 60000 * 60, 3600);
  time_Animation(ui_hour, 0, 60000 * 60 * 12, 3600);
}

static void Update(void)
{
}

static void Destroy(void)
{
  // lv_anim_del(&PropertyAnimation_0, NULL);
  if (lv_obj_is_valid(Clock.PageContent))
  {
    lv_async_call(lv_obj_clean, Clock.PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Clock_Init()
{
  strcpy(Clock.name, "Clock");
  Clock.show_status_bar = 1;
  Clock.BeforeEnter = NULL;
  Clock.Created = Created;
  Clock.Update = Update;
  Clock.Destroy = Destroy;
  Clock.Method = Method;
  Clock.PageContent = create_new_screen();
  Page_Register(Clock);
}