/*
 * @Date: 2024-03-09 19:37:00
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-10 22:52:26
 */
#include "HAL.h"

static const char *TAG = "SDMMC";

sdmmc_card_t *card;

bool is_sd_present()
{
  return (gpio_get_level(SDMMC_PIN_CD) == 0);
}

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

void list_directory(const char *path)
{
  FF_DIR dir;
  FILINFO fno;
  FRESULT res;

  res = f_opendir(&dir, path);
  if (res != FR_OK)
  {
    printf("打开目录失败. Error: %d\n", res);
    return;
  }

  printf("列出目录中的文件 '%s':\n", path);
  for (;;)
  {
    res = f_readdir(&dir, &fno);
    if (res != FR_OK || fno.fname[0] == 0)
    {
      break;
    }
    if (fno.fname[0] == '.')
    {
      continue;
    }
    printf("  %s\n", fno.fname);
  }
  f_closedir(&dir);
}

void init_sdmmc()
{
  // 初始化cd引脚，检查sd卡是否插入
  gpio_config_t io_conf;
  // 配置GPIO的模式为输入模式
  io_conf.mode = GPIO_MODE_INPUT;
  // 配置GPIO的引脚位
  io_conf.pin_bit_mask = (1ULL << SDMMC_PIN_CD);
  // 配置GPIO的中断模式
  io_conf.intr_type = GPIO_INTR_DISABLE;
  // 设置为默认值
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  // 配置GPIO
  gpio_config(&io_conf);

  esp_err_t ret;
  if (is_sd_present())
  {
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4;
    slot_config.cd = (GPIO_NUM_21);
    slot_config.clk = SDMMC_PIN_CLK;
    slot_config.cmd = SDMMC_PIN_CMD;
    slot_config.d0 = SDMMC_PIN_D0;
    slot_config.d1 = SDMMC_PIN_D1;
    slot_config.d2 = SDMMC_PIN_D2;
    slot_config.d3 = SDMMC_PIN_D3;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    ESP_LOGI(TAG, "SDIO初始化成功");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {

      ESP_LOGE(TAG, "初始化失败");
    }
    ESP_LOGI(TAG, "文件系统挂载成功");

    sdmmc_card_print_info(stdout, card);
    sd_test();
    // 列出所有文件
    // list_directory("/");
  }
}