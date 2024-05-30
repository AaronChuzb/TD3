/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditClock: 2024-05-30 16:30:08
 */

#include "Clock.h"

struct PageType Clock;


static void Created()
{

  

  
}

static void Update(void)
{
}

static void Destroy(void)
{
  lv_async_call(lv_obj_clean, Clock.PageContent);
}

static void Method(void *btn, int event)
{
}

void Clock_Init()
{
  strcpy(Clock.name, "Clock");
  Clock.show_status_bar = 1;
  Clock.BeforeEnter = NULL;
  Clock.Created = Created;
  Clock.Update = Update;
  Clock.Destroy = Destroy;
  Clock.Method = Method;
  Clock.PageContent = create_new_screen();
  Page_Register(Clock);
}