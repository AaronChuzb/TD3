/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-04 01:21:07
 */
#pragma once

#ifndef BLK_H
#define BLK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"


#define BACKLIGHT_GPIO (20)

void init_blk();
void setBackLightLevel(int level);
  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
