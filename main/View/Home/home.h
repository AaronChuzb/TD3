/*
 * @Date: 2024-04-04 23:22:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-04 23:23:24
 */

#ifndef HOME_H
#define HOME_H
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


//本文件是用于挂载自定义字体的文件
void init_view(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_CUSTOM_FONTS_H*/
