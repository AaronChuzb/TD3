/*
 * @Date: 2024-07-23 17:24:18
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-07-24 11:48:21
 */

#include "Terminal.h"

PageType *Terminal;

#define MAX_LINES 10       // 屏幕最多显示行数
static int line_index = 0; // 当前显示行索引
#define MAX_LOG_LENGTH 1024
lv_obj_t *label1;

static void event_cb(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_current_target(e);
  printf("%s", lv_msgbox_get_active_btn_text(obj));
  lv_msgbox_close(obj);
  Page_Back(1);
}

 
static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_UART_DATA:
    char *data = lv_msg_get_payload(m);
    lv_label_set_text(label1, data);
    break;
  default:
    break;
  }
}

static void Created()
{
  label1 = lv_label_create(Terminal->PageContent);
  lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);
  lv_obj_add_style(label1, &font_style_dingdingjinbu_14, LV_PART_MAIN);
  lv_msg_subsribe(MSG_UART_DATA, event_handle_cb, NULL);
  lv_obj_set_width(label1, 310); /*Set smaller width to make the lines wrap*/
  lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 30);
  lv_obj_set_style_text_color(label1, lv_color_hex(0x61b865), LV_PART_MAIN);
  
}

static void Update(void)
{
}

static void Destroy(void)
{
  if (lv_obj_is_valid(Terminal->PageContent))
  {
    lv_async_call(lv_obj_clean, Terminal->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Terminal_Init()
{
  Terminal = lv_mem_alloc(sizeof(PageType));
  strcpy(Terminal->name, "Terminal");
  Terminal->show_status_bar = 1;
  Terminal->BeforeEnter = NULL;
  Terminal->Created = Created;
  Terminal->Update = Update;
  Terminal->Destroy = Destroy;
  Terminal->Method = Method;
  Terminal->PageContent = create_new_screen();
  Page_Register(*Terminal);
}