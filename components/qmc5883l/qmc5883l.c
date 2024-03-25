/*
 * @Date: 2024-02-01 15:45:05
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-20 21:32:05
 */
#include "qmc5883l.h"
#include <math.h>

uint8_t range = 0;

#define CHECK_ARG(VAL)            \
  do                              \
  {                               \
    if (!(VAL))                   \
      return ESP_ERR_INVALID_ARG; \
  } while (0)

#define PI 3.14159265358979323846

static const char *TAG = "QMC5883L";

/**
 * @description: 软重启
 * @return {*}
 */
esp_err_t qmc5883l_restart()
{
  esp_err_t ret;
  ret = i2c_write_byte(REG_ID, 0x0A, 0x80);
  return ret;
}

// 连续测量模式： 1和待命模式： 0
esp_err_t qmc5883l_set_mode(qmc5883l_mode_t mode)
{
  esp_err_t ret;
  uint8_t v = 0;
  ret = i2c_write_byte(REG_ID, 0x09, (v & 0xfe) | mode);
  ESP_LOGI(TAG, "设置模式：%s\n", ((v & 0xfe) | mode) == 1 ? "连续测量模式" : "待命模式");
  return ret;
}

esp_err_t qmc5883l_get_mode()
{
  esp_err_t ret;
  uint8_t buff[1];
  // buff = i2c_read_8bit(REG_ID, 0x09);
  ret = i2c_read_data(REG_ID, 0x09, buff, 1);
  // printf("测量模式：%d\n", buff[0] & 1);
  return ret;
}

esp_err_t qmc5883l_set_config(qmc5883l_mode_t mode, qmc5883l_odr_t odr, qmc5883l_osr_t osr, qmc5883l_range_t rng)
{
  esp_err_t ret;
  CHECK_ARG(odr <= QMC5883L_DR_200 && osr <= QMC5883L_OSR_512 && rng <= QMC5883L_RNG_8);
  uint8_t v = 0;
  range = rng;
  ret = i2c_write_byte(REG_ID, 0x0B, 0x01); // Define set/reset period
  ret = i2c_write_byte(REG_ID, 0x09, ((v & 0xfe) | mode) | ((odr & 3) << 2) | ((rng & 1) << 4) | ((osr & 3) << 6));
  ESP_LOGI(TAG, "配置输出数据速率：%s", (odr == QMC5883L_DR_10 ? "10Hz" : (odr == QMC5883L_DR_50 ? "50Hz": (odr == QMC5883L_DR_100 ? "100Hz": "200Hz"))));
  ESP_LOGI(TAG, "配置过采样率：%s", (osr == QMC5883L_OSR_64 ? "64 samples" : (osr == QMC5883L_OSR_128 ? "128 samples": (osr == QMC5883L_OSR_256 ? "256 samples": "512 samples"))));
  ESP_LOGI(TAG, "设置模式：%s", ((v & 0xfe) | mode) == 1 ? "连续测量模式" : "待命模式");
  ESP_LOGI(TAG, "配置磁场范围：%s", ((rng == QMC5883L_RNG_2) ? "-2G..+2G" : "-8G..+8G"));
  return ret;
}

/**
 * @description: 获取原始磁性数据
 * @param {qmc5883l_raw_data_t} *raw
 * @return {*}
 */
esp_err_t qmc5883l_get_raw_data(qmc5883l_raw_data_t *raw)
{
  CHECK_ARG(raw);
  esp_err_t ret = i2c_read_data(REG_ID, 0x00, raw, 6);
  if (ret != ESP_OK)
    ESP_LOGE(TAG, "无法读取数据 %d", ret);
  return ret;
}

/**
 * @description: 将原始磁性数据转换成磁场强度
 * @param {qmc5883l_raw_data_t} *raw
 * @param {qmc5883l_data_t} *data
 * @return {*}
 */
esp_err_t qmc5883l_raw_to_mg(qmc5883l_raw_data_t *raw, qmc5883l_data_t *data)
{
  CHECK_ARG(raw && data);
  float f = (range == QMC5883L_RNG_2 ? 2000.0 : 8000.0) / 32768;
  data->x = raw->x * f;
  data->y = raw->y * f;
  data->z = raw->z * f;
  return ESP_OK;
}

/**
 * @description: 获取磁场强度数据
 * @param {qmc5883l_data_t} *data
 * @return {*}
 */
esp_err_t qmc5883l_get_data(qmc5883l_data_t *data)
{
  qmc5883l_raw_data_t raw;
  qmc5883l_get_raw_data(&raw);
  return qmc5883l_raw_to_mg(&raw, data);
}

/**
 * @description: 计算方位角
 * @param {qmc5883l_raw_data_t} *raw
 * @return {*}
 */
float qmc5883l_raw_to_azimuth()
{
  qmc5883l_raw_data_t raw;
  qmc5883l_get_raw_data(&raw);
  float azimuth = atan2((int)raw.y, (int)raw.x) * 180 / PI;
  azimuth = azimuth < 0 ? 360 + azimuth : azimuth;
  ESP_LOGI(TAG, "方位角：%.1f(°)\n", azimuth);
  return azimuth;
}

/**
 * @description: 计算方向
 * @param {qmc5883l_raw_data_t} *raw
 * @return {*}
 */
char *qmc5883l_azimuth_to_direction()
{
  float azimuth = qmc5883l_raw_to_azimuth();
  char *directions[8] = {"北", "东北", "东", "东南", "南", "西南", "西", "西北"};
  int section = (int)(azimuth / 45.0);
  ESP_LOGI(TAG, "方向: %s\n", directions[section]);
  return directions[section];
}

int qmc5883l_azimuth_smooth_output()
{
  float a_list[5] = {0};
  qmc5883l_raw_data_t raw;
  for (int i = 0; i < 5; i++)
  {

    qmc5883l_get_raw_data(&raw);
    float azimuth = atan2((int)raw.y, (int)raw.x) * 180 / PI;
    azimuth = azimuth < 0 ? 360 + azimuth : azimuth;
    a_list[i] = azimuth;
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
  float min, max, sum = 0.0;
  int count = 0;
  min = max = a_list[0];
  for (int i = 0; i < 5; i++)
  {
    if (a_list[i] < min)
    {
      min = a_list[i];
    }
    if (a_list[i] > max)
    {
      max = a_list[i];
    }
  }
  for (int i = 0; i < 5; i++)
  {
    if (a_list[i] != min && a_list[i] != max)
    {
      sum += a_list[i];
      count++;
    }
  }
  int average = (int)(sum / count);
  // printf("A: %d\n", average);
  ESP_LOGI(TAG, "A: %d\n", average);
  return average;
}

void init_qmc5883l()
{
  // 多设备共用一组i2c设置个延时再去初始化
  vTaskDelay(50 / portTICK_PERIOD_MS);
  if (i2c_check_dev(REG_ID) != ESP_OK)
  {
    ESP_LOGI(TAG, "检测不到I2C地址0x0D, QMC5883L初始化失败");
  }
  else
  {
    qmc5883l_restart();
    qmc5883l_set_config(QMC5883L_MODE_CONTINUOUS, QMC5883L_DR_200, QMC5883L_OSR_512, QMC5883L_RNG_8);
    ESP_LOGI(TAG, "初始化QMC5883L成功");
    qmc5883l_raw_to_azimuth();
    qmc5883l_azimuth_to_direction();
    // ESP_LOGI(TAG, "方位角：%.1f(°)", qmc5883l_raw_to_azimuth());
    // ESP_LOGI(TAG, "方向: %s\n", qmc5883l_azimuth_to_direction());
  }
}