#ifndef LV_CUSTOM_FONTS_H
#define LV_CUSTOM_FONTS_H
 /* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "HAL.h"
#define LVGL_PATH "/sd"

//本文件是用于挂载自定义字体的文件
void lv_custom_fonts(void);

extern lv_style_t font_style_youyuan_21;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_CUSTOM_FONTS_H*/
