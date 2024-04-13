/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-13 16:28:29
 */

#include "StartUp.h"


// 声明页面结构体

struct PageType StartUp;


lv_obj_t *obj2;
static void event_btn1_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); // 获取回调事件
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    Page_Push("Home");
    // Page_Push("Settings");
  }
}

static void anim_x_cb(void *var, int32_t v)
{
  lv_obj_set_x((lv_obj_t *)var, v);
}

static void button_anim_in()
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj2);
  lv_anim_set_values(&a, -100, 5);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_x_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  // 页面切换动画执行完毕之后再执行出现动画
  lv_anim_set_delay(&a, 300);
  lv_anim_start(&a);
}


static void Created()
{

  // lv_obj_t *page_bg = lv_img_create(StartUp.PageContent);
  // lv_img_set_src(page_bg, &background);
  // lv_obj_set_width(page_bg, LV_SIZE_CONTENT);  /// 1
  // lv_obj_set_height(page_bg, LV_SIZE_CONTENT); /// 1
  // lv_obj_set_align(page_bg, LV_ALIGN_CENTER);
  // lv_obj_add_flag(page_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  // lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);   
  // lv_obj_set_y(page_bg, -30);
  // lv_img_set_zoom(page_bg, 155);                     /// Flags
  obj2 = lv_obj_create(StartUp.PageContent);
  lv_obj_set_size(obj2, 100, 150);
  lv_obj_set_pos(obj2, -100, 35);
  lv_obj_add_event_cb(obj2, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  button_anim_in();
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