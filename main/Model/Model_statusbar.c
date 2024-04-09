/*
 * @Date: 2024-04-09 13:52:54
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 16:39:19
 */

#include "Model.h"

TaskHandle_t task_update_time_handle;

void task_update_time(void *pvParameters)
{
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

void statusbar_viewmodel_init(void)
{
  xTaskCreate(task_update_time, "task_update_time", 1024 * 2, NULL, 1, &task_update_time_handle);
}

void statusbar_task_suspend(void)
{
  vTaskSuspend(task_update_time_handle);
}

void statusbar_task_resume(void)
{
  vTaskResume(task_update_time_handle);
}