/*
 * @Date: 2024-03-09 19:37:07
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-01 22:19:32
 */
#ifndef SNTP_H
#define SNTP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "esp_sntp.h"
#include <time.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void init_sntp();
void sntp_setlocaltime();



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif