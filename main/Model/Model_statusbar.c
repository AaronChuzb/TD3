/*
 * @Date: 2024-04-09 13:52:54
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-11 23:18:58
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
  char msg1[20];
  char msg2[20];
  char msg3[20];
  struct tm timechip1;
  float battery_level = 100.0;
  while (1)
  {
    // 更新时间
    pcf8563_get_time(&timechip1);
    sprintf(msg, "%02d:%02d", timechip1.tm_hour, timechip1.tm_min);
    lv_msg_send(MSG_TIME_SET, msg);
    battery_level = getBatLevelWithColumeter();
    sprintf(msg_battery, "%d%%", (int)battery_level);
    lv_msg_send(MSG_BAT_SET, msg_battery);
    
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
    float coulometer_data = getCoulometerData();
    float battery_data = getBatVoltage();
    
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