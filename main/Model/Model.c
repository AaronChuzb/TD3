/*
 * @Date: 2024-04-03 09:59:02
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-06 22:30:25
 */

#include "Model.h"

// 全局消息队列的句柄
extern QueueHandle_t message_queue;

void Queue_init(void)
{
  message_queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
}

void Model_receive_message_task(void *pvParameters)
{
  int flag = 0;
  while (1)
  {
    if (xQueueReceive(message_queue, &flag, portMAX_DELAY) == pdPASS)
    {
      if(flag == RESUME_TASK){
        printf("恢复任务\n");
        statusbar_task_resume();
        lvgl_task_resume();
      } else {
        printf("挂起任务\n");
        statusbar_task_suspend();
        lvgl_task_suspend();
      }
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void Model_init(void)
{
  // lvgl_task_init();
  statusbar_viewmodel_init();
  time_model_init();
  battery_model_init();
  Queue_init();
  xTaskCreate(Model_receive_message_task, "Model_receive_message_task", 1024 * 2, NULL, 2, NULL);
}
