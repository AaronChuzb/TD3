/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-08 13:18:50
 */
#pragma once

#ifndef PCF8563_H
#define PCF8563_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "i2c.h"
#include <time.h>
// #include <sys/time.h>

#define REG_ID 0x51

void init_pcf8563();
esp_err_t pcf8563_set_time(struct tm *time);
esp_err_t pcf8563_get_time(struct tm *time);
// void setBackLightLevel(int level);
  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
