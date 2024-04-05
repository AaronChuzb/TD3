/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 23:37:31
 */

#include "View/Pages/StartUp/StartUp.h"
#include "View/lv_port/lv_fonts/lv_custom_fonts.h"

// 声明页面结构体

struct PageType StartUp;

static void event_btn1_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); // 获取回调事件
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    Page_Push("Home");
    // Page_Push("Settings");
  }
}

static void Created()
{

  lv_obj_t *obj1;
  obj1 = lv_obj_create(StartUp.PageContent);
  lv_obj_set_size(obj1, 100, 150);
  lv_obj_set_pos(obj1, 5, 35);
  lv_obj_add_event_cb(obj1, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  lv_obj_t *label = lv_label_create(StartUp.PageContent);

   lv_label_set_text(label, "入口页");
  lv_obj_add_style(label, &font_style_youyuan_21, 0);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 50);
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

void StartUp_Init()
{
  printf("111");
  strcpy(StartUp.name, "StartUp");
  StartUp.show_status_bar = 1;
  StartUp.BeforeEnter = NULL;
  StartUp.Created = Created;
  StartUp.Update = Update;
  StartUp.Destroy = Destroy;
  StartUp.Method = Method;
  StartUp.PageContent = create_new_screen();
  Page_Register(StartUp);
}