/*
 * @Date: 2024-04-03 09:52:59
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 16:52:50
 */

#ifndef MODEL_H
#define MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "HAL.h"

#include "Message.h"

#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE 10


void statusbar_viewmodel_init(void);
void statusbar_task_suspend(void);
void statusbar_task_resume(void);
void Model_init(void);




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif