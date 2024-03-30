/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-30 18:49:43
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
#define BinToBCD(bin) ((((bin) / 10) << 4) + ((bin) % 10))

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
    uint8_t month;
    uint16_t year;
} DATATIME;

void init_pcf8563();
esp_err_t pcf8563_set_datetime(DATATIME *dateTime);
esp_err_t pcf8563_get_datetime(DATATIME *dateTime);
// void setBackLightLevel(int level);
  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
