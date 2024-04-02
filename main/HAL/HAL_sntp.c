/*
 * @Date: 2024-04-01 22:09:39
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-01 22:33:14
 */
#include "HAL.h"

void sntp_setlocaltime()
{
  init_sntp();
}

// 回调函数
static void time_sync_cb(struct timeval *tv)
{
  ESP_LOGI("SNTP", "The current time is %lld (secs)\n", tv->tv_sec);
  char strftime_buf[64];
  struct tm timeinfo;
  time_t now = 0;
  // 设置东八区时区
  setenv("TZ", "CST-8", 1);
  tzset();

  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  ESP_LOGI("SNTP", "The current date/time in Shanghai is: %s", strftime_buf);
}

void init_sntp()
{
  // ESP_LOGI(TAG, "Initializing SNTP");
  sntp_set_time_sync_notification_cb(time_sync_cb);
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "ntp1.aliyun.com");
  sntp_init();
}
