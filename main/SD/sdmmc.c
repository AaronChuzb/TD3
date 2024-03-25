/*
 * @Date: 2024-03-09 19:37:00
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-23 22:30:27
 */
#include "sdmmc.h"

static const char *TAG = "SDMMC";

// #define SOC_SDMMC_USE_GPIO_MATRIX

sdmmc_card_t *card;

void sd_test(void)
{

  const char *file_hello = MOUNT_POINT "/hello.txt";

  ESP_LOGI(TAG, "打开文件 %s", file_hello);
  FILE *file = fopen(file_hello, "w");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "创建文件失败");
    return;
  }
  fprintf(file, "Helloword！\n");
  fclose(file);
  file = fopen(file_hello, "r");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "打开失败");
    return;
  }
  char line[64];
  fgets(line, sizeof(line), file);
  fclose(file);

  char *pos = strchr(line, '\n');
  if (pos)
  {
    *pos = '\0';
  }
  ESP_LOGI(TAG, "读出内容: '%s'", line);
}

void init_sdmmc()
{
  esp_err_t ret;

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
      .format_if_mount_failed = true,
#else
      .format_if_mount_failed = false,
#endif
      .max_files = 5,
      .allocation_unit_size = 16 * 1024};
  const char mount_point[] = MOUNT_POINT;
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
  slot_config.width = 4;
  slot_config.cd = (GPIO_NUM_21);
#ifdef SOC_SDMMC_USE_GPIO_MATRIX
  slot_config.clk = CONFIG_EXAMPLE_PIN_CLK;
  slot_config.cmd = CONFIG_EXAMPLE_PIN_CMD;
  slot_config.d0 = CONFIG_EXAMPLE_PIN_D0;
  slot_config.d1 = CONFIG_EXAMPLE_PIN_D1;
  slot_config.d2 = CONFIG_EXAMPLE_PIN_D2;
  slot_config.d3 = CONFIG_EXAMPLE_PIN_D3;
#endif
  slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
  ESP_LOGI(TAG, "SDIO初始化成功");
  ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
  if (ret != ESP_OK)
  {

    ESP_LOGE(TAG, "初始化失败");
  }
  ESP_LOGI(TAG, "文件系统挂载成功");

  sdmmc_card_print_info(stdout, card);
  // while (1)
  // {
  //   /* code */
  //   sd_test();
  //   vTaskDelay(1000 / portTICK_PERIOD_MS);
  // }
  sd_test();
  
}