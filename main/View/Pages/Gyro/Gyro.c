/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-17 16:58:39
 */

#include "Gyro.h"

PageType *Gyro;

static lv_obj_t *chart;

static lv_chart_series_t *chart_series_2;
static lv_chart_series_t *chart_series_3;

LV_FONT_DECLARE(lv_dingdingjinbu_14)

static lv_obj_t *ui_tipbox;
static lv_obj_t *ui_pitch;
static lv_obj_t *ui_pitchicon;
static lv_obj_t *ui_pitchtext;
static lv_obj_t *ui_pitchvalue;
static lv_obj_t *ui_roll;
static lv_obj_t *ui_rollicon;
static lv_obj_t *ui_rolltext;
static lv_obj_t *ui_rollvalue;


char pitch[10];
char roll[10];


static void event_cb(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_current_target(e);
  printf("%s", lv_msgbox_get_active_btn_text(obj));
  lv_msgbox_close(obj);
  Page_Back(1);
}

typedef struct
{
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float mx;
  float my;
  float mz;
  float temp;
  float roll;
  float pitch;
  float yaw;
} gyro_data_view_t;

static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_GYRO_DATA:
    gyro_data_view_t *data = lv_msg_get_payload(m);
    lv_chart_set_next_value(chart, chart_series_2, data->pitch);
    lv_chart_set_next_value(chart, chart_series_3, data->roll);

    sprintf(pitch, "%.0f", getMin(data->pitch, 180));
    sprintf(roll, "%.0f", getMin(data->roll, 180));

    lv_label_set_text(ui_pitchvalue, pitch);
    lv_label_set_text(ui_rollvalue, roll);
    break;
  }
}

static void Created()
{

  ui_tipbox = lv_obj_create(Gyro->PageContent);
  lv_obj_set_height(ui_tipbox, 110);
  lv_obj_set_width(ui_tipbox, lv_pct(95));
  lv_obj_set_x(ui_tipbox, 0);
  lv_obj_set_y(ui_tipbox, 35);
  lv_obj_set_align(ui_tipbox, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_tipbox, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_pitch = lv_obj_create(ui_tipbox);
  lv_obj_remove_style_all(ui_pitch);
  lv_obj_set_width(ui_pitch, lv_pct(50));
  lv_obj_set_height(ui_pitch, 30);
  lv_obj_align(ui_pitch, LV_ALIGN_LEFT_MID, 0, -30);
  lv_obj_clear_flag(ui_pitch, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_pitchicon = lv_obj_create(ui_pitch);
  lv_obj_set_width(ui_pitchicon, 20);
  lv_obj_set_height(ui_pitchicon, 20);
  lv_obj_set_align(ui_pitchicon, LV_ALIGN_LEFT_MID);
  lv_obj_clear_flag(ui_pitchicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_pitchicon, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_pitchicon, lv_color_hex(0xBD3406), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_pitchicon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_pitchtext = lv_label_create(ui_pitch);
  lv_obj_set_width(ui_pitchtext, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_pitchtext, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_pitchtext, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(ui_pitchtext, 25);
  lv_label_set_text(ui_pitchtext, "仰俯");
  lv_obj_set_style_text_font(ui_pitchtext, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_pitchvalue = lv_label_create(ui_pitch);
  lv_obj_set_width(ui_pitchvalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_pitchvalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_pitchvalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_pitchvalue, "100");
  lv_obj_set_style_text_font(ui_pitchvalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_roll = lv_obj_create(ui_tipbox);
  lv_obj_remove_style_all(ui_roll);
  lv_obj_set_width(ui_roll, lv_pct(50));
  lv_obj_set_height(ui_roll, 30);
  lv_obj_clear_flag(ui_roll, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_align(ui_roll, LV_ALIGN_LEFT_MID, 0, 0);

  ui_rollicon = lv_obj_create(ui_roll);
  lv_obj_set_width(ui_rollicon, 20);
  lv_obj_set_height(ui_rollicon, 20);
  lv_obj_set_align(ui_rollicon, LV_ALIGN_LEFT_MID);
  lv_obj_clear_flag(ui_rollicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_rollicon, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_rollicon, lv_color_hex(0x18ACE4), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_rollicon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_rolltext = lv_label_create(ui_roll);
  lv_obj_set_width(ui_rolltext, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_rolltext, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_rolltext, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(ui_rolltext, 25);
  lv_label_set_text(ui_rolltext, "翻滚");
  lv_obj_set_style_text_font(ui_rolltext, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_rollvalue = lv_label_create(ui_roll);
  lv_obj_set_width(ui_rollvalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_rollvalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_rollvalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_rollvalue, "100");
  lv_obj_set_style_text_font(ui_rollvalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);


  chart = lv_chart_create(Gyro->PageContent);
  lv_obj_set_width(chart, 250);
  lv_obj_set_height(chart, 200);
  lv_obj_set_x(chart, 10);
  lv_obj_set_y(chart, 155);
  lv_obj_set_align(chart, LV_ALIGN_TOP_MID);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -180, 180);
  lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
  lv_chart_set_div_line_count(chart, 11, 10);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 2, true, 50);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 8, 5, 11, 2, true, 50);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 25);

  chart_series_2 = lv_chart_add_series(chart, lv_color_hex(0xBD3406),
                                       LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t chart_series_2_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  lv_chart_set_ext_y_array(chart, chart_series_2, chart_series_2_array);
  chart_series_3 = lv_chart_add_series(chart, lv_color_hex(0x18ACE4),
                                       LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t chart_series_3_array[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  lv_chart_set_ext_y_array(chart, chart_series_3, chart_series_3_array);
  lv_chart_cursor_t *cursor;
  cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_GREEN), LV_DIR_RIGHT);
  lv_point_t point = {0, 100};
  lv_chart_set_cursor_pos(chart, cursor, &point);
  lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_msg_subsribe(MSG_GYRO_DATA, event_handle_cb, NULL);

  // 发送消息创建获取任务
  // printf("Gyro init\n");
  lv_msg_send(MSG_GYRO_GET_DATA_EVENT, NULL);
}

static void Update(void)
{
}

static void Destroy(void)
{
  lv_msg_send(MSG_GYRO_DESTROY_EVENT, NULL);
  if (lv_obj_is_valid(Gyro->PageContent))
  {
    lv_async_call(lv_obj_clean, Gyro->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Gyro_Init()
{
  Gyro = lv_mem_alloc(sizeof(PageType));
  strcpy(Gyro->name, "Gyro");
  Gyro->show_status_bar = 1;
  Gyro->BeforeEnter = NULL;
  Gyro->Created = Created;
  Gyro->Update = Update;
  Gyro->Destroy = Destroy;
  Gyro->Method = Method;
  Gyro->PageContent = create_new_screen();
  Page_Register(*Gyro);
}