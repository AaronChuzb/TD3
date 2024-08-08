#ifndef HAL_H
#define HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h" // 使用外部内存任务堆栈的头文件
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "esp_pm.h"
#include "esp_system.h"

#include "driver/uart.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_sntp.h"
#include <time.h>

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

#include "esp_tls.h"
#include "esp_http_client.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "lvgl.h"
  // #include "lv_demos.h"

#include "esp_lcd_gc9b71.h"
#include "esp_lcd_touch_cst816s.h"

#include "iot_button.h"

#include "lvgl.h"
#include "i2c.h"
#include "axp173.h"
#include "blk.h"
#include "bmp280.h"

#include "lsm6ds3.h"
#include "beep.h"
#include "pcf8563.h"

#include "View.h"
#include "Message.h"

#include "cJSON.h"

#define USE_BLUETOOTH 1

// #define EXAMPLE_MAX_CHAR_SIZE    128
// SDMMC
#define MOUNT_POINT "/sd"

#define SDMMC_PIN_CLK (GPIO_NUM_5)
#define SDMMC_PIN_CMD (GPIO_NUM_6)
#define SDMMC_PIN_D0 (GPIO_NUM_4)

#define SDMMC_PIN_D1 (GPIO_NUM_3)
#define SDMMC_PIN_D2 (GPIO_NUM_8)
#define SDMMC_PIN_D3 (GPIO_NUM_7)

#define SDMMC_PIN_CD (GPIO_NUM_21)

  // WiFi

#define WIFI_SSID "AaronChu"
#define WIFI_PASS "05210122"
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

  // LCD

#define LCD_HOST SPI2_HOST
#define TOUCH_HOST I2C_NUM_0
#define LCD_BIT_PER_PIXEL (16)

#define CONFIG_LV_COLOR_DEPTH 16

#define PIN_NUM_LCD_CS (GPIO_NUM_14)
#define PIN_NUM_LCD_PCLK (GPIO_NUM_13)
#define PIN_NUM_LCD_DATA0 (GPIO_NUM_9)
#define PIN_NUM_LCD_DATA1 (GPIO_NUM_10)
#define PIN_NUM_LCD_DATA2 (GPIO_NUM_11)
#define PIN_NUM_LCD_DATA3 (GPIO_NUM_12)
#define PIN_NUM_LCD_RST (GPIO_NUM_15)

// The pixel number in horizontal and vertical
#define LCD_H_RES 320
#define LCD_V_RES 386

#define PIN_NUM_TOUCH_SCL (GPIO_NUM_19)
#define PIN_NUM_TOUCH_SDA (GPIO_NUM_18)
#define PIN_NUM_TOUCH_RST (GPIO_NUM_16)
#define PIN_NUM_TOUCH_INT (GPIO_NUM_17)

#define LVGL_TICK_PERIOD_MS 2

#define SUSPEND_TASK 0
#define RESUME_TASK 1

  /* Attributes State Machine */
  enum
  {
      IDX_SVC,
      // IDX_CHAR_A,
      // IDX_CHAR_VAL_A,
      // IDX_CHAR_CFG_A,

      // IDX_CHAR_LED,
      // IDX_CHAR_VAL_LED,

      IDX_CHAR_TEMP,
      IDX_CHAR_VAL_TEMP,
      IDX_CHAR_CFG_TEMP,

      // IDX_CHAR_B,
      // IDX_CHAR_VAL_B,

      // IDX_CHAR_C,
      // IDX_CHAR_VAL_C,

      HRS_IDX_NB,
  };
  /// Attributes State Machine
  // enum
  // {
  //   HRS_IDX_SVC,

  //   // HRS_IDX_HR_MEAS_CHAR,
  //   // HRS_IDX_HR_MEAS_VAL,
  //   // HRS_IDX_HR_MEAS_NTF_CFG,

  //   // HRS_IDX_BOBY_SENSOR_LOC_CHAR,
  //   // HRS_IDX_BOBY_SENSOR_LOC_VAL,

  //   IDX_CHAR_TEMP,
  //   IDX_CHAR_VAL_TEMP,
  //   IDX_CHAR_CFG_TEMP,

  //   // HRS_IDX_HR_CTNL_PT_CHAR,
  //   // HRS_IDX_HR_CTNL_PT_VAL,

  //   HRS_IDX_NB,
  // };

#ifndef ft_open                                                                                                                                 // 自定义 FATFS打开文件使用， ftopen(fd, path, FA_READ)
#define ft_open(fd, path, mode) (fd = heap_caps_malloc(sizeof(FIL), MALLOC_CAP_SPIRAM), (f_open(fd, (path), mode) == 0) ? (0) : (free(fd), -1)) // 自带申请一个内存 并且打开失败时自动释放内存
#endif
#ifndef ft_close                             // 自定义 FATFS关闭文件使用，ftclose(fd)
#define ft_close(fd) (f_close(fd), free(fd)) // 自带释放一个内存
#endif

  // HAL
  void HAL_init();
  void Pre_HAL_Init();
  // void get_sram_size();

  // SDMMC
  void init_sdmmc();
  bool is_sd_present();

  // SNTP
  void sntp_setlocaltime();
  void init_sntp();

  // WiFi
  void init_wifi(void);

  // LCD
  void init_lcd(void);
  void init_lvgl_port();

  // BUTTON
  void button_init();

  // POWER
  void power_save_init(void);

  // BEEP
  void init_tone();

  // FLASH
  void init_falsh(void);

  // UART
  void init_uarts(void);

  // BLE
  void hal_ble_init(void);

  // HTTP
  void http_native_request(void);
  void http_process(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif