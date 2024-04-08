#include "HAL.h"


void HAL_init() {
  init_blk();
  init_lcd();
  // 由于需要挂载字库到PSRAM所以先初始化SDMMC
  init_sdmmc();
  init_lvgl_port();
  
  init_i2c();
  // i2c_scan_devices();
  init_axp173();
  init_bmp280();
  init_lsm6ds3();
  init_pcf8563();
//    DATETIME set_time;
//   set_time.year = 124 + 1900;
//   set_time.month = 3;
//   set_time.day = 8;
//   set_time.hour = 11;
//   set_time.weekday = 1;
//   set_time.minute = 48;
//   set_time.second = 20;
//   // 更新芯片时间
//   pcf8563_set_datetime(&set_time);
// vTaskDelay(3000 / portTICK_PERIOD_MS);
//  DATETIME pcf8563_time;
//   pcf8563_get_datetime(&pcf8563_time);
//   char time_str[30];
//   sprintf(time_str, "%d-%d-%d %d:%d:%d", pcf8563_time.year, pcf8563_time.month + 1, pcf8563_time.day, pcf8563_time.hour, pcf8563_time.minute, pcf8563_time.second);
//   ESP_LOGI("SNTP", "The current date/time in pcf8563 is: %s", time_str);
  // setBackLightLevel(10);
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  //  setBackLightLevel(2);
  //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  //  setBackLightLevel(0);
  //  vTaskDelay(3000 / portTICK_PERIOD_MS);
  //  setBackLightLevel(5);

  
  // init_beep();
  button_init();
  // printf("%d", sizeof(YUEPU));
  //  vTaskDelay(6000 / portTICK_PERIOD_MS);
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  // init_wifi();
  // sntp_setlocaltime();
  // 获取剩余内存大小
  int freeHeap = esp_get_free_heap_size();
  int psramFreeH = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  int inside = (freeHeap - psramFreeH) / 1024;
  // // 打印剩余内存大小
  printf("Free heap: %d bytes\n", freeHeap);
  // 获取PSRAM的剩余内存大小
  printf("PSRAM free size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  ESP_LOGI("HAL", "内部内存剩余: %d Kbytes", inside);
  

}