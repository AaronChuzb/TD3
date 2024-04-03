/*
 * @Date: 2024-04-03 09:59:02
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-03 10:03:24
 */

#include "Model.h"

// 初始化消息队列
void initModel(void)
{
  // 创建消息队列
  sensorQueue = xQueueCreate(QUEUE_LENGTH, sizeof(SensorMessage));
}

// 硬件采集任务
void vHardwareTask(void *pvParameters)
{
  while (1)
  {
    // 模拟从硬件中采集数据
    int data = 0;

    // 创建一个消息结构体
    SensorMessage message;
    message.sensorData = data;

    // 将消息发送到消息队列
    xQueueSend(sensorQueue, &message, portMAX_DELAY);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// 更新传感器数据
void updateSensorData(int data)
{
  SensorMessage message;
  message.sensorData = data;
  // 将消息发送到消息队列
  xQueueSend(sensorQueue, &message, portMAX_DELAY);
}