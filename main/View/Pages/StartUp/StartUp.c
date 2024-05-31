/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-05-31 11:39:29
 */

#include "StartUp.h"

struct PageType StartUp;

lv_obj_t *title_left;
lv_obj_t *title_right;
lv_obj_t *sub_title;
lv_obj_t *progress;

typedef struct _ui_anim_user_data_t
{
  lv_obj_t *target;
  lv_img_dsc_t **imgset;
  int32_t imgset_size;
  int32_t val;
} ui_anim_user_data_t;

void _ui_anim_callback_set_opacity(lv_anim_t *a, int32_t v)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  lv_obj_set_style_opa(usr->target, v, 0);
}

void _ui_anim_callback_free_user_data(lv_anim_t *a)
{
  lv_mem_free(a->user_data);
  a->user_data = NULL;
}

int32_t _ui_anim_callback_get_opacity(lv_anim_t *a)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  return lv_obj_get_style_opa(usr->target, 0);
}

void _ui_anim_callback_set_x(lv_anim_t *a, int32_t v)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  lv_obj_set_x(usr->target, v);
}

int32_t _ui_anim_callback_get_x(lv_anim_t *a)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  return lv_obj_get_x_aligned(usr->target);
}

void _ui_anim_callback_set_y(lv_anim_t *a, int32_t v)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  lv_obj_set_y(usr->target, v);
}

int32_t _ui_anim_callback_get_y(lv_anim_t *a)
{
  ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
  return lv_obj_get_y_aligned(usr->target);
}

///////////////////// ANIMATIONS ////////////////////
void left_title_in_Animation(lv_obj_t *TargetObject, int delay)
{
  ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_0_user_data->target = TargetObject;
  PropertyAnimation_0_user_data->val = -1;
  lv_anim_t PropertyAnimation_0;
  lv_anim_init(&PropertyAnimation_0);
  lv_anim_set_time(&PropertyAnimation_0, 500);
  lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
  lv_anim_set_values(&PropertyAnimation_0, 0, 255);
  lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
  lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_0, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
  lv_anim_set_early_apply(&PropertyAnimation_0, false);
  lv_anim_start(&PropertyAnimation_0);
  ui_anim_user_data_t *PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_1_user_data->target = TargetObject;
  PropertyAnimation_1_user_data->val = -1;
  lv_anim_t PropertyAnimation_1;
  lv_anim_init(&PropertyAnimation_1);
  lv_anim_set_time(&PropertyAnimation_1, 500);
  lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_x);
  lv_anim_set_values(&PropertyAnimation_1, -195, -35);
  lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_linear);
  lv_anim_set_delay(&PropertyAnimation_1, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_1, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
  lv_anim_set_early_apply(&PropertyAnimation_1, false);
  lv_anim_start(&PropertyAnimation_1);
}
void right_title_in_Animation(lv_obj_t *TargetObject, int delay)
{
  ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_0_user_data->target = TargetObject;
  PropertyAnimation_0_user_data->val = -1;
  lv_anim_t PropertyAnimation_0;
  lv_anim_init(&PropertyAnimation_0);
  lv_anim_set_time(&PropertyAnimation_0, 500);
  lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
  lv_anim_set_values(&PropertyAnimation_0, 0, 255);
  lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
  lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_0, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
  lv_anim_set_early_apply(&PropertyAnimation_0, false);
  lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
  lv_anim_start(&PropertyAnimation_0);
  ui_anim_user_data_t *PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_1_user_data->target = TargetObject;
  PropertyAnimation_1_user_data->val = -1;
  lv_anim_t PropertyAnimation_1;
  lv_anim_init(&PropertyAnimation_1);
  lv_anim_set_time(&PropertyAnimation_1, 500);
  lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_x);
  lv_anim_set_values(&PropertyAnimation_1, 195, 35);
  lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_linear);
  lv_anim_set_delay(&PropertyAnimation_1, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_1, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
  lv_anim_set_early_apply(&PropertyAnimation_1, false);
  lv_anim_start(&PropertyAnimation_1);
}
void sub_title_in_Animation(lv_obj_t *TargetObject, int delay)
{
  ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_0_user_data->target = TargetObject;
  PropertyAnimation_0_user_data->val = -1;
  lv_anim_t PropertyAnimation_0;
  lv_anim_init(&PropertyAnimation_0);
  lv_anim_set_time(&PropertyAnimation_0, 500);
  lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
  lv_anim_set_values(&PropertyAnimation_0, 0, 255);
  lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_out);
  lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_0, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
  lv_anim_set_early_apply(&PropertyAnimation_0, false);
  lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
  lv_anim_start(&PropertyAnimation_0);
  ui_anim_user_data_t *PropertyAnimation_1_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
  PropertyAnimation_1_user_data->target = TargetObject;
  PropertyAnimation_1_user_data->val = -1;
  lv_anim_t PropertyAnimation_1;
  lv_anim_init(&PropertyAnimation_1);
  lv_anim_set_time(&PropertyAnimation_1, 500);
  lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
  lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_y);
  lv_anim_set_values(&PropertyAnimation_1, 200, 20);
  lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_linear);
  lv_anim_set_delay(&PropertyAnimation_1, delay + 0);
  lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
  lv_anim_set_playback_time(&PropertyAnimation_1, 0);
  lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
  lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
  lv_anim_set_early_apply(&PropertyAnimation_1, false);
  lv_anim_start(&PropertyAnimation_1);
}

void my_timer(lv_timer_t * timer)
{
  Page_Replace("Home");
}


static void Created()
{

  lv_obj_clear_flag(StartUp.PageContent, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  title_left = lv_label_create(StartUp.PageContent);
  lv_obj_set_width(title_left, 60);
  lv_obj_set_height(title_left, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(title_left, -195);
  lv_obj_set_y(title_left, -20);
  lv_obj_set_align(title_left, LV_ALIGN_CENTER);
  lv_label_set_text(title_left, "TD");
  lv_obj_set_style_text_align(title_left, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(title_left, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

  sub_title = lv_label_create(StartUp.PageContent);
  lv_obj_set_width(sub_title, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(sub_title, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(sub_title, 0);
  lv_obj_set_y(sub_title, 200);
  lv_obj_set_align(sub_title, LV_ALIGN_CENTER);
  lv_label_set_text(sub_title, "c o m m u n i t y");

  title_right = lv_label_create(StartUp.PageContent);
  lv_obj_set_width(title_right, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(title_right, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(title_right, 195);
  lv_obj_set_y(title_right, -20);
  lv_obj_set_align(title_right, LV_ALIGN_CENTER);
  lv_label_set_text(title_right, "3");
  lv_obj_set_style_text_font(title_right, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(title_right, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(title_right, lv_color_hex(0xFF9000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(title_right, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(title_right, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(title_right, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(title_right, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(title_right, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  progress = lv_bar_create(StartUp.PageContent);
  lv_obj_set_height(progress, 20);
  lv_obj_set_width(progress, lv_pct(100));
  lv_obj_set_align(progress, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_style_radius(progress, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(progress, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(progress, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_anim_time(progress, 3000, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_radius(progress, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(progress, lv_color_hex(0xFF9000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(progress, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  sub_title_in_Animation(sub_title, 0);
  right_title_in_Animation(title_right, 0);
  left_title_in_Animation(title_left, 0);
  lv_bar_set_value(progress, 100, LV_ANIM_ON);
  lv_timer_t * timer = lv_timer_create(my_timer, 3000,  NULL);
  lv_timer_set_repeat_count(timer, 1);

  // lv_obj_add_event_cb(StartUp.PageContent, ui_event_Screen1, LV_EVENT_ALL, NULL);
}

static void Update(void)
{
}

static void Destroy(void)
{
  if (lv_obj_is_valid(StartUp.PageContent))
  {
    lv_async_call(lv_obj_clean, StartUp.PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void StartUp_Init()
{
  strcpy(StartUp.name, "StartUp");
  StartUp.show_status_bar = 0;
  StartUp.BeforeEnter = NULL;
  StartUp.Created = Created;
  StartUp.Update = Update;
  StartUp.Destroy = Destroy;
  StartUp.Method = Method;
  StartUp.PageContent = create_new_screen();
  Page_Register(StartUp);
}