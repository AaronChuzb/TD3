/*
 * @Date: 2024-04-05 21:06:40
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-03 17:25:30
 */


#ifndef VIEW_H
#define VIEW_H
 /* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#include "Page.h"
#include "StartUp.h"
#include "Auth.h"
#include "Home.h"
#include "Clock.h"
#include "Pressure.h"

//本文件是用于挂载自定义字体的文件
void init_view(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
