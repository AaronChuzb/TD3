/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "lvgl.h"
#include "i2c.h"
#include "axp173.h"
#include "blk.h"
#include "bmp280.h"
#include "ui.h"
#include "lsm6ds3.h"
#include "HAL.h"
#include "beep.h"
#include "pcf8563.h"



// 定义按钮GPIO
#define BUTTON_GPIO 41

// 初始化按钮GPIO
void button_init()
{
  gpio_config_t io_conf;
  // 设置GPIO模式为输入
  io_conf.mode = GPIO_MODE_INPUT;
  // 设置GPIO引脚
  io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO);
  // 使能上拉
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);
}

// 检测按钮是否被按下
bool button_is_pressed(int gpio)
{
  return gpio_get_level(gpio) == 0;
}

// 任务：检测按钮并打印信息
void button_task(void *pvParameter)
{
  button_init();
  int flag = 0;
  while (1)
  {
    if (button_is_pressed(BUTTON_GPIO))
    {
      printf("Button is pressed\n");
      setBackLightLevel(0);
      // flag ++;
      // if(flag > 10 && flag < 1000){
      //   flag = 0;
      //   setBackLightLevel(0);
      // }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

static const char *TAG = "main";


// void bat_task(void *arg)
// {
//   while (1)
//   {
//     // 获取电量百分比
//     float bat = getBatLevel();
//     char str[10];
//     sprintf(str, "%.1f%%", bat);
//     lv_label_set_text(ui_W3_Num, str);

//     char str1[10];
//     float ax = lsm6ds3_get_accelerometer_x();
//     sprintf(str1, "%.1f", ax);
//     lv_label_set_text(ui_W1_Num, str1);

//     char str2[10];
//     float ay = lsm6ds3_get_accelerometer_y();
//     sprintf(str2, "%.1f", ay);
//     lv_label_set_text(ui_W2_Num, str2);
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

#define P 0
#define L1 1
#define L1_ 2
#define L2 3
#define L2_ 4
#define L3 5
#define L4 6
#define L4_ 7
#define L5 8
#define L5_ 9
#define L6 10
#define L6_ 11
#define L7 12
#define M1 13
#define M1_ 14
#define M2 15
#define M2_ 16
#define M3 17
#define M4 18
#define M4_ 19
#define M5 20
#define M5_ 21
#define M6 22
#define M6_ 23
#define M7 24
#define H1 25
#define H1_ 26
#define H2 27
#define H2_ 28
#define H3 29
#define H4 30
#define H4_ 31
#define H5 32
#define H5_ 33
#define H6 34
#define H6_ 35
#define H7 36

// 索引与频率对照表
unsigned int FreqTable[] = {
    0, // 休止符，表示不弹
    262,
    277,
    294,
    311,
    330,
    349,
    370,
    392,
    415,
    440,
    466,
    496,
    523,
    554,
    587,
    622,
    659,
    698,
    740,
    784,
    831,
    880,
    932,
    988,
    1046,
    1109,
    1175,
    1245,
    1318,
    1397,
    1480,
    1568,
    1661,
    1760,
    1865,
    1976,
};

struct YINFU
{ // 定义音符
  int freq;
  int Duration;
};

struct YINFU MUSIC[512] = {
    {P, 4},
    {P, 4},
    {P, 4},
    {M6, 2},
    {M7, 2},
    {H1, 4 + 2},
    {M7, 2},
    {H1, 4},
    {H3, 4},
    {M7, 4 + 4 + 4},
    {M3, 2},
    {M3, 2},
    {M6, 4 + 2},
    {M5, 2},
    {M6, 4},
    {H1, 4},
    {M5, 4 + 4 + 4},
    {M3, 4},
    {M4, 4 + 2},
    {M3, 2},
    {M4, 4},
    {H1, 4},
    {M3, 4 + 4},
    {P, 2},
    {H1, 2},
    {H1, 2},
    {H1, 2},
    {M7, 4 + 2},
    {M4_, 2},
    {M4_, 4},
    {M7, 4},
    {M7, 8},
    {P, 4},
    {M6, 2},
    {M7, 2},
    {H1, 4 + 2},
    {M7, 2},
    {H1, 4},
    {H3, 4},
    {M7, 4 + 4 + 4},
    {M3, 2},
    {M3, 2},
    {M6, 4 + 2},
    {M5, 2},
    {M6, 4},
    {H1, 4},
    {M5, 4 + 4 + 4},
    {M2, 2},
    {M3, 2},
    {M4, 4},
    {H1, 2},
    {M7, 2 + 2},
    {H1, 2 + 4},
    {H2, 2},
    {H2, 2},
    {H3, 2},
    {H1, 2 + 4 + 4},
    {H1, 2},
    {M7, 2},
    {M6, 2},
    {M6, 2},
    {M7, 4},
    {M5_, 4},
    {M6, 4 + 4 + 4},
    {H1, 2},
    {H2, 2},
    {H3, 4 + 2},
    {H2, 2},
    {H3, 4},
    {H5, 4},
    {H2, 4 + 4 + 4},
    {M5, 2},
    {M5, 2},
    {H1, 4 + 2},
    {M7, 2},
    {H1, 4},
    {H3, 4},
    {H3, 4 + 4 + 4 + 4},
    {M6, 2},
    {M7, 2},
    {H1, 4},
    {M7, 4},
    {H2, 2},
    {H2, 2},
    {H1, 4 + 2},
    {M5, 2 + 4 + 4},
    {H4, 4},
    {H3, 4},
    {H3, 4},
    {H1, 4},
    {H3, 4 + 4 + 4},
    {H3, 4},
    {H6, 4 + 4},
    {H5, 4},
    {H5, 4},
    {H3, 2},
    {H2, 2},
    {H1, 4 + 4},
    {P, 2},
    {H1, 2},
    {H2, 4},
    {H1, 2},
    {H2, 2},
    {H2, 4},
    {H5, 4},
    {H3, 4 + 4 + 4},
    {H3, 4},
    {H6, 4 + 4},
    {H5, 4 + 4},
    {H3, 2},
    {H2, 2},
    {H1, 4 + 4},
    {P, 2},
    {H1, 2},
    {H2, 4},
    {H1, 2},
    {H2, 2 + 4},
    {M7, 4},
    {M6, 4 + 4 + 4},
    {P, 4}};

struct YINFU YUEPU[128] = {
    {831, 300},
    {740, 300},
    {622, 300},
    {740, 300},
    {988, 300},
    {831, 150},
    {740, 150},
    {831, 400},
    {0, 300},
    {0, 300},
    {622, 300},
    {740, 300},
    {831, 300},
    {740, 300},
    {622, 300},
    {494, 150},
    {440, 150},
    {740, 150},
    {622, 150},
    {554, 400},
    {0, 300},
    {554, 300},
    {622, 300},
    {740, 300},
    {740, 300},
    {831, 300},
    {622, 150},
    {554, 150},
    {494, 400},
    {0, 300},
    {0, 300},
    {740, 300},
    {622, 300},
    {554, 150},
    {494, 150},
    {440, 150},
    {494, 150},
    {392, 500},
    {0, 300},
    {0, 300},
};
void midi_task(void *arg)
{
  int index = 0;
  int flag = 0;
  while (1)
  {
    setBeepTone(FreqTable[MUSIC[index].freq]);
    vTaskDelay((MUSIC[index].Duration * 125) / portTICK_PERIOD_MS);
    index += 1;
    if (index > 119)
    {
      flag += 1;
      index = 0;
    }
    if (flag == 2)
    {
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
      ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
      vTaskSuspend(NULL);
    }
  }
}

void app_main()
{
  init_blk();
  vTaskDelay(30 / portTICK_PERIOD_MS);
  setBackLightLevel(0);
  init_lcd();
  init_i2c();
  // i2c_scan_devices();
  init_axp173();
  init_bmp280();
  init_lsm6ds3();
  init_pcf8563();

  // vTaskDelay(3000 / portTICK_PERIOD_MS);

  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  //

  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  // setBackLight(3000, 0);
  // vTaskDelay(6000 / portTICK_PERIOD_MS);

  init_sdmmc();
  init_beep();
  // printf("%d", sizeof(YUEPU));
  //  vTaskDelay(6000 / portTICK_PERIOD_MS);
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  init_wifi();
  sntp_setlocaltime();

  // xTaskCreate(bat_task, "bat_tash", 1024 * 2, NULL, 2, NULL);
  // xTaskCreate(button_task, "button_task", 2048 * 2, NULL, 5, NULL);
  // xTaskCreate(midi_task, "midi_task", 1024 * 10, NULL, 2, NULL);
  // DATATIME datetime;
  // pcf8563_get_datetime(&datetime);
  // ESP_LOGI(TAG, "%d", datetime.year);

  while (1)
  {
    // lsm6ds3_get_accelerometer
    // lsm6ds3_get_accelerometer();
    // qmc5883l_raw_to_azimuth();
    // qmc5883l_azimuth_to_direction();
    // time(&now);

    // localtime_r(&now, &timeinfo);
    // strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    // ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
