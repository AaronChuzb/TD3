
/*
 * @Date: 2024-01-31 16:12:07
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-08 13:15:53
 */
#include "i2c.h"

static const char *TAG = "I2C";
void init_i2c()
{
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

  esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
  // TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "I2C config returned error");
  if(ret != ESP_OK){
    ESP_LOGI(TAG, "I2C配置失败");
  }
  

  ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
  if(ret != ESP_OK){
    ESP_LOGI(TAG, "I2C安装失败");
  }
  // TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "I2C install returned error");
}

esp_err_t i2c_check_dev(uint8_t addr)
{
  esp_err_t ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1 | I2C_MASTER_WRITE), true);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100);
  if (ret == ESP_OK)
  {
    ESP_LOGI(TAG, "检测地址成功0x%02X", addr);
  } else {
    ESP_LOGI(TAG, "检测地址失败0x%02X", addr);
  }
  return ret;
}

/**
 * @description: 搜索I2C设备
 * @return {*}
 */
void i2c_scan_devices()
{
  printf("Scanning I2C bus...\n");
  for (int address = 0x03; address <= 0x77; address++)
  { // 扫描的I2C地址从0x03到0x77
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1 | I2C_MASTER_WRITE), true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100);
    if (ret == ESP_OK)
    { // 发送给当前地址成功，表示设备的地址是当前的地址
      printf("Device found at address 0x%02X\n", address);
    }
    i2c_cmd_link_delete(cmd);
    vTaskDelay(10);
  }

  printf("Scan end\n");
}

/**
 * @description: i2c发送1字节数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @param {uint8_t} data 数据
 * @return {*}
 */
esp_err_t i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t data)
{
  esp_err_t err;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, 1);
  if(reg != NULL){
    i2c_master_write_byte(cmd, reg, 1);
  }
  i2c_master_write_byte(cmd, data, 1);
  i2c_master_stop(cmd);
  err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  return err;
}

/**
 * @description: i2c发送多字节数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @param {uint8_t} data 数据
 * @return {*}
 */
esp_err_t i2c_write_data(uint8_t addr, const void *reg, const void *data, int dataLen)
{
  esp_err_t err;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, 1);
  if(reg != NULL){
    i2c_master_write_byte(cmd, (void *)reg, 1);
  }
  i2c_master_write(cmd, (void *)data, (size_t)dataLen, 1);
  i2c_master_stop(cmd);
  err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  return err;
}


esp_err_t i2c_read_data(uint8_t addr, uint8_t reg, uint8_t *buff, int size)
{
  esp_err_t ret;
  ret = i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, (size_t)size, 1000 / portTICK_PERIOD_MS);
  return ret;
}

/**
 * @description: i2c读取8位 1字节数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint8_t i2c_read_8bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[1];
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 1, 1000 / portTICK_PERIOD_MS);
  return buff[0];
}

/**
 * @description: i2c读取12位数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint16_t i2c_read_12bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[2] = {0};
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 2, 1000 / portTICK_PERIOD_MS);
  return (buff[0] << 4) + buff[1];
}


/**
 * @description: i2c读取13位数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint16_t i2c_read_13bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[2] = {0};
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 2, 1000 / portTICK_PERIOD_MS);
  return (buff[0] << 5) + buff[1];
}

/**
 * @description: i2c读取16位数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint16_t i2c_read_16bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[2] = {0};
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 2, 1000 / portTICK_PERIOD_MS);
  return (buff[0] << 8) + buff[1];
}

/**
 * @description: i2c读取24位数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint16_t i2c_read_24bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[4] = {0};
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 4, 1000 / portTICK_PERIOD_MS);
  return (buff[0] << 16) + (buff[1] << 8) + buff[2];
}

/**
 * @description: i2c读取32位数据
 * @param {uint8_t} addr 从机地址
 * @param {uint8_t} reg 寄存器地址
 * @return {*}
 */
uint16_t i2c_read_32bit(uint8_t addr, uint8_t reg)
{
  uint8_t buff[4] = {0};
  i2c_master_write_read_device(I2C_MASTER_NUM, addr, &reg, 1, buff, 4, 1000 / portTICK_PERIOD_MS);
  return (buff[0] << 24) + (buff[1] << 16) + (buff[2] << 8) + buff[3];
}