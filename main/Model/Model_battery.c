/*
 * @Date: 2024-06-06 21:49:24
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-06 22:44:10
 */
#include "Model.h"

static TaskHandle_t get_battery_data_task = NULL;

typedef struct
{
  float battery_voltage;
  float battery_current;
  bool battery_charge;
  float battery_coulomb;
} battery_data_t;

battery_data_t battery_data;

void get_battery_data_event(void *pvParameters)
{
  while (1)
  {
    if (lvgl_lock(-1))
    {
      //  printf("任务执行中\n");
      // 发送数据
      battery_data.battery_voltage = getBatVoltage();
      battery_data.battery_current = getBatCurrent();
      battery_data.battery_charge = isCharging();
      battery_data.battery_coulomb = getCoulometerData();
      lv_msg_send(MSG_BATTERY_DATA, &battery_data);
      lvgl_unlock();
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_BATTERY_GET_DATA_EVENT:
    // 创建任务
    // printf("创建任务\n");
    xTaskCreate(get_battery_data_event, "get_battery_data_event", 1024 * 5, NULL, 1, &get_battery_data_task);
    break;
  case MSG_BATTERY_DESTROY_EVENT:
    // 销毁任务
    vTaskDelete(get_battery_data_task);
    break;
  default:
    break;
  }
}

void battery_model_init(void)
{
  lv_msg_subsribe(MSG_BATTERY_GET_DATA_EVENT, event_handle_cb, NULL);
  lv_msg_subsribe(MSG_BATTERY_DESTROY_EVENT, event_handle_cb, NULL);
}