/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-05-31 11:30:07
 */

#include "Home.h"
#include "Fonts.h"
#include <stdlib.h>

// 声明页面结构体

struct PageType Home;

lv_obj_t *obj1;
lv_obj_t *obj5;

lv_obj_t *content_title;
int current_index = 0;

LV_IMG_DECLARE(ui_img_battery_png);
LV_IMG_DECLARE(ui_img_gyro_png);
LV_IMG_DECLARE(ui_img_press_png);
LV_IMG_DECLARE(ui_img_sdcard_png);
LV_IMG_DECLARE(ui_img_setting_png);
LV_IMG_DECLARE(ui_img_time_png);
LV_IMG_DECLARE(ui_img_net_png);
LV_IMG_DECLARE(ui_img_weather_png);
LV_IMG_DECLARE(ui_img_music_png);

struct ButtonData
{
  char *title;
  lv_img_dsc_t *img;
  char *path;
};

const struct ButtonData button_data[9] = {
    {"电池信息", &ui_img_battery_png, ""},
    {"陀螺仪", &ui_img_gyro_png, ""},
    {"气压计", &ui_img_press_png, ""},
    {"储存卡", &ui_img_sdcard_png, ""},
    {"实时时钟", &ui_img_time_png, "Clock"},
    {"系统设置", &ui_img_setting_png, ""},
    {"网络配置", &ui_img_net_png, ""},
    {"天气查询", &ui_img_weather_png, ""},
    {"MIDI音乐", &ui_img_music_png, ""},
};

static void boxarea_event_cb(lv_event_t *e)
{
  // lv_obj_t *box = lv_event_get_target(e); // 获取事件的初始对象
  lv_event_code_t code = lv_event_get_code(e);
  // if (lv_obj_is_scrolling(box))
  // {
  //   return;
  // }
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
  lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;
  lv_coord_t r = lv_obj_get_width(cont) * 7 / 10;
  uint32_t i;
  uint32_t child_cnt = lv_obj_get_child_cnt(cont);
  lv_coord_t mid_btn_index = (child_cnt - 1) / 2;
  for (i = 0; i < child_cnt; i++)
  {
    lv_obj_t *child = lv_obj_get_child(cont, i);
    lv_area_t child_a;
    lv_obj_get_coords(child, &child_a);
    lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;
    lv_coord_t diff_y = child_y_center - cont_y_center;
    diff_y = LV_ABS(diff_y);
    lv_coord_t x;
    if (diff_y >= r)
    {
      x = r;
    }
    else
    {
      uint32_t x_sqr = r * r - diff_y * diff_y;
      lv_sqrt_res_t res;
      lv_sqrt(x_sqr, &res, 0x8000);
      x = r - res.i - 20;
    }
    lv_obj_set_style_translate_x(child, x, 0);

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
        // printf("current_btn_index:%d", current_btn_index);
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
        lv_label_set_text(content_title, button_data[current_index].title);
        lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF);
        break;
      }
    }
  }
}

void home_menu()
{
  lv_obj_t *cont = lv_obj_create(Home.PageContent);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(cont, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 25);
  lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 25);

  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

  lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL); // 为了方便实现，先把滚动的动画屏蔽
  lv_obj_add_event_cb(cont, scroll_end_event, LV_EVENT_SCROLL_END, NULL);

  // lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(cont, true, 0);
  lv_obj_set_scroll_dir(cont, LV_DIR_VER);
  lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t *box = lv_btn_create(Home.PageContent);
  lv_obj_align(box, LV_ALIGN_RIGHT_MID, LV_HOR_RES / 2, 12);
  lv_obj_set_size(box, LV_HOR_RES, LV_HOR_RES);
  lv_obj_set_style_radius(box, 70, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(box, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(box, boxarea_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t *title_box = lv_obj_create(box);
  lv_obj_remove_style_all(title_box);
  lv_obj_align(title_box, LV_ALIGN_TOP_LEFT, -15, 10);
  lv_obj_set_size(title_box, LV_HOR_RES / 2, 30);

  content_title = lv_label_create(title_box);
  lv_obj_add_style(content_title, &font_style_youshebiaotihei_24, 0);
  lv_label_set_text(content_title, "");
  lv_obj_set_style_text_color(content_title, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_set_align(content_title, LV_ALIGN_CENTER);

  uint32_t i;
  for (i = 0; i < sizeof(button_data) / sizeof(button_data[0]); i++)
  {
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_width(btn, lv_pct(28));
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
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 0, 0);
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
  lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_ON);
}

static void Created()
{
  lv_obj_set_scrollbar_mode(Home.PageContent, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(Home.PageContent, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_clip_corner(Home.PageContent, true, 0);
  home_menu();

  // lv_obj_t *arc = lv_arc_create(Home.PageContent);
  // lv_obj_set_size(arc, 150, 150);
  // lv_arc_set_rotation(arc, 200);
  // lv_arc_set_bg_angles(arc, 0, 45);
  // lv_arc_set_value(arc, 40);
  // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  // lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  // lv_obj_set_pos(arc, 10, 25);
  // lv_obj_center(arc);
  // lv_obj_t *page_bg = lv_img_create(Home.PageContent);
  // lv_img_set_src(page_bg, &background);
  // lv_obj_set_width(page_bg, LV_SIZE_CONTENT);  /// 1
  // lv_obj_set_height(page_bg, LV_SIZE_CONTENT); /// 1
  // lv_obj_set_align(page_bg, LV_ALIGN_CENTER);
  // lv_obj_add_flag(page_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  // lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);                        /// Flags
  // lv_obj_set_y(page_bg, -30);
  // lv_img_set_zoom(page_bg, 155);

  // obj1 = lv_obj_create(Home.PageContent);
  // lv_obj_set_size(obj1, 100, 150);
  // lv_obj_set_pos(obj1, 5, -150);
  // lv_obj_add_event_cb(obj1, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  // obj5 = lv_obj_create(Home.PageContent);
  // lv_obj_set_size(obj5, 100, 150);
  // lv_obj_set_pos(obj5, 200, 35);
  // lv_obj_add_event_cb(obj5, event_btn2_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  // lv_obj_t *label = lv_label_create(Home.PageContent);
  // lv_label_set_text(label, time_str);
  // lv_obj_add_style(label, &font_style_gigi_72, 0);
  // lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -50);

  // // 设置消息回调
  // lv_obj_add_event_cb(label, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // // 订阅消息
  // lv_msg_subsribe_obj(MSG_TIME_SET, label, NULL);

  // button_anim_in();

  //   lv_obj_t* img;

  // img = lv_img_create(Home.PageContent);
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
  // if (lv_obj_is_valid(Home.PageContent))
  // {
  //   lv_async_call(lv_obj_clean, Home.PageContent);
  // }
  
}

static void Method(void *btn, int event)
{
}

void Home_Init()
{
  strcpy(Home.name, "Home");
  Home.show_status_bar = 1;
  Home.BeforeEnter = NULL;
  Home.Created = Created;
  Home.Update = Update;
  Home.Destroy = Destroy;
  Home.Method = Method;
  Home.PageContent = create_new_screen();
  Page_Register(Home);
}
