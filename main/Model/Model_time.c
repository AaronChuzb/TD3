/*
 * @Date: 2024-05-31 22:11:48
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-05-31 22:39:32
 */
#include "Model.h"

static struct tm timechip;

void get_time_from_rtc()
{
  if(initialized_pcf8563()){
    pcf8563_get_time(&timechip);
  }
}


static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_GET_TIME:
    get_time_from_rtc(&timechip);
    lv_msg_send(MSG_SET_TIME, &timechip);
    break;
  default:
    break;
  }
}

void time_model_init()
{
  lv_msg_subsribe(MSG_GET_TIME, event_handle_cb, NULL);
}

