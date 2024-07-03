/*
 * @Date: 2024-06-13 14:24:45
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-13 16:25:50
 */
#include "Model.h"

static TaskHandle_t get_gyro_data_task = NULL;


static gyro_data_t gyro_data;

void get_gyro_data_event(void *pvParameters)
{
  while (1)
  {
    if (lvgl_lock(-1))
    {
      //  printf("任务执行中\n");
      // 发送数据
      gyro_data = lsm6ds3_get_all_data();
      
      lv_msg_send(MSG_GYRO_DATA, &gyro_data);
      lvgl_unlock();
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

static void event_handle_cb(void *s, lv_msg_t *m)
{
  LV_UNUSED(s);
  switch (lv_msg_get_id(m))
  {
  case MSG_GYRO_GET_DATA_EVENT:
    // 创建任务
    // printf("创建任务\n");
    xTaskCreate(get_gyro_data_event, "get_gyro_data_event", 1024 * 5, NULL, 1, &get_gyro_data_task);
    break;
  case MSG_GYRO_DESTROY_EVENT:
    // 销毁任务
    vTaskDelete(get_gyro_data_task);
    break;
  default:
    break;
  }
}

void gyro_model_init(void)
{
  lv_msg_subsribe(MSG_GYRO_GET_DATA_EVENT, event_handle_cb, NULL);
  lv_msg_subsribe(MSG_GYRO_DESTROY_EVENT, event_handle_cb, NULL);
}