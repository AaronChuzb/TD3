/*
 * @Date: 2024-04-05 21:08:09
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-07 23:42:18
 */
#include "Page.h"

struct PageType new_page, old_page, cur_page; // 新页面，旧页面，默认页面

struct PageType PageList[10];  // 页面列表
struct PageType PageStack[10]; // 页面堆栈
uint16_t PageNum = 10;         // 页面数量
uint16_t StackTop;             // 栈顶
bool isChanging;               // 页面切换中

uint16_t page_index = 0;

lv_style_t light_style;
lv_style_t dark_style;



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


void Page_Init()
{
  // 初始化默认页面
  cur_page.Created = NULL;
  cur_page.Destroy = NULL;
  StackTop = 0;
  // lv_timer_t *timer = lv_timer_create(update_statusbar, 100, NULL);
  // 初始化状态栏样式
  status_bar_style_init();
}

// 注册页面
bool Page_Register(struct PageType Page)
{
  printf("Page register: %s\n", Page.name);
  PageList[page_index] = Page;
  page_index++;
  return true;
}

// 页面入栈
bool Page_Push(char *name)
{
  printf("Page push: %s\n", name);
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
  printf("Find page and push to page:%s", page.name);
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
    status_bar_init(cur_page.PageContent);
    status_bar_in();
  }
  lv_scr_load(cur_page.PageContent);
  // lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false); // 不要删除旧屏幕对象指针，会导致返回页面空指针异常

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
  printf("Page replace: %s\n", name);
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
  printf("Find page and replace to page:%s", page.name);
  if (StackTop >= PageNum - 1)
  {
    return false;
  }
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
    status_bar_init(cur_page.PageContent);
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
  lv_scr_load(cur_page.PageContent);
  // lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, true);

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
    status_bar_init(cur_page.PageContent);
    status_bar_in();
  }
  lv_scr_load(cur_page.PageContent);
  // lv_scr_load_anim(cur_page.PageContent, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);

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

bool Page_refresh()
{
  lv_obj_clean(cur_page.PageContent);
  cur_page.Created();
  if (cur_page.show_status_bar == 1)
  {
    status_bar_init(cur_page.PageContent);
    status_bar_in();
  }
  lv_scr_load(cur_page.PageContent);
  return true;
}

void Page_Update()
{
  if (!isChanging)
  {
    cur_page.Update();
  }
}
