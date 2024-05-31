/*
 * @Date: 2024-04-07 23:44:15
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-08 00:05:50
 */

#ifndef STATUSBAR_H
#define STATUSBAR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "Message.h"
#include "Page.h"

void status_bar_style_init();
void status_bar_init(lv_obj_t *pageContent);
void status_bar_in(void);
void status_bar_out(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
