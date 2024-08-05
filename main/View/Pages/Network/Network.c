/*
 * @Date: 2024-07-23 16:55:14
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-07-23 16:55:36
 */

#include "Network.h"

PageType *Network;

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
  if (lv_obj_is_valid(Network->PageContent))
  {
    lv_async_call(lv_obj_clean, Network->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Network_Init()
{
  Network = lv_mem_alloc(sizeof(PageType));
  strcpy(Network->name, "Network");
  Network->show_status_bar = 1;
  Network->BeforeEnter = NULL;
  Network->Created = Created;
  Network->Update = Update;
  Network->Destroy = Destroy;
  Network->Method = Method;
  Network->PageContent = create_new_screen();
  Page_Register(*Network);
}