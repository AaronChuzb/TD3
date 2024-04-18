#include "HAL.h"


void HAL_init() {
  init_blk();
  init_lcd();
  // 由于需要挂载字库到PSRAM所以先初始化SDMMC
  init_sdmmc();
  init_lvgl_port();
  
  init_i2c();
  init_bmp280();
  init_axp173();
  
  init_lsm6ds3();
  init_pcf8563();

  init_beep();
  button_init();

  //  vTaskDelay(6000 / portTICK_PERIOD_MS);
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  init_wifi();
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
  // init_tone();
  

}