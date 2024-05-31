/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditClock: 2024-05-30 16:30:08
 */

#include "Clock.h"

struct PageType Clock;

LV_IMG_DECLARE(ui_img_clockwise_sec_png);  // assets/clockwise_sec.png
LV_IMG_DECLARE(ui_img_clockwise_min_png);  // assets/clockwise_min.png
LV_IMG_DECLARE(ui_img_clockwise_hour_png); // assets/clockwise_hour.png

lv_obj_t *ui_sec;
lv_obj_t *ui_min;
lv_obj_t *ui_hour;

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
  } else {
    return 0;
  }
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

static void Created()
{
  ui_sec = lv_img_create(Clock.PageContent);
  lv_img_set_src(ui_sec, &ui_img_clockwise_sec_png);
  lv_obj_set_width(ui_sec, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_sec, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_sec, 0);
  lv_obj_set_y(ui_sec, -67);
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
  lv_obj_set_y(ui_min, -75);
  lv_obj_set_align(ui_min, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_min, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_min, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_pivot(ui_min, 9, 153);

  ui_hour = lv_img_create(Clock.PageContent);
  lv_img_set_src(ui_hour, &ui_img_clockwise_hour_png);
  lv_obj_set_width(ui_hour, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_hour, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_hour, 0);
  lv_obj_set_y(ui_hour, -45);
  lv_obj_set_align(ui_hour, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_hour, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_hour, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_img_set_pivot(ui_hour, 9, 93);
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