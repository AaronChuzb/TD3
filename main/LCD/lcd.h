#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lv_demos.h"

#include "esp_lcd_gc9b71.h"
#include "esp_lcd_touch_cst816s.h"



#define LCD_HOST    SPI2_HOST
#define TOUCH_HOST  I2C_NUM_0
#define LCD_BIT_PER_PIXEL       (16)


#define CONFIG_LV_COLOR_DEPTH 16

#define PIN_NUM_LCD_CS            (GPIO_NUM_14)
#define PIN_NUM_LCD_PCLK          (GPIO_NUM_13)
#define PIN_NUM_LCD_DATA0         (GPIO_NUM_9)
#define PIN_NUM_LCD_DATA1         (GPIO_NUM_10)
#define PIN_NUM_LCD_DATA2         (GPIO_NUM_11)
#define PIN_NUM_LCD_DATA3         (GPIO_NUM_12)
#define PIN_NUM_LCD_RST           (GPIO_NUM_15)


// The pixel number in horizontal and vertical
#define LCD_H_RES              320
#define LCD_V_RES              386


#define PIN_NUM_TOUCH_SCL         (GPIO_NUM_19)
#define PIN_NUM_TOUCH_SDA         (GPIO_NUM_18)
#define PIN_NUM_TOUCH_RST         (GPIO_NUM_16)
#define PIN_NUM_TOUCH_INT         (GPIO_NUM_17)


#define LVGL_TICK_PERIOD_MS    2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1
#define LVGL_TASK_STACK_SIZE   (4 * 1024)
#define LVGL_TASK_PRIORITY     2




void init_lcd(void);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif