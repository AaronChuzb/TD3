/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-26 22:30:35
 */
#pragma once

#ifndef BEEP_H
#define BEEP_H

#ifdef __cplusplus
extern "C"
{
#endif





#define SPEED	500



#include <stdio.h>
#include "driver/ledc.h"
#include "esp_log.h"




void init_beep();
void setBeepTone(uint16_t freq);
  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
