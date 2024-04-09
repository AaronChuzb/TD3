/*
 * @Date: 2024-04-07 23:49:04
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 22:34:34
 */


#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// 定义所有的消息类型
#define MSG_WIFI_IS_CONNECTED     1
#define MSG_WIFI_NOT_CONNECTED     2

#define MSG_TIME_SET    3

#define MSG_BAT1_SET 4
#define MSG_BAT2_SET 5
#define MSG_BAT3_SET 6

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
