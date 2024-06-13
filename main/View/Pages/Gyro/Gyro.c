/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-13 11:25:00
 */

#include "Gyro.h"

PageType *Gyro;

static void event_cb(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_current_target(e);
  printf("%s", lv_msgbox_get_active_btn_text(obj));
  lv_msgbox_close(obj);
  Page_Back(1);
}

static void Created()
{

  // static const char *btns[] = {"确定", "返回", ""};

  // lv_obj_t *mbox1 = lv_msgbox_create(NULL, "提示", "功能正在开发中...", btns, false);
  // lv_obj_add_style(mbox1, &font_style_dingdingjinbu_14, 0);
  // lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // lv_obj_center(mbox1);
  lv_obj_t *chart = lv_chart_create(Gyro->PageContent);
  lv_obj_set_width(chart, 250);
  lv_obj_set_height(chart, 200);
  lv_obj_set_x(chart, 10);
  lv_obj_set_y(chart, 35);
  lv_obj_set_align(chart, LV_ALIGN_TOP_MID);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -10, 10);
  lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
  lv_chart_set_div_line_count(chart, 11, 10);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 2, true, 50);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 8, 5, 11, 2, true, 50);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_SECONDARY_Y, 0, 0, 0, 0, false, 25);
  lv_chart_series_t *chart_series_1 = lv_chart_add_series(chart, lv_color_hex(0x808080),
                                                          LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t chart_series_1_array[] = {10, 8, 7, 5, 2, 1, -4, -8, 1, 0};
  lv_chart_set_ext_y_array(chart, chart_series_1, chart_series_1_array);
  lv_chart_series_t *chart_series_2 = lv_chart_add_series(chart, lv_color_hex(0xBD3406),
                                                          LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t chart_series_2_array[] = {-10, -7, 5, 2, 1, -4, 1, 0, 6};
  lv_chart_set_ext_y_array(chart, chart_series_2, chart_series_2_array);
  lv_chart_series_t *chart_series_3 = lv_chart_add_series(chart, lv_color_hex(0x18ACE4),
                                                          LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t chart_series_3_array[] = {0, 10, 2, 4, -8, -8, -4, 2, 10, 0};
  lv_chart_set_ext_y_array(chart, chart_series_3, chart_series_3_array);
  lv_chart_cursor_t *cursor;
  cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_GREEN), LV_DIR_RIGHT);
  lv_point_t point = {0, 100};
  lv_chart_set_cursor_pos(chart, cursor, &point);
}

static void Update(void)
{
}

static void Destroy(void)
{
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