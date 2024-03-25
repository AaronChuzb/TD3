/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-25 21:27:21
 */
#pragma once

#ifndef LSM6DS3_H
#define LSM6DS3_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "i2c.h"

#define REG_ID 0x6A

  // esp_err_t bmp280_restart();
  // esp_err_t bmp280_read_param();
  // esp_err_t bmp280_measure_cmd();
  // esp_err_t bmp280_read_data();
  esp_err_t lsm6ds3_get_id();
  esp_err_t lsm6ds3_get_gyroscope();
  esp_err_t lsm6ds3_get_accelerometer();
  float lsm6ds3_get_accelerometer_x();
  float lsm6ds3_get_accelerometer_y();
  void init_lsm6ds3();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
