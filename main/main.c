
#include "HAL.h"

static const char *TAG = "main";

void app_main()
{
  HAL_init();
  char msg[20];
  struct tm timechip1;
  while (1)
  {

    pcf8563_get_time(&timechip1);
    sprintf(msg, "%d:%d", timechip1.tm_hour, timechip1.tm_min);
    lv_msg_send(MSG_TIME_SET, msg);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
