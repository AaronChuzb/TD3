/*
 * @Date: 2024-02-01 18:31:36
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-30 18:55:44
 */
#include "pcf8563.h"

static const char *TAG = "PCF8563";

void init_pcf8563()
{
  if (i2c_check_dev(REG_ID) != ESP_OK)
  {
    ESP_LOGI(TAG, "检测不到I2C地址0x51, PCF8563初始化失败");
  }
  static bool init = false;
  // if (!init)
  // {
  //   uint8_t tmp = 0b00000000;
  //   i2c_write_byte(REG_ID, 0x00, &tmp);
  //   mode &= 0b00010011;
  //   ret = PCF_Write(0x01, &mode, 1);
  //   if (ret != ESP_OK)
  //   {
  //     return -2;
  //   }
  //   init = true;
  // }
}

esp_err_t pcf8563_set_datetime(DATATIME *dateTime)
{
  if (dateTime->second >= 60 || dateTime->minute >= 60 || dateTime->hour >= 24 || dateTime->day > 32 || dateTime->weekday > 6 || dateTime->month > 12 || dateTime->year < 1900 || dateTime->year >= 2100)
  {
    return ESP_FAIL;
  }
  uint8_t buffer[7];
  buffer[0] = BinToBCD(dateTime->second) & 0x7F;
  buffer[1] = BinToBCD(dateTime->minute) & 0x7F;
  buffer[2] = BinToBCD(dateTime->hour) & 0x3F;
  buffer[3] = BinToBCD(dateTime->day) & 0x3F;
  buffer[4] = BinToBCD(dateTime->weekday) & 0x07;
  buffer[5] = BinToBCD(dateTime->month) & 0x1F;
  if (dateTime->year >= 2000)
  {
    buffer[5] |= 0x80;
    buffer[6] = BinToBCD(dateTime->year - 2000);
  }
  else
  {
    buffer[6] = BinToBCD(dateTime->year - 1900);
  }
  esp_err_t ret = i2c_write_data(REG_ID, 0x02, buffer, sizeof(buffer));
  return ret;
}

esp_err_t pcf8563_get_datetime(DATATIME *dateTime)
{
  uint8_t buffer[7];
  esp_err_t ret;
  ret = i2c_read_data(REG_ID, 0x02, buffer, sizeof(buffer));
  if (ret != ESP_OK)
  {
    return ret;
  }
  dateTime->second = (((buffer[0] >> 4) & 0x07) * 10) + (buffer[0] & 0x0F);
  dateTime->minute = (((buffer[1] >> 4) & 0x07) * 10) + (buffer[1] & 0x0F);
  dateTime->hour = (((buffer[2] >> 4) & 0x03) * 10) + (buffer[2] & 0x0F);
  dateTime->day = (((buffer[3] >> 4) & 0x03) * 10) + (buffer[3] & 0x0F);
  dateTime->weekday = (buffer[4] & 0x07);
  dateTime->month = ((buffer[5] >> 4) & 0x01) * 10 + (buffer[5] & 0x0F);
  dateTime->year = 1900 + ((buffer[6] >> 4) & 0x0F) * 10 + (buffer[6] & 0x0F);
  if (buffer[5] & 0x80)
  {
    dateTime->year += 100;
  }

  if (buffer[0] & 0x80) // Clock integrity not guaranted
  {
    return ESP_FAIL;
  }
  return ESP_OK;
}

