/*
 * @Date: 2024-02-01 15:45:05
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-09 14:20:24
 */
#include "bmp280.h"
#include <math.h>

static const char *TAG = "BMP280";

struct BMP280Param
{
  uint16_t T1;
  int16_t T2;
  int16_t T3;
  uint16_t P1;
  int16_t P2;
  int16_t P3;
  int16_t P4;
  int16_t P5;
  int16_t P6;
  int16_t P7;
  int16_t P8;
  int16_t P9;
  int32_t adc_T;
  int32_t adc_P;
  int32_t t_fine;
} BMP280;

/**
 * @description: 根据数据手册公式计算实际温度
 * @return {*}
 */
float bmp_compute_temperature(void) //
{
  int32_t var1, var2, T;
  var1 = ((((BMP280.adc_T >> 3) - ((int32_t)BMP280.T1 << 1))) * ((int32_t)BMP280.T2)) >> 11;
  var2 = (((((BMP280.adc_T >> 4) - ((int32_t)BMP280.T1)) * ((BMP280.adc_T >> 4) - ((int32_t)BMP280.T1))) >> 12) * ((int32_t)BMP280.T3)) >> 14;
  BMP280.t_fine = var1 + var2;
  T = (BMP280.t_fine * 5 + 128) >> 8;
  return (float)T / 100;
}

/**
 * @description: 根据数据手册公式计算实际压强(需要先计算温度，需要温度作为参考值，在室内计算出来压强并不能实际作为数据参考，需要接近实际的数据请到户外，去除室内温度的影响)
 * @return {*}
 */
float bmp280_compute_pressure(void)
{
  bmp_compute_temperature();
  int64_t var1, var2, p;
  var1 = ((int64_t)BMP280.t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)BMP280.P6;
  var2 = var2 + ((var1 * (int64_t)BMP280.P5) << 17);
  var2 = var2 + (((int64_t)BMP280.P4) << 35);
  var1 = ((var1 * var1 * (int64_t)BMP280.P3) >> 8) + ((var1 * (int64_t)BMP280.P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)BMP280.P1) >> 33;
  if (var1 == 0)
  {
    return 0;
  }
  else
  {
    p = 1048576 - BMP280.adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)BMP280.P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)BMP280.P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)BMP280.P7) << 4);
    return (float)p / 256;
  }
}

/**
 * @description: BMP280软重启
 * @return {*}
 */
esp_err_t bmp280_restart()
{
  esp_err_t ret = i2c_write_byte(REG_ID, 0xE0, 0xB6);
  return ret;
}

/**
 * @description: 修正参数读取
 * @return {*}
 */
esp_err_t bmp280_read_param()
{
  esp_err_t ret;
  uint8_t buff[26] = {0};
  ret = i2c_read_data(REG_ID, 0x88, buff, 26);
  if (ret != ESP_OK)
  {
    ESP_LOGI(TAG, "读取修正参数失败");
    return ret;
  }
  BMP280.T1 = buff[0] | (buff[1] << 8);
  BMP280.T2 = buff[2] | (buff[3] << 8);
  BMP280.T3 = buff[4] | (buff[5] << 8);
  BMP280.P1 = buff[6] | (buff[7] << 8);
  BMP280.P2 = buff[8] | (buff[9] << 8);
  BMP280.P3 = buff[10] | (buff[11] << 8);
  BMP280.P4 = buff[12] | (buff[13] << 8);
  BMP280.P5 = buff[14] | (buff[15] << 8);
  BMP280.P6 = buff[16] | (buff[17] << 8);
  BMP280.P7 = buff[18] | (buff[19] << 8);
  BMP280.P8 = buff[20] | (buff[21] << 8);
  BMP280.P9 = buff[22] | (buff[23] << 8);
  ESP_LOGI(TAG, "读取修正参数成功");
  return ret;
}

/**
 * @description: 使能一次mode，进行新一次测量
 * @return {*}
 */
esp_err_t bmp280_measure_cmd()
{
  esp_err_t ret = i2c_write_byte(REG_ID, 0xF4, 0x56);
  return ret;
}

/**
 * @description: 读取BMP280的数据
 * @return {*}
 */
esp_err_t bmp280_read_data()
{
  bmp280_measure_cmd();
  esp_err_t ret;
  uint8_t buffT[3] = {0};
  uint8_t buffP[3] = {0};
  ret = i2c_read_data(REG_ID, 0xFA, buffT, 3);
  BMP280.adc_T = (buffT[0] << 12) | (buffT[1] << 4) | (buffT[0] >> 4);
  ret = i2c_read_data(REG_ID, 0xF7, buffP, 3);
  BMP280.adc_P = (buffP[0] << 12) | (buffP[1] << 4) | (buffP[0] >> 4);
  float temperature = bmp_compute_temperature();
  float pressure = bmp280_compute_pressure();
  float high = ((temperature + 273.15) / 0.0065) * (1 - pow((pressure / 101325), 0.19029));
  ESP_LOGI(TAG, "温度: %.1f(℃)", temperature);
  ESP_LOGI(TAG, "大气压强：%.1f(Pa)", pressure);
  ESP_LOGI(TAG, "海拔高度：%.1f(M)\n", high);
  return ret;
}

/**
 * @description: BMP280初始化(使用的是force mode)
 * @return {*}
 */
void init_bmp280()
{
  if (i2c_check_dev(REG_ID) != ESP_OK)
  {
    ESP_LOGI(TAG, "检测不到I2C地址0x76, BMP280初始化失败");
  }
  else
  {
    ESP_LOGI(TAG, "BMP280初始化成功");
    bmp280_restart();
    // vTaskDelay(200 / portTICK_PERIOD_MS);
    // 0x55： 0b01010101 16倍气压过采样，2倍温度过采样，force mode
    i2c_write_byte(REG_ID, 0xF4, 0x55);
    // 0x10： 0b00010000 使用force mode不关心采样间隔时间，全填0，滤波器使用16倍，不使用SPI模式不关心，写0
    i2c_write_byte(REG_ID, 0xF5, 0x10);
    bmp280_read_param();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    bmp280_read_data();
  }
}