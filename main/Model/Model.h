/*
 * @Date: 2024-04-03 09:52:59
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 21:49:40
 */

#ifndef MODEL_H
#define MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


// 定义消息队列长度
#define QUEUE_LENGTH 5

// 定义消息结构体
typedef struct {
    int sensorData;
} SensorMessage;

// 创建消息队列句柄
QueueHandle_t sensorQueue;

void initModel(void);
void updateSensorData(int data);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif