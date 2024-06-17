/*
 * @Date: 2024-02-01 15:45:05
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-17 16:56:50
 */
#include "lsm6ds3.h"
#include <math.h>

static const char *TAG = "LSM6DS3";
float const PI = 3.14159265F;

gyro_data_t gyro_data;

/**
 * @description: 获取芯片id
 * @return {*}
 */
esp_err_t lsm6ds3_get_id()
{
  uint8_t buff[1];
  esp_err_t ret = i2c_read_data(LSM_REG_ID, 0x0f, buff, 1);
  ESP_LOGI(TAG, "IC ID: %.2x", buff[0]);
  return ret;
}

esp_err_t lsm6ds3_get_temp()
{
  float temperature;
  uint8_t data[2];
  esp_err_t ret = i2c_read_data(LSM_REG_ID, 0x20, data, 2);
  temperature = (float)(((int16_t)(data[1] << 8) | data[0]) * 0.0625) + 25.0;
  // ESP_LOGI(TAG, "补偿温度: %.1f", temperature);
  gyro_data.temp = temperature;
  return ret;
}

esp_err_t lsm6ds3_get_gyroscope()
{
  float raddeg = 180 / PI;
  float g_fact = 4.375e-3 * raddeg;
  float gx, gy, gz;
  uint8_t data[6];
  esp_err_t ret = i2c_read_data(LSM_REG_ID, 0x22, data, 6);
  gx = (float)(((int16_t)(data[1] << 8) | data[0]) * g_fact);
  gy = (float)(((int16_t)(data[3] << 8) | data[2]) * g_fact);
  gz = (float)(((int16_t)(data[5] << 8) | data[4]) * g_fact);
  // ESP_LOGI(TAG, "数据: gx: %.1f, gy: %.1f, gz: %.1f", gx, gy, gz);
  gyro_data.gx = gx;
  gyro_data.gy = gy;
  gyro_data.gz = gz;
  return ret;
}

esp_err_t lsm6ds3_get_accelerometer()
{
  float x_fact = 0.00006103515625;
  float ax, ay, az;
  uint8_t data[6];
  esp_err_t ret = i2c_read_data(LSM_REG_ID, 0x28, data, 6);
  ax = (float)(((int16_t)(data[1] << 8) | data[0]) * x_fact);
  ay = (float)(((int16_t)(data[3] << 8) | data[2]) * x_fact);
  az = (float)(((int16_t)(data[5] << 8) | data[4]) * x_fact);
  // ESP_LOGI(TAG, "数据: ax: %.1f, ay: %.1f, az: %.1f", ax, ay, az);

  gyro_data.ax = ax;
  gyro_data.ay = ay;
  gyro_data.az = az;
  return ret;
}

void lsm6ds3_get_orientation()
{
  float roll, pitch, yaw;
  roll = (float)atan2(gyro_data.ay, gyro_data.az);
  if (gyro_data.ay * sin(roll) + gyro_data.az * cos(roll) == 0)
  {
    pitch = gyro_data.ax > 0 ? (PI / 2) : (-PI / 2);
  }
  else
  {
    pitch = (float)atan(-gyro_data.ax / (gyro_data.ay * sin(roll) + gyro_data.az * cos(roll)));
  }
  roll 	 = - roll * 180.0 / PI;
	pitch 	 =   pitch * 180.0 / PI;
  gyro_data.roll = roll;
  gyro_data.pitch = pitch;
}

gyro_data_t lsm6ds3_get_all_data()
{
  lsm6ds3_get_temp();
  lsm6ds3_get_gyroscope();
  lsm6ds3_get_accelerometer();
  lsm6ds3_get_orientation();
  return gyro_data;
}

/**
 * @description: 初始化
 * @return {*}
 */
void init_lsm6ds3()
{
  // 多设备共用一组i2c设置个延时再去初始化
  // vTaskDelay(100 / portTICK_PERIOD_MS);
  if (i2c_check_dev(LSM_REG_ID) != ESP_OK)
  {
    ESP_LOGI(TAG, "检测不到I2C地址0x6B, LSM6DS3初始化失败");
  }
  else
  {
    ESP_LOGI(TAG, "LSM6DS3初始化成功");
    lsm6ds3_get_id();
    // Enable embedded functions register (r/w).  This bit must be set to ‘0’ for the correct operation of the device.
    i2c_write_byte(LSM_REG_ID, 0x01, 0x00);
    // FIFO control register (r/w). This bit must be set to ‘0’ for the correct operation of the device.
    i2c_write_byte(LSM_REG_ID, 0x0A, 0x00);

    uint8_t CTRL9_val = (1 << 3) | (1 << 4) | (1 << 5);
    i2c_write_byte(LSM_REG_ID, 0x18, CTRL9_val);

    i2c_write_byte(LSM_REG_ID, 0x15, 0x00);

    i2c_write_byte(LSM_REG_ID, 0x10, 0x40);
    i2c_write_byte(LSM_REG_ID, 0x11, 0x40);
  }
}