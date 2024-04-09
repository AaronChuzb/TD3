/*
 * @Date: 2024-04-09 13:52:54
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 23:30:34
 */

#include "Model.h"

TaskHandle_t task_update_time_handle;

void task_update_time(void *pvParameters)
{
  char msg[20];
  char msg1[20];
  char msg2[20];
  char msg3[20];
  struct tm timechip1;
  while (1)
  {
    pcf8563_get_time(&timechip1);
    sprintf(msg, "%d:%d", timechip1.tm_hour, timechip1.tm_min);
    lv_msg_send(MSG_TIME_SET, msg);
    float coulometer_data = getCoulometerData();
    float battery_data = getBatVoltage();
    float battery_level = getBatLevel();
    sprintf(msg1, "%.2f", coulometer_data);
    sprintf(msg2, "%.2f", battery_data);
    sprintf(msg3, "%.2f", battery_level);
    lv_msg_send(MSG_BAT1_SET, msg1);
    lv_msg_send(MSG_BAT2_SET, msg2);
    lv_msg_send(MSG_BAT3_SET, msg3);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void statusbar_viewmodel_init(void)
{
  xTaskCreate(task_update_time, "task_update_time", 1024 * 4, NULL, 1, &task_update_time_handle);
}

void statusbar_task_suspend(void)
{
  vTaskSuspend(task_update_time_handle);
}

void statusbar_task_resume(void)
{
  vTaskResume(task_update_time_handle);
}