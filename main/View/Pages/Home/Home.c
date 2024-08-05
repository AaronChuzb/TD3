/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-07-24 09:24:55
 */

#include "Home.h"
#include "Fonts.h"
#include <stdlib.h>

// 声明页面结构体

PageType *Home;

lv_obj_t *obj1;
lv_obj_t *obj5;

int current_index = 5;

lv_obj_t *title_roller;
static lv_obj_t *sub_title;

LV_IMG_DECLARE(ui_img_battery_png);
LV_IMG_DECLARE(ui_img_gyro_png);
LV_IMG_DECLARE(ui_img_press_png);
LV_IMG_DECLARE(ui_img_sdcard_png);
LV_IMG_DECLARE(ui_img_setting_png);
LV_IMG_DECLARE(ui_img_clock_png);
LV_IMG_DECLARE(ui_img_net_png);
LV_IMG_DECLARE(ui_img_weather_png);
LV_IMG_DECLARE(ui_img_music_png);
LV_IMG_DECLARE(ui_img_serial_png);
LV_FONT_DECLARE(lv_youshebiaotihei_24)

struct ButtonData
{
  char *title;
  char *sub_title;
  lv_img_dsc_t *img;
  char *path;
};

const struct ButtonData button_data[10] = {
    {"电池信息", "查看来自AXP173电源管理芯片的数据,包括电压电流库仑计数据等.\n可以手动重置库仑计数据.", &ui_img_battery_png, "Battery"},
    {"陀螺仪", "查看陀螺仪数据", &ui_img_gyro_png, "Gyro"},
    {"气压计", "查看来自BMP280的大气压数据,和相对高度的变化.", &ui_img_press_png, "Pressure"},
    {"储存卡", "查看内存卡数据文件列表.", &ui_img_sdcard_png, "Files"},
    {"实时时钟", "查看当前时间,同步网络时间.\n设定定时任务,如番茄时钟等.", &ui_img_clock_png, "Clock"},
    {"系统设置", "查看系统版本,设置亮度,设置背光时间,设置天气城市,设置天气更新频率.", &ui_img_setting_png, "Setting"},
    {"网络信息", "查看联网状态和WiFi配置信息", &ui_img_net_png, "Network"},
    {"天气查询", "查看当前设定的城市的天气.", &ui_img_weather_png, "Weather"},
    {"MIDI音乐", "播放mid格式的音乐...\n功能开发中", &ui_img_music_png, ""},
    {"串口数据", "查看上位机发送的串口数据", &ui_img_serial_png, "Terminal"},
};

static void boxarea_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    printf("%s", button_data[current_index].path);
    // Page_Back(1);
    if (strcmp(button_data[current_index].path, "") != 0)
    {
      Page_Push(button_data[current_index].path);
    }
  }
}

static void scroll_event_cb(lv_event_t *e)
{
  lv_obj_t *cont = lv_event_get_target(e);
  lv_area_t cont_a;
  lv_obj_get_coords(cont, &cont_a);
  // 滚动区域y轴中点
  lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;
  lv_coord_t r = lv_obj_get_width(cont) * 7 / 20;
  // 滚动子元素数量
  uint32_t child_cnt = lv_obj_get_child_cnt(cont);

  for (uint32_t i = 0; i < child_cnt; i++)
  {
    lv_obj_t *child = lv_obj_get_child(cont, i);
    lv_obj_t *img = lv_obj_get_child(child, 1);
    lv_area_t child_a;
    lv_obj_get_coords(child, &child_a);
    lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;
    lv_coord_t diff_y = child_y_center - cont_y_center;
    // 距离中心点位置
    diff_y = LV_ABS(diff_y);
    // 离得越进 缩放越明显
    lv_coord_t zoom = 325 - diff_y;
    // printf("zoom: %d\n", zoom);
    if (zoom <= 256)
    {
      zoom = 256;
    }

    // uint32_t x_sqr = r * r - diff_y * diff_y;
    // lv_sqrt_res_t res;
    // lv_sqrt(x_sqr, &res, 0x8000);
    // zoom = r - res.i;
    lv_img_set_zoom(img, zoom);
    // lv_obj_set_style_translate_x(child, x, 0);
    // lv_obj_t *img = lv_obj_get_child(btn, 1);
    // lv_img_set_zoom(img, 300);

    /*Use some opacity with larger translations*/
    // lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
    // lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
  }
}

/**
 * @brief 滚动结束后的事件
 * @param event
 */
static void scroll_end_event(lv_event_t *e)
{
  lv_obj_t *cont = lv_event_get_target(e); // 获取事件的初始对象

  /* 获取事件的事件代码 */
  if (lv_event_get_code(e) == LV_EVENT_SCROLL_END)
  {

    /* 判断是否在滚动中 */
    if (lv_obj_is_scrolling(cont))
    {
      // printf("在滚动中\n");
      return;
    }

    lv_coord_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    lv_coord_t mid_btn_index = (child_cnt - 1) / 2;    // 中间界面的位置
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
      lv_obj_t *child = lv_obj_get_child(cont, i);
      lv_area_t child_a;
      lv_obj_get_coords(child, &child_a);
      lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;
      if (child_y_center == cont_y_center)
      {
        lv_coord_t current_btn_index = lv_obj_get_index(child);
        lv_coord_t move_btn_quantity = LV_ABS(current_btn_index - mid_btn_index);
        for (lv_coord_t j = 0; j < move_btn_quantity; j++)
        {
          /* 向上滑动 */
          if (current_btn_index < mid_btn_index)
          {
            lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0);
          }
          /* 向下滑动 */
          if (current_btn_index > mid_btn_index)
          {
            lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1);
          }
        }
        lv_obj_t *btn = lv_obj_get_child(cont, mid_btn_index);
        lv_obj_t *text = lv_obj_get_child(btn, 0);
        char *index_str = lv_label_get_text(text);
        current_index = atoi(index_str);
        lv_label_set_text(sub_title, button_data[current_index].sub_title);
        lv_roller_set_selected(title_roller, current_index, LV_ANIM_ON);
        lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF);

        break;
      }
    }
  }
}

void home_menu()
{
  lv_obj_t *cont = lv_obj_create(Home->PageContent);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(cont, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(cont, LV_HOR_RES / 2, LV_VER_RES - 25);
  lv_obj_align(cont, LV_ALIGN_TOP_RIGHT, 0, 25);

  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

  lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL); // 为了方便实现，先把滚动的动画屏蔽
  lv_obj_add_event_cb(cont, scroll_end_event, LV_EVENT_SCROLL_END, NULL);

  lv_obj_set_style_clip_corner(cont, true, 0);
  lv_obj_set_scroll_dir(cont, LV_DIR_VER);
  lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t *box = lv_btn_create(Home->PageContent);
  lv_obj_align(box, LV_ALIGN_LEFT_MID, -LV_HOR_RES / 2, 12);
  lv_obj_set_size(box, LV_HOR_RES, LV_HOR_RES);
  lv_obj_set_style_radius(box, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(box, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(box, boxarea_event_cb, LV_EVENT_ALL, NULL);

  title_roller = lv_roller_create(box);
  lv_obj_set_style_bg_opa(title_roller, LV_OPA_TRANSP, LV_PART_SELECTED);
  lv_obj_set_style_pad_all(title_roller, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(title_roller, 0, LV_PART_MAIN);
  lv_obj_set_style_text_color(title_roller, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(title_roller, &lv_youshebiaotihei_24, LV_PART_SELECTED);
  lv_obj_clear_flag(title_roller, LV_OBJ_FLAG_SCROLLABLE);

  lv_roller_set_options(title_roller,
                        "电池信息\n"
                        "陀螺仪\n"
                        "气压计\n"
                        "储存卡\n"
                        "实时时钟\n"
                        "系统设置\n"
                        "网络配置\n"
                        "天气查询\n"
                        "MIDI音乐\n"
                        "串口数据",
                        LV_ROLLER_MODE_INFINITE);

  // lv_obj_center(title_roller);
  lv_obj_align(title_roller, LV_ALIGN_TOP_RIGHT, -5, 20);
  lv_roller_set_visible_row_count(title_roller, 1);

  sub_title = lv_label_create(box);
  lv_obj_add_style(sub_title, &font_style_dingdingjinbu_14, LV_PART_MAIN);
  lv_obj_set_size(sub_title, LV_HOR_RES / 2 - 30, LV_HOR_RES - 70);
  lv_label_set_text(sub_title, "");
  lv_label_set_long_mode(sub_title, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_color(sub_title, lv_color_hex(0xabb2bf), LV_PART_MAIN);
  lv_obj_align(sub_title, LV_ALIGN_TOP_RIGHT, 5, 60);

  uint32_t i;
  for (i = 0; i < sizeof(button_data) / sizeof(button_data[0]); i++)
  {
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_width(btn, lv_pct(100));
    lv_obj_set_height(btn, lv_pct(40));
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    char index[5];
    itoa(i, index, 10);
    // printf("%s\n", index);
    lv_obj_t *labelopa = lv_label_create(btn);
    lv_label_set_text(labelopa, index);
    lv_obj_add_flag(labelopa, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *img = lv_img_create(btn);
    lv_img_set_src(img, button_data[i].img);
    lv_obj_set_width(img, LV_SIZE_CONTENT);  /// 64
    lv_obj_set_height(img, LV_SIZE_CONTENT); /// 64
    lv_obj_set_align(img, LV_ALIGN_CENTER);
    // lv_obj_align(img, LV_ALIGN_LEFT_MID, 0, 0);
  }

  lv_coord_t mid_btn_index = (lv_obj_get_child_cnt(cont) - 1) / 2;
  lv_coord_t child_cnt = lv_obj_get_child_cnt(cont);

  int roll_direction = mid_btn_index - mid_btn_index;
  for (lv_coord_t i = 0; i < LV_ABS(roll_direction); i++)
  {
    if (roll_direction > 0)
    {
      lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0);
    }
    else
    {
      lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1);
    }
  }
  lv_obj_scroll_to_view(lv_obj_get_child(cont, current_index), LV_ANIM_ON);
  lv_roller_set_selected(title_roller, current_index, LV_ANIM_ON);
}

static void Created()
{
  lv_obj_set_scrollbar_mode(Home->PageContent, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(Home->PageContent, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_clip_corner(Home->PageContent, true, 0);
  home_menu();

  // lv_example_roller_30();

  // lv_obj_t *arc = lv_arc_create(Home->PageContent);
  // lv_obj_set_size(arc, 150, 150);
  // lv_arc_set_rotation(arc, 200);
  // lv_arc_set_bg_angles(arc, 0, 45);
  // lv_arc_set_value(arc, 40);
  // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  // lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  // lv_obj_set_pos(arc, 10, 25);
  // lv_obj_center(arc);
  // lv_obj_t *page_bg = lv_img_create(Home->PageContent);
  // lv_img_set_src(page_bg, &background);
  // lv_obj_set_width(page_bg, LV_SIZE_CONTENT);  /// 1
  // lv_obj_set_height(page_bg, LV_SIZE_CONTENT); /// 1
  // lv_obj_set_align(page_bg, LV_ALIGN_CENTER);
  // lv_obj_add_flag(page_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  // lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);                        /// Flags
  // lv_obj_set_y(page_bg, -30);
  // lv_img_set_zoom(page_bg, 155);

  // obj1 = lv_obj_create(Home->PageContent);
  // lv_obj_set_size(obj1, 100, 150);
  // lv_obj_set_pos(obj1, 5, -150);
  // lv_obj_add_event_cb(obj1, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  // obj5 = lv_obj_create(Home->PageContent);
  // lv_obj_set_size(obj5, 100, 150);
  // lv_obj_set_pos(obj5, 200, 35);
  // lv_obj_add_event_cb(obj5, event_btn2_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  // lv_obj_t *label = lv_label_create(Home->PageContent);
  // lv_label_set_text(label, time_str);
  // lv_obj_add_style(label, &font_style_gigi_72, 0);
  // lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -50);

  // // 设置消息回调
  // lv_obj_add_event_cb(label, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // // 订阅消息
  // lv_msg_subsribe_obj(MSG_TIME_SET, label, NULL);

  // button_anim_in();

  //   lv_obj_t* img;

  // img = lv_img_create(Home->PageContent);
  //   /* Assuming a File system is attached to letter 'A'
  //    * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
  //   lv_img_set_src(img, "S:/png/1.png");
  //   lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
}

static void Update(void)
{
}

static void Destroy(void)
{
  // ;
  if (lv_obj_is_valid(Home->PageContent))
  {
    lv_async_call(lv_obj_clean, Home->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Home_Init()
{
  Home = lv_mem_alloc(sizeof(PageType));
  strcpy(Home->name, "Home");
  Home->show_status_bar = 1;
  Home->BeforeEnter = NULL;
  Home->Created = Created;
  Home->Update = Update;
  Home->Destroy = Destroy;
  Home->Method = Method;
  Home->PageContent = create_new_screen();
  Page_Register(*Home);
}
