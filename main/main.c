/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "lvgl.h"
#include "i2c.h"
#include "axp173.h"
#include "blk.h"
#include "bmp280.h"
#include "ui.h"
#include "lsm6ds3.h"
#include "HAL.h"
#include "beep.h"
#include "pcf8563.h"

static const char *TAG = "main";

void app_main()
{

  init_blk();
  vTaskDelay(30 / portTICK_PERIOD_MS);
  setBackLightLevel(0);
  init_lcd();
  init_lvgl_port();
  init_i2c();
  // i2c_scan_devices();
  init_axp173();
  init_bmp280();
  init_lsm6ds3();
  init_pcf8563();

  // vTaskDelay(3000 / portTICK_PERIOD_MS);

  init_sdmmc();
  init_beep();
  // printf("%d", sizeof(YUEPU));
  //  vTaskDelay(6000 / portTICK_PERIOD_MS);
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  init_wifi();
  sntp_setlocaltime();
  // 获取剩余内存大小
  int freeHeap = esp_get_free_heap_size();

  // 打印剩余内存大小
  printf("Free heap: %d bytes\n", freeHeap);
  // 获取PSRAM的剩余内存大小
  printf("PSRAM free size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));


  while (1)
  {
    // lsm6ds3_get_accelerometer
    // lsm6ds3_get_accelerometer();
    // qmc5883l_raw_to_azimuth();
    // qmc5883l_azimuth_to_direction();
    // time(&now);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
