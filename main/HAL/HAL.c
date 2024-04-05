#include "HAL.h"


void HAL_init() {
  init_blk();
  init_sdmmc();
  init_lcd();
  init_lvgl_port();
  
  init_i2c();
  // i2c_scan_devices();
  init_axp173();
  init_bmp280();
  init_lsm6ds3();
  init_pcf8563();
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
  init_wifi();
  sntp_setlocaltime();
  // 获取剩余内存大小
  int freeHeap = esp_get_free_heap_size();

  // 打印剩余内存大小
  printf("Free heap: %d bytes\n", freeHeap);
  // 获取PSRAM的剩余内存大小
  printf("PSRAM free size: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}