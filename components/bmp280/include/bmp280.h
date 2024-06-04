/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-02-04 20:05:40
 */
#pragma once

#ifndef BMP280_H
#define BMP280_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "i2c.h"

#define BMP_REG_ID 0x76

esp_err_t bmp280_restart();
esp_err_t bmp280_read_param();
esp_err_t bmp280_measure_cmd();
esp_err_t bmp280_read_data();
void init_bmp280();
 

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
