/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-27 17:07:35
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

static void Created()
{
  // lv_obj_t *page_bg = lv_img_create(Home.PageContent);
  // lv_img_set_src(page_bg, &background);
  // lv_obj_set_width(page_bg, LV_SIZE_CONTENT);  /// 1
  // lv_obj_set_height(page_bg, LV_SIZE_CONTENT); /// 1
  // lv_obj_set_align(page_bg, LV_ALIGN_CENTER);
  // lv_obj_add_flag(page_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  // lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);                        /// Flags
  // lv_obj_set_y(page_bg, -30);
  // lv_img_set_zoom(page_bg, 155);

  obj1 = lv_obj_create(Home.PageContent);
  lv_obj_set_size(obj1, 100, 150);
  lv_obj_set_pos(obj1, 5, -150);
  lv_obj_add_event_cb(obj1, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  obj5 = lv_obj_create(Home.PageContent);
  lv_obj_set_size(obj5, 100, 150);
  lv_obj_set_pos(obj5, 200, 35);
  lv_obj_add_event_cb(obj5, event_btn2_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  lv_obj_t *label = lv_label_create(Home.PageContent);
  lv_label_set_text(label, time_str);
  lv_obj_add_style(label, &font_style_gigi_72, 0);
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -50);

  // 设置消息回调
  lv_obj_add_event_cb(label, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_TIME_SET, label, NULL);

  button_anim_in();
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