/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-06 22:42:26
 */

#include "Battery.h"

PageType *Battery;

LV_IMG_DECLARE(ui_img_voltage_png); // assets/voltage.png
LV_IMG_DECLARE(ui_img_current_png); // assets/current.png
LV_IMG_DECLARE(ui_img_charge_png);  // assets/charge.png
LV_IMG_DECLARE(ui_img_coulomb_png); // assets/coulomb.png

LV_FONT_DECLARE(lv_dingdingjinbu_14)

static lv_obj_t *ui_votagebox;
static lv_obj_t *ui_votageicon;
static lv_obj_t *ui_votagetitle;
static lv_obj_t *ui_votagevalue;
static lv_obj_t *ui_currentbox;
static lv_obj_t *ui_currenticon;
static lv_obj_t *ui_currentvalue;
static lv_obj_t *ui_currenttitle;
static lv_obj_t *ui_chargebox;
static lv_obj_t *ui_chargeicon;
static lv_obj_t *ui_chargevalue;
static lv_obj_t *ui_chargetitle;
static lv_obj_t *ui_coulombbox;
static lv_obj_t *ui_coulombicon;
static lv_obj_t *ui_coulombvalue;
static lv_obj_t *ui_coulombtitle;
static lv_obj_t *ui_Panel5;
static lv_obj_t *ui_resetcoulomb;
static lv_obj_t *ui_Label10;
static lv_obj_t *ui_stopcharge;
static lv_obj_t *ui_Label11;
static lv_obj_t *ui_Label12;

static void event_cb(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_current_target(e);
  printf("%s", lv_msgbox_get_active_btn_text(obj));
  lv_msgbox_close(obj);
  Page_Back(1);
}

typedef struct
{
  float battery_voltage;
  float battery_current;
  bool battery_charge;
  float battery_coulomb;
} battery_data_t;

static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_BATTERY_DATA:
    battery_data_t *data = lv_msg_get_payload(m);
    // printf("battery_voltage:%f\n", data->battery_voltage);
    char votage[10];
    sprintf(votage, "%.2fV", data->battery_voltage);
    lv_label_set_text(ui_votagevalue, votage);
    char current[10];
    sprintf(current, "%.2fmA", data->battery_current);
    lv_label_set_text(ui_currentvalue, current);
    char coulomb[10];
    sprintf(coulomb, "%.2fmAh", data->battery_coulomb);
    lv_label_set_text(ui_coulombvalue, coulomb);
    if(data->battery_charge){
      lv_label_set_text(ui_chargevalue, "是");
    } else {
      lv_label_set_text(ui_chargevalue, "否");
    }
    break;
  default:
    break;
  }
}

static void Created()
{
  lv_obj_set_scrollbar_mode(Battery->PageContent, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(Battery->PageContent, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_clip_corner(Battery->PageContent, true, 0);

  // static const char *btns[] = {"确定", "返回", ""};

  // lv_obj_t *mbox1 = lv_msgbox_create(NULL, "提示", "功能正在开发中...", btns, false);
  // lv_obj_add_style(mbox1, &font_style_dingdingjinbu_14, 0);
  // lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // lv_obj_center(mbox1);
  ui_votagebox = lv_obj_create(Battery->PageContent);
  lv_obj_set_width(ui_votagebox, 300);
  lv_obj_set_height(ui_votagebox, 50);
  lv_obj_set_x(ui_votagebox, 0);
  lv_obj_set_y(ui_votagebox, 35);
  lv_obj_set_align(ui_votagebox, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_votagebox, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_border_width(ui_votagebox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_votageicon = lv_img_create(ui_votagebox);
  lv_img_set_src(ui_votageicon, &ui_img_voltage_png);
  lv_obj_set_width(ui_votageicon, LV_SIZE_CONTENT);  /// 32
  lv_obj_set_height(ui_votageicon, LV_SIZE_CONTENT); /// 32
  lv_obj_set_align(ui_votageicon, LV_ALIGN_LEFT_MID);
  lv_obj_add_flag(ui_votageicon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_votageicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_votagetitle = lv_label_create(ui_votagebox);
  lv_obj_set_width(ui_votagetitle, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_votagetitle, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_votagetitle, 40);
  lv_obj_set_y(ui_votagetitle, 0);
  lv_obj_set_align(ui_votagetitle, LV_ALIGN_LEFT_MID);
  lv_label_set_text(ui_votagetitle, "电池电压：");
  lv_obj_set_style_text_font(ui_votagetitle, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_votagevalue = lv_label_create(ui_votagebox);
  lv_obj_set_width(ui_votagevalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_votagevalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_votagevalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_votagevalue, "4.2V");
  lv_obj_set_style_text_font(ui_votagevalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_currentbox = lv_obj_create(Battery->PageContent);
  lv_obj_set_width(ui_currentbox, 300);
  lv_obj_set_height(ui_currentbox, 50);
  lv_obj_set_x(ui_currentbox, 0);
  lv_obj_set_y(ui_currentbox, 95);
  lv_obj_set_align(ui_currentbox, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_currentbox, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_border_width(ui_currentbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_currenticon = lv_img_create(ui_currentbox);
  lv_img_set_src(ui_currenticon, &ui_img_current_png);
  lv_obj_set_width(ui_currenticon, LV_SIZE_CONTENT);  /// 32
  lv_obj_set_height(ui_currenticon, LV_SIZE_CONTENT); /// 32
  lv_obj_set_align(ui_currenticon, LV_ALIGN_LEFT_MID);
  lv_obj_add_flag(ui_currenticon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_currenticon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_currentvalue = lv_label_create(ui_currentbox);
  lv_obj_set_width(ui_currentvalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_currentvalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_currentvalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_currentvalue, "88.2mA");
  lv_obj_set_style_text_font(ui_currentvalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_currenttitle = lv_label_create(ui_currentbox);
  lv_obj_set_width(ui_currenttitle, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_currenttitle, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_currenttitle, 40);
  lv_obj_set_y(ui_currenttitle, 0);
  lv_obj_set_align(ui_currenttitle, LV_ALIGN_LEFT_MID);
  lv_label_set_text(ui_currenttitle, "实时电流：");
  lv_obj_set_style_text_font(ui_currenttitle, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_chargebox = lv_obj_create(Battery->PageContent);
  lv_obj_set_width(ui_chargebox, 300);
  lv_obj_set_height(ui_chargebox, 50);
  lv_obj_set_x(ui_chargebox, 0);
  lv_obj_set_y(ui_chargebox, 155);
  lv_obj_set_align(ui_chargebox, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_chargebox, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_border_width(ui_chargebox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_chargeicon = lv_img_create(ui_chargebox);
  lv_img_set_src(ui_chargeicon, &ui_img_charge_png);
  lv_obj_set_width(ui_chargeicon, LV_SIZE_CONTENT);  /// 32
  lv_obj_set_height(ui_chargeicon, LV_SIZE_CONTENT); /// 32
  lv_obj_set_align(ui_chargeicon, LV_ALIGN_LEFT_MID);
  lv_obj_add_flag(ui_chargeicon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_chargeicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_chargevalue = lv_label_create(ui_chargebox);
  lv_obj_set_width(ui_chargevalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_chargevalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_chargevalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_chargevalue, "是");
  lv_obj_set_style_text_font(ui_chargevalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_chargetitle = lv_label_create(ui_chargebox);
  lv_obj_set_width(ui_chargetitle, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_chargetitle, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_chargetitle, 40);
  lv_obj_set_y(ui_chargetitle, 0);
  lv_obj_set_align(ui_chargetitle, LV_ALIGN_LEFT_MID);
  lv_label_set_text(ui_chargetitle, "是否充电：");
  lv_obj_set_style_text_font(ui_chargetitle, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_coulombbox = lv_obj_create(Battery->PageContent);
  lv_obj_set_width(ui_coulombbox, 300);
  lv_obj_set_height(ui_coulombbox, 50);
  lv_obj_set_x(ui_coulombbox, 0);
  lv_obj_set_y(ui_coulombbox, 215);
  lv_obj_set_align(ui_coulombbox, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ui_coulombbox, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_border_width(ui_coulombbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_coulombicon = lv_img_create(ui_coulombbox);
  lv_img_set_src(ui_coulombicon, &ui_img_coulomb_png);
  lv_obj_set_width(ui_coulombicon, LV_SIZE_CONTENT);  /// 32
  lv_obj_set_height(ui_coulombicon, LV_SIZE_CONTENT); /// 32
  lv_obj_set_align(ui_coulombicon, LV_ALIGN_LEFT_MID);
  lv_obj_add_flag(ui_coulombicon, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  lv_obj_clear_flag(ui_coulombicon, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  ui_coulombvalue = lv_label_create(ui_coulombbox);
  lv_obj_set_width(ui_coulombvalue, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_coulombvalue, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_coulombvalue, LV_ALIGN_RIGHT_MID);
  lv_label_set_text(ui_coulombvalue, "150.2mAh");
  lv_obj_set_style_text_font(ui_coulombvalue, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_coulombtitle = lv_label_create(ui_coulombbox);
  lv_obj_set_width(ui_coulombtitle, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_coulombtitle, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_coulombtitle, 40);
  lv_obj_set_y(ui_coulombtitle, 0);
  lv_obj_set_align(ui_coulombtitle, LV_ALIGN_LEFT_MID);
  lv_label_set_text(ui_coulombtitle, "剩余容量：");
  lv_obj_set_style_text_font(ui_coulombtitle, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Panel5 = lv_obj_create(Battery->PageContent);
  lv_obj_set_width(ui_Panel5, 320);
  lv_obj_set_height(ui_Panel5, 100);
  lv_obj_set_x(ui_Panel5, 0);
  lv_obj_set_y(ui_Panel5, 16);
  lv_obj_set_align(ui_Panel5, LV_ALIGN_BOTTOM_MID);
  lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_Panel5, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_resetcoulomb = lv_btn_create(ui_Panel5);
  lv_obj_set_width(ui_resetcoulomb, 120);
  lv_obj_set_height(ui_resetcoulomb, 50);
  lv_obj_set_x(ui_resetcoulomb, 15);
  lv_obj_set_y(ui_resetcoulomb, -8);
  lv_obj_set_align(ui_resetcoulomb, LV_ALIGN_LEFT_MID);
  lv_obj_add_flag(ui_resetcoulomb, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_resetcoulomb, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

  ui_Label10 = lv_label_create(ui_resetcoulomb);
  lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);
  lv_label_set_text(ui_Label10, "重置库仑计");
  lv_obj_set_style_text_font(ui_Label10, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_stopcharge = lv_btn_create(ui_Panel5);
  lv_obj_set_width(ui_stopcharge, 120);
  lv_obj_set_height(ui_stopcharge, 50);
  lv_obj_set_x(ui_stopcharge, -15);
  lv_obj_set_y(ui_stopcharge, -8);
  lv_obj_set_align(ui_stopcharge, LV_ALIGN_RIGHT_MID);
  lv_obj_add_flag(ui_stopcharge, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
  lv_obj_clear_flag(ui_stopcharge, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

  ui_Label11 = lv_label_create(ui_stopcharge);
  lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
  lv_label_set_text(ui_Label11, "停止充电");
  lv_obj_set_style_text_font(ui_Label11, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_Label12 = lv_label_create(Battery->PageContent);
  lv_obj_set_width(ui_Label12, 310);
  lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_Label12, 0);
  lv_obj_set_y(ui_Label12, 93);
  lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
  lv_label_set_long_mode(ui_Label12, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_label_set_text(ui_Label12, "重置库仑计将会使库仑计数据归零，请在电量即将耗尽时操作。");
  lv_obj_set_style_text_font(ui_Label12, &lv_dingdingjinbu_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_msg_subsribe(MSG_BATTERY_DATA, event_handle_cb, NULL);

  // 发送消息创建获取任务
  lv_msg_send(MSG_BATTERY_GET_DATA_EVENT, NULL);
}

static void Update(void)
{
}

static void Destroy(void)
{
  lv_msg_send(MSG_BATTERY_DESTROY_EVENT, NULL);
  if (lv_obj_is_valid(Battery->PageContent))
  {
    lv_async_call(lv_obj_clean, Battery->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Battery_Init()
{
  Battery = lv_mem_alloc(sizeof(PageType));
  strcpy(Battery->name, "Battery");
  Battery->show_status_bar = 1;
  Battery->BeforeEnter = NULL;
  Battery->Created = Created;
  Battery->Update = Update;
  Battery->Destroy = Destroy;
  Battery->Method = Method;
  Battery->PageContent = create_new_screen();
  Page_Register(*Battery);
}