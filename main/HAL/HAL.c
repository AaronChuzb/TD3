#include "HAL.h"

static const char *TAG = "HAL";

void get_sram_size()
{
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



void HAL_init()
{
  // init_blk();
  // init_uarts();
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

  // init_beep();
  button_init();

  //  vTaskDelay(6000 / portTICK_PERIOD_MS);
  // vTaskDelay(3000 / portTICK_PERIOD_MS);
  init_wifi();
  
  // sntp_setlocaltime();
  // hal_ble_init();

  uint8_t mac[6];
  // 获取MAC地址
  esp_err_t err = esp_read_mac(mac, ESP_IF_WIFI_STA);

  if (err == ESP_OK)
  {
    ESP_LOGI(TAG, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }
  else
  {
    ESP_LOGE(TAG, "Failed to get MAC address");
  }
  // 示例 MAC 地址
  char macAddress[18] = {};
  sprintf(macAddress, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  lv_msg_send(MSG_MAC_SET, macAddress);

  init_falsh();
  get_sram_size();

}

void Pre_HAL_Init()
{
  
}