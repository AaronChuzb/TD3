/*
 * @Date: 2024-02-01 18:31:36
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-08 18:20:00
 */
#include "pcf8563.h"

static const char *TAG = "PCF8563";

esp_err_t pcf8563_reset()
{
  uint8_t data[2];
  data[0] = 0;
  data[1] = 0;
  i2c_write_byte(REG_ID, 0x00, 0);
  i2c_write_byte(REG_ID, 0x01, 0);
  return ESP_OK;
}

void init_pcf8563()
{
  if (i2c_check_dev(REG_ID) != ESP_OK)
  {
    ESP_LOGI(TAG, "检测不到I2C地址0x51, PCF8563初始化失败");
  }
  // 获取时钟芯片时间
  struct tm timechip;
  pcf8563_get_time(&timechip);
}

uint8_t bcd2dec(uint8_t val)
{
  return (val >> 4) * 10 + (val & 0x0f);
}

uint8_t dec2bcd(uint8_t val)
{
  return ((val / 10) << 4) + (val % 10);
}

esp_err_t pcf8563_set_time(struct tm *time)
{
  uint8_t data[7];
  data[0] = dec2bcd(time->tm_sec);
  data[1] = dec2bcd(time->tm_min);
  data[2] = dec2bcd(time->tm_hour);
  data[3] = dec2bcd(time->tm_mday);
  data[4] = dec2bcd(time->tm_wday);    // tm_wday is 0 to 6
  data[5] = dec2bcd(time->tm_mon); // tm_mon is 0 to 11
  data[6] = dec2bcd(time->tm_year);
  esp_err_t err = i2c_write_data(REG_ID, 0x02, data, sizeof(data));
  // printf("write status %d\n", err);
  return err;
}

esp_err_t pcf8563_get_time(struct tm *time)
{

  uint8_t data[7];

  /* read time */
  esp_err_t res = i2c_read_data(REG_ID, 0x02, data, sizeof(data));
  if (res != ESP_OK)
    return res;
  // printf("status %d\n", res);
  // /* convert to unix time structure */
  // ESP_LOGI("", "data=%02x %02x %02x %02x %02x %02x %02x",
  //          data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
  time->tm_sec = bcd2dec(data[0] & 0x7F);
  time->tm_min = bcd2dec(data[1] & 0x7F);
  time->tm_hour = bcd2dec(data[2] & 0x3F);
  time->tm_mday = bcd2dec(data[3] & 0x3F);
  time->tm_wday = bcd2dec(data[4] & 0x07);    // tm_wday is 0 to 6
  time->tm_mon = bcd2dec(data[5] & 0x1F); // tm_mon is 0 to 11
  time->tm_year = bcd2dec(data[6]) + 1900;
  time->tm_isdst = 0;
  // ESP_LOGI(TAG, "读取片内时钟：%d-%d-%d %d:%d:%d", time->tm_year, time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
  return ESP_OK;
}



