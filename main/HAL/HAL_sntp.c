/*
 * @Date: 2024-04-01 22:09:39
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-08 13:19:19
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
  // 将对时的时间存入pcf8563时钟芯片


  printf("%d\n", timeinfo.tm_year);
  pcf8563_set_time(&timeinfo);
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  struct tm timechip;
  pcf8563_get_time(&timechip);
  char time_str[30];
  sprintf(time_str, "%d-%d-%d %d:%d:%d", timechip.tm_year, timechip.tm_mon, timechip.tm_mday, timechip.tm_hour, timechip.tm_min, timechip.tm_sec);
  ESP_LOGI("SNTP", "The current date/time in pcf8563 is: %s", time_str);
  
}

void init_sntp()
{
  // ESP_LOGI(TAG, "Initializing SNTP");
  sntp_set_time_sync_notification_cb(time_sync_cb);
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "ntp1.aliyun.com");
  sntp_init();
 
}
