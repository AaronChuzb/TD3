/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-04 17:28:31
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

  static const char *btns[] = {"确定", "返回", ""};

  lv_obj_t *mbox1 = lv_msgbox_create(NULL, "提示", "功能正在开发中...", btns, false);
  lv_obj_add_style(mbox1, &font_style_dingdingjinbu_14, 0);
  lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_center(mbox1);
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