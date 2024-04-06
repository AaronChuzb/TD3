﻿/*
 * @Date: 2024-04-05 21:08:09
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 23:13:24
 */
#include "View/Page.h"

struct PageType new_page, old_page, cur_page; // 新页面，旧页面，默认页面

struct PageType PageList[10];  // 页面列表
struct PageType PageStack[10]; // 页面堆栈
uint16_t PageNum = 10;         // 页面数量
uint16_t StackTop;             // 栈顶
bool isChanging;               // 页面切换中

uint16_t page_index = 0;

lv_style_t light_style;
lv_style_t dark_style;
lv_obj_t *label_wifi = NULL;
lv_obj_t *label_batchar = NULL;
lv_obj_t *panel = NULL;

void my_timer(lv_timer_t *timer)
{
  // lv_obj_clean(timer->user_data);
  // lv_obj_t *page = (lv_obj_t *)timer->user_data;
  lv_obj_clean((lv_obj_t *)timer->user_data);
}

static void gesture_event(lv_event_t *e)
{
  lv_event_code_t event = lv_event_get_code(e);
  if (event == LV_EVENT_GESTURE)
  {
    //	等待触屏释放
    lv_indev_wait_release(lv_indev_get_act());
    //	获取方向
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    switch (dir)
    {
    case LV_DIR_LEFT:
      // imganmitoleft();        printf("to left\n");

      break;
    case LV_DIR_RIGHT:
      // imganmitoright();        printf("to right\n");
      Page_Back(1);
      break;
    }
  }
}

// 页面对象创建
lv_obj_t *create_new_screen()
{
  lv_obj_t *main_obj = lv_obj_create(NULL);
  lv_obj_clean(main_obj);
  lv_obj_set_size(main_obj, LV_HOR_RES, LV_VER_RES);
  return main_obj;
}

/**
 * @description: 状态栏更新定时器
 * @param {lv_timer_t} *timer
 * @return {*}
 */
static void update_statusbar(lv_timer_t *timer)
{
  // if (label_batchar != NULL)
  // {
  //   // 电池状态
  //   if (HAL::Get_Bat() > 0 && HAL::Get_Bat() <= 25)
  //   {
  //     lv_label_set_text(label_batchar, LV_SYMBOL_BATTERY_1);
  //   }
  //   else if (HAL::Get_Bat() > 25 && HAL::Get_Bat() <= 50)
  //   {
  //     lv_label_set_text(label_batchar, LV_SYMBOL_BATTERY_2);
  //   }
  //   else if (HAL::Get_Bat() > 50 && HAL::Get_Bat() <= 75)
  //   {
  //     lv_label_set_text(label_batchar, LV_SYMBOL_BATTERY_3);
  //   }
  //   else if (HAL::Get_Bat() > 75 && HAL::Get_Bat() <= 100)
  //   {
  //     lv_label_set_text(label_batchar, LV_SYMBOL_BATTERY_FULL);
  //   }
  //   // 是否充电中
  //   if (HAL::isCharging())
  //   {
  //     lv_label_set_text(label_batchar, LV_SYMBOL_CHARGE);
  //   }
  // }
  // if (label_wifi != NULL)
  // {
  //   if (HAL::Get_WiFiStatus() == WL_CONNECTED)
  //   {
  //     lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);
  //   }
  // }
}

// 状态栏动画
static void anim_y_cb(void *var, int32_t v)
{
  lv_obj_set_y((lv_obj_t *)var, v);
}
static void status_bar_in()
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, panel);
  lv_anim_set_values(&a, -25, 0);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_y_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  // 页面切换动画执行完毕之后再执行出现动画
  lv_anim_set_delay(&a, 300);
  lv_anim_start(&a);
}

static void status_bar_out()
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

// 页面状态栏创建
void Page_StatusBar_Init(lv_obj_t *pageContent)
{
  // 状态栏初始化
  static lv_style_t obj_layout_style; // 容器的样式
  lv_style_init(&obj_layout_style);
  lv_style_set_pad_all(&obj_layout_style, 0);
  // lv_style_set_bg_opa(&obj_layout_style, 50);
  lv_style_set_text_font(&obj_layout_style, &lv_font_montserrat_18);
  lv_style_set_border_side(&obj_layout_style, LV_BORDER_SIDE_NONE);
  lv_style_set_radius(&obj_layout_style, 0);
  lv_style_set_text_color(&obj_layout_style, lv_color_hex(0xffffff));
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

  label_wifi = lv_label_create(panel);
  lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);
  lv_obj_t *label_time = lv_label_create(panel);
  lv_label_set_text(label_time, "00:00");

  lv_obj_t *label_bat = lv_label_create(panel);
  lv_label_set_text(label_bat, LV_SYMBOL_BATTERY_EMPTY);
  label_batchar = lv_label_create(label_bat);
  lv_obj_set_style_text_font(label_batchar, &lv_font_montserrat_14, 0);
  lv_label_set_text(label_batchar, LV_SYMBOL_CHARGE);

  lv_obj_center(label_batchar);
}

void Page_Init()
{
  // 初始化默认页面
  cur_page.Created = NULL;
  cur_page.Destroy = NULL;
  StackTop = 0;
  lv_timer_t *timer = lv_timer_create(update_statusbar, 100, NULL);
}

// 注册页面
bool Page_Register(struct PageType Page)
{
  printf("222");
  PageList[page_index] = Page;
  page_index++;
  return true;
}

// 页面入栈
bool Page_Push(char *name)
{
  printf("333");
  struct PageType page;
  // 在路由表中查找改页面，未找到跳转失败。
  for (int i = 0; i < PageNum; i++)
  {
    if (strcmp(PageList[i].name, name) == 0)
    {
      page = PageList[i];
      break;
    }
  }
  // 防止堆栈溢出
  if (StackTop >= PageNum - 1)
  {
    return false;
  }
  StackTop++;
  isChanging = true;
  PageStack[StackTop] = page;
  new_page = PageStack[StackTop];
  old_page = cur_page;
  cur_page = new_page;

  cur_page.Created();
  // 如果旧页面有状态栏，显示移除动画
  if (old_page.show_status_bar == 1)
  {
    status_bar_out();
  }
  // 如果新页面显示状态栏，初始化状态栏，显示载入动画
  if (cur_page.show_status_bar == 1)
  {
    Page_StatusBar_Init(cur_page.PageContent);
    status_bar_in();
  }
  lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false); // 不要删除旧屏幕对象指针，会导致返回页面空指针异常

  if (old_page.Destroy != NULL)
  {
    // 清空旧页面的事件
    lv_obj_remove_event_cb(old_page.PageContent, gesture_event);
    old_page.Destroy();
    lv_timer_t *timer = lv_timer_create(my_timer, 300, old_page.PageContent);
    lv_timer_set_repeat_count(timer, 1);
  }
  isChanging = false;
  // 添加手势监听
  lv_obj_add_event_cb(cur_page.PageContent, gesture_event, LV_EVENT_GESTURE, NULL);
  return true;
}

// 页面入栈
bool Page_Replace(char *name)
{
  printf("444");
  struct PageType page;
  // 在路由表中查找改页面，未找到跳转失败。
  for (int i = 0; i < PageNum; i++)
  {
    if (strcmp(PageList[i].name, name) == 0)
    {
      page = PageList[i];
      break;
    }
  }
  printf("555 %s", page.name);
  if (StackTop >= PageNum - 1)
  {
    printf("777");
    return false;
  }
  isChanging = true;
  PageStack[StackTop] = page;
  new_page = PageStack[StackTop];
  old_page = cur_page;
  cur_page = new_page;
  cur_page.Created();
  printf("666 %s", cur_page.name);
  // 如果旧页面有状态栏，显示移除动画
  if (old_page.show_status_bar == 1)
  {
    status_bar_out();
  }
  // 如果新页面显示状态栏，初始化状态栏，显示载入动画
  if (cur_page.show_status_bar == 1)
  {
    Page_StatusBar_Init(cur_page.PageContent);
    status_bar_in();
  }
  if (old_page.Destroy != NULL)
  {
    // 清空旧页面的事件
    lv_obj_remove_event_cb(old_page.PageContent, gesture_event);
    old_page.Destroy();
    lv_timer_t *timer = lv_timer_create(my_timer, 300, old_page.PageContent);
    lv_timer_set_repeat_count(timer, 1);
  }
  lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);

  isChanging = false;
  // 添加手势监听
  lv_obj_add_event_cb(cur_page.PageContent, gesture_event, LV_EVENT_GESTURE, NULL);
  return true;
}

bool Page_Back(uint16_t delt)
{
  // 防止堆栈溢出
  if (StackTop == 0)
  {
    return false;
  }
  while (delt > 0)
  {
    StackTop--;
    delt -= 1;
  }
  isChanging = true;
  old_page = cur_page;
  cur_page = PageStack[StackTop];
  cur_page.Created();
  // 如果旧页面有状态栏，显示移除动画
  if (old_page.show_status_bar == 1)
  {
    status_bar_out();
  }
  // 如果新页面显示状态栏，初始化状态栏，显示载入动画
  if (cur_page.show_status_bar == 1)
  {
    Page_StatusBar_Init(cur_page.PageContent);
    status_bar_in();
  }
  lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);

  if (old_page.Destroy != NULL)
  {
    // 清空旧页面的事件
    lv_obj_remove_event_cb(old_page.PageContent, gesture_event);
    old_page.Destroy();
    lv_timer_t *timer = lv_timer_create(my_timer, 300, old_page.PageContent);
    lv_timer_set_repeat_count(timer, 1);
  }
  isChanging = false;
  // 添加手势监听
  lv_obj_add_event_cb(cur_page.PageContent, gesture_event, LV_EVENT_GESTURE, NULL);
  return true;
}

void Page_Update()
{
  if (!isChanging)
  {
    cur_page.Update();
  }
}