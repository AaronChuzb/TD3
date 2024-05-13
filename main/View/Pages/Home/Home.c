/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-05-13 22:24:45
 */

#include "Home.h"

// 声明页面结构体

struct PageType Home;

lv_obj_t *obj1;
lv_obj_t *obj5;

// 记住时间状态
static char *time_str = "00:00";

static void event_btn1_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); // 获取回调事件
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    // Page_Back(1);
    Page_Push("Auth");
  }
}

static void event_btn2_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); // 获取回调事件
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    // Page_Back(1);
    // Page_Push("Auth");
    lv_msg_send(MSG_GET_SRAM, NULL);
  }
}

static void anim_y_cb(void *var, int32_t v)
{
  lv_obj_set_y((lv_obj_t *)var, v);
}

static void button_anim_in()
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj1);
  lv_anim_set_values(&a, -150, 35);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_y_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  // 页面切换动画执行完毕之后再执行出现动画
  lv_anim_set_delay(&a, 300);
  lv_anim_start(&a);
}

static void msg_event_cb(lv_event_t *e)
{
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);
  switch (lv_msg_get_id(m))
  {
  case MSG_TIME_SET:
    time_str = lv_msg_get_payload(m);
    lv_label_set_text(label, time_str);
    break;
  default:
    break;
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
      return;
    }

    lv_coord_t child_cnt = lv_obj_get_child_cnt(cont); // 获取子界面的数量
    lv_coord_t mid_btn_index = (child_cnt - 1) / 2;    // 中间界面的位置

    /* 获取父对象y轴的中心坐标值 */
    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);                                       // 将cont对象的坐标复制到cont_a
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2; // 获取界面的高像素大小/2
    // printf("cont_y_center %d", cont_y_center);
    /* 注意，这里的中心显示界面的坐标不在正中心，所以这里加上了差值 */
    // cont_y_center += 169;

    /* 遍历子界面 */
    for (lv_coord_t i = 0; i < child_cnt; i++)
    {
      lv_obj_t *child = lv_obj_get_child(cont, i); // 通过索引获取子对象

      /* 获取子对象y轴的中心坐标值 */
      lv_area_t child_a;
      lv_obj_get_coords(child, &child_a);
      lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2; // 获取界面中按钮高像素值的大小/2
      /* 子界面的坐标与父界面的坐标相等时，说明当前界面在父界面中显示 */
      if (child_y_center == cont_y_center)
      {
        /* 当前显示界面的索引 */
        lv_coord_t current_btn_index = lv_obj_get_index(child);

        /* 判断界面移动的数据，并将当前界面的索引改为中间位置 */
        /* 因为是在滑动结束后实现的，建议界面较多的情况下使用此方式，当界面较少，一次滑动太多界面时，容易滑倒边界出现卡顿现象 */
        lv_coord_t move_btn_quantity = LV_ABS(current_btn_index - mid_btn_index);
        for (lv_coord_t j = 0; j < move_btn_quantity; j++)
        {
          /* 向上滑动 */
          if (current_btn_index < mid_btn_index)
          {
            lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0);            // 将最后一个界面索引改为第一个界面
            lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // lv_obj_get_child 通过子索引获取对象的子对象
          }
          /* 向下滑动 */
          if (current_btn_index > mid_btn_index)
          {
            lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1);            // 将第一个界面的索引值改为最后一个界面
            lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // lv_obj_get_child 通过子索引获取对象的子对象
          }
        }
        /* 保证界面居中显示 */
        lv_obj_set_style_translate_y(lv_obj_get_child(cont, mid_btn_index), 0, 0);
        lv_obj_clear_state(lv_obj_get_child(cont, mid_btn_index - 1), LV_STATE_USER_1);
        lv_obj_add_state(lv_obj_get_child(cont, mid_btn_index), LV_STATE_USER_1);
        lv_obj_clear_state(lv_obj_get_child(cont, mid_btn_index + 1), LV_STATE_USER_1);
        break;
      }
    }
  }
}

/**
 * Translate the object as they scroll
 */
void lvgl_scroll_test(void)
{
  lv_obj_t *cont = lv_obj_create(Home.PageContent);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x101418), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(cont, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 25);
  lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 25);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  // lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);       // 为了方便实现，先把滚动的动画屏蔽
  lv_obj_add_event_cb(cont, scroll_end_event, LV_EVENT_SCROLL_END, NULL);

  // lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(cont, true, 0);
  lv_obj_set_scroll_dir(cont, LV_DIR_VER);
  lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  // lv_style_set_text_color(&style_btn, lv_color_black());
  lv_style_set_width(&style_btn, lv_pct(110));

  static const lv_style_prop_t style_prop[] =
      {
          LV_STYLE_WIDTH,
          LV_STYLE_PROP_INV};
  static lv_style_transition_dsc_t trans;
  lv_style_transition_dsc_init(
      &trans,
      style_prop,
      lv_anim_path_overshoot,
      200,
      0,
      NULL);

  lv_style_set_transition(&style_btn, &trans);
  LV_IMG_DECLARE(ui_img_battery_png); // assets/battery.png
  LV_IMG_DECLARE(ui_img_gyro_png);    // assets/gyro.png
  LV_IMG_DECLARE(ui_img_press_png);   // assets/press.png
  LV_IMG_DECLARE(ui_img_sdcard_png);  // assets/sdcard.png
  LV_IMG_DECLARE(ui_img_setting_png); // assets/setting.png
  LV_IMG_DECLARE(ui_img_time_png);    // assets/time.png
  LV_IMG_DECLARE(ui_img_wifi_png);    // assets/wifi.png
                                      // 生命周期钩子
  const lv_img_dsc_t *image_descs[] = {
      &ui_img_battery_png,
      &ui_img_gyro_png,
      &ui_img_press_png,
      &ui_img_sdcard_png,
      &ui_img_setting_png,
      &ui_img_time_png,
      &ui_img_wifi_png,
      // Add more image descriptors as needed
  };
  uint32_t i;
  for (i = 0; i < 7; i++)
  {
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_width(btn, lv_pct(50));
    lv_obj_set_height(btn, lv_pct(85));
    lv_obj_add_style(btn, &style_btn, LV_STATE_USER_1);
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text_fmt(label, "Button %" LV_PRIu32, i);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
    lv_obj_set_style_pad_left(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *img = lv_img_create(btn);
    lv_img_set_src(img, image_descs[i]);

    lv_obj_set_width(img, LV_SIZE_CONTENT);  /// 64
    lv_obj_set_height(img, LV_SIZE_CONTENT); /// 64
    lv_obj_set_align(img, LV_ALIGN_LEFT_MID);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 10, -15);
  }

  /*---------------------------------------- 指定中心显示界面 ----------------------------------------*/
  lv_coord_t mid_btn_index = (lv_obj_get_child_cnt(cont) - 1) / 2; // 如果界面为偶数，将中间数向下取整的界面设置为中间界面
  lv_coord_t child_cnt = lv_obj_get_child_cnt(cont);               // 获取子界面的数量
  int roll_direction = mid_btn_index - mid_btn_index;              // 确定滚动方向

  /* 通过循环将指定界面移到中心位置 */
  for (lv_coord_t i = 0; i < LV_ABS(roll_direction); i++)
  {
    if (roll_direction > 0)
    {
      lv_obj_move_to_index(lv_obj_get_child(cont, child_cnt - 1), 0); // 将最后一个界面的索引更改为 0 （移至第一个界面）
    }
    else
    {
      lv_obj_move_to_index(lv_obj_get_child(cont, 0), child_cnt - 1); // 将第一个界面的索引值改为最大值（移至最后一个界面）
    }
  }

  /*当按钮数为偶数时，确保按钮居中*/
  lv_obj_scroll_to_view(lv_obj_get_child(cont, mid_btn_index), LV_ANIM_OFF); // 滚动到一个对象，直到它在其父对象上可见
  lv_obj_add_state(lv_obj_get_child(cont, mid_btn_index), LV_STATE_USER_1);
}

static void Created()
{
  lvgl_scroll_test();
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