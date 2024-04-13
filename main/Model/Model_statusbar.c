/*
 * @Date: 2024-04-09 13:52:54
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-13 16:28:02
 */

#include "Model.h"

TaskHandle_t tash_update_statusbar_handle;

inline float _getMin(float a, float b)
{
  return ((a) < (b) ? (a) : (b));
}

void tash_update_statusbar(void *pvParameters)
{
  char msg[20];
  char msg_battery[20];
  struct tm timechip1;
  float battery_level = 100.0;
  while (1)
  {
    // 更新时间
    pcf8563_get_time(&timechip1);
    sprintf(msg, "%02d:%02d", timechip1.tm_hour, timechip1.tm_min);
    lv_msg_send(MSG_TIME_SET, msg);
    battery_level = getBatLevel();
    int level = (int)battery_level;
    // printf("battery level: %d\n", level);
    if(level >= 0 && level < 20){
      lv_msg_send(MSG_BAT_SET, LV_SYMBOL_BATTERY_EMPTY);
    } else if(level >= 20 && level < 40){
      lv_msg_send(MSG_BAT_SET, LV_SYMBOL_BATTERY_1);
    } else if(level >= 40 && level < 60){
      lv_msg_send(MSG_BAT_SET, LV_SYMBOL_BATTERY_2);
    } else if(level >= 60 && level < 80){
      lv_msg_send(MSG_BAT_SET, LV_SYMBOL_BATTERY_3);
    } else if(level >= 80 && level <= 100){
      lv_msg_send(MSG_BAT_SET, LV_SYMBOL_BATTERY_FULL);
    }
    // 更新充电状态
    if (isCharging())
    {
      lv_msg_send(MSG_CHARGE_SET, LV_SYMBOL_CHARGE);
    }
    else
    {
      lv_msg_send(MSG_CHARGE_SET, "");
    }
    // 更新SD卡状态
    if (is_sd_present())
    {
      lv_msg_send(MSG_SDCARD_MOUNT, LV_SYMBOL_SD_CARD);
    }
    else
    {
      lv_msg_send(MSG_SDCARD_MOUNT, "");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void statusbar_viewmodel_init(void)
{
  // xTaskCreate(tash_update_statusbar, "tash_update_statusbar", 1024 * 10, NULL, 1, &tash_update_statusbar_handle);
  // 使用外部内存
  xTaskCreateWithCaps(tash_update_statusbar, "tash_update_statusbar", 20 * 1024, NULL, 1, &tash_update_statusbar_handle, MALLOC_CAP_SPIRAM);
}

void statusbar_task_suspend(void)
{
  vTaskSuspend(tash_update_statusbar_handle);
}

void statusbar_task_resume(void)
{
  vTaskResume(tash_update_statusbar_handle);
}