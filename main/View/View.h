/*
 * @Date: 2024-04-05 21:06:40
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 23:35:43
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

#include "View/lv_port/lv_fonts/lv_custom_fonts.h"
#include "View/Page.h"
#include "View/Pages/StartUp/StartUp.h"
#include "View/Pages/Home/Home.h"

//本文件是用于挂载自定义字体的文件
void init_view(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
