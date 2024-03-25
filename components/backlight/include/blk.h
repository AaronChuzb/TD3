/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-20 22:25:20
 */
#pragma once

#ifndef BLK_H
#define BLK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "driver/ledc.h"
#include "esp_log.h"




#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE 0
#define LEDC_HS_CH0_GPIO (20)
#define LEDC_HS_CH0_CHANNEL LEDC_CHANNEL_0

#define LEDC_TEST_CH_NUM (1)
#define LEDC_TEST_DUTY (2000)      // 渐变的变大最终目标占空比
#define LEDC_TEST_FADE_TIME (1000) // 变化时长

void init_blk();
void setBackLight(int time, uint16_t val);
  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
