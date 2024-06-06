/*
 * @Date: 2024-04-03 09:52:59
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-06 22:31:32
 */

#ifndef MODEL_H
#define MODEL_H

#ifdef __cplusplus
extern "C"
{
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

#include "cJSON.h"

#include "HAL.h"
#include "Message.h"
#include "Page.h"

#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE 10

#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1
#define LVGL_TASK_STACK_SIZE (480 * 1024) // 使用外部内存 我他妈直接给480k
#define LVGL_TASK_PRIORITY 5

  void statusbar_viewmodel_init(void);
  void statusbar_task_suspend(void);
  void statusbar_task_resume(void);

  void lvgl_task_init(void);
  void lvgl_task_suspend(void);
  void lvgl_task_resume(void);
  bool lvgl_lock(int timeout_ms);
  void lvgl_unlock(void);

  void time_model_init();

  void battery_model_init(void);

  void cJSON_Init();
  void load_config2ram();
  char *get_config_item_char(char *key);
  bool update_config_item_char(char *key, char *value);
  int get_config_item_int(char *key);
  bool update_config_item_int(char *key, int value);
  bool get_config_item_bool(char *key);
  bool update_config_item_bool(char *key, bool value);


  

  void Model_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif