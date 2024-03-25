/*
 * @Date: 2024-03-09 19:37:07
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-23 19:44:22
 */
#ifndef SDMMC_H
#define SDMMC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"



#define EXAMPLE_MAX_CHAR_SIZE    128

#define MOUNT_POINT "/sdcard"

#define CONFIG_EXAMPLE_PIN_CLK  (GPIO_NUM_5)
#define CONFIG_EXAMPLE_PIN_CMD  (GPIO_NUM_6)
#define CONFIG_EXAMPLE_PIN_D0  (GPIO_NUM_4)

#define CONFIG_EXAMPLE_PIN_D1  (GPIO_NUM_3)
#define CONFIG_EXAMPLE_PIN_D2  (GPIO_NUM_8)
#define CONFIG_EXAMPLE_PIN_D3  (GPIO_NUM_7)

void init_sdmmc();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif