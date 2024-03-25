/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "lvgl.h"
#include "lcd.h"
#include "i2c.h"
#include "axp173.h"
#include "blk.h"
#include "bmp280.h"
#include "ui.h"
#include "qmc5883l.h"
#include "lsm6ds3.h"
#include "sdmmc.h"
#include "wifi.h"

static const char *TAG = "main";

void bat_task(void *arg)
{
  while (1)
  {
    // 获取电量百分比
    float bat = getBatLevel();
    char str[10];
    sprintf(str, "%.1f%%", bat);
    lv_label_set_text(ui_W3_Num, str);

    char str1[10];
    float ax = lsm6ds3_get_accelerometer_x();
    sprintf(str1, "%.1f", ax);
    lv_label_set_text(ui_W1_Num, str1);

    char str2[10];
    float ay = lsm6ds3_get_accelerometer_y();
    sprintf(str2, "%.1f", ay);
    lv_label_set_text(ui_W2_Num, str2);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void app_main()
{
  init_lcd();
  init_i2c();
  // i2c_scan_devices();
  init_axp173();
  init_bmp280();
  init_lsm6ds3();
  init_blk();
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  // setBackLight(3000, 0);
  // vTaskDelay(6000 / portTICK_PERIOD_MS);
  setBackLight(3000, 50);
  init_sdmmc();
  // init_wifi();

  xTaskCreate(bat_task, "bat_tash", 1024 * 2, NULL, 1, NULL);
  while (1)
  {
    // lsm6ds3_get_accelerometer
    // lsm6ds3_get_accelerometer();
    // qmc5883l_raw_to_azimuth();
    // qmc5883l_azimuth_to_direction();
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
