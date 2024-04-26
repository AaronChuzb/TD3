
#include "HAL.h"

static const char *TAG = "FLASH";

// Mount path for the partition
static const char *base_path = "/spifats";

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
void init_falsh(void)
{
  ESP_LOGI(TAG, "Mounting FAT filesystem");
  // To mount device we need name of device partition, define base_path
  // and allow format partition in case if it is new one and was not formatted before
  const esp_vfs_fat_mount_config_t mount_config = {
      .max_files = 4,
      .format_if_mount_failed = false,
      .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
  };
  esp_err_t err;
  err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, "storage", &mount_config, &s_wl_handle);

  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
  }

  // DIR *dir;
  // struct dirent *entry;

  // dir = opendir("/spifats");
  // if (dir != NULL)
  // {
  //   while ((entry = readdir(dir)) != NULL)
  //   {
  //     if (entry->d_type != DT_DIR)
  //     {
  //       ESP_LOGI(TAG, "/spifats/%s", entry->d_name);
  //     }
  //   }
  // };

  const char *file_key = "/spifats/key.txt";
  ESP_LOGI(TAG, "打开文件 %s", file_key);
  FILE *file = fopen(file_key, "r");
  // file = fopen(file_hello, "r");
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

esp_err_t spifats_unmount(void)
{
  return esp_vfs_fat_spiflash_unmount_rw_wl(base_path, s_wl_handle);
}
