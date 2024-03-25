/*
 * @Date: 2024-02-01 15:44:58
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-18 22:38:57
 */
/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-02-01 15:31:57
 */
#pragma once

#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO 39      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 38      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 400000 /*!< I2C master clock frequency */

void init_i2c(void);
esp_err_t i2c_check_dev(uint8_t addr);
esp_err_t i2c_write_byte(uint8_t addr, uint8_t reg, uint8_t data);
esp_err_t i2c_write_data(uint8_t addr, uint8_t reg, uint8_t data, int dataLen);
esp_err_t i2c_read_data(uint8_t addr, uint8_t reg, uint8_t *buff, int size);
uint8_t i2c_read_8bit(uint8_t addr, uint8_t reg);
uint16_t i2c_read_12bit(uint8_t addr, uint8_t reg);
uint16_t i2c_read_13bit(uint8_t addr, uint8_t reg);
uint16_t i2c_read_16bit(uint8_t addr, uint8_t reg);
uint16_t i2c_read_24bit(uint8_t addr, uint8_t reg);
uint16_t i2c_read_32bit(uint8_t addr, uint8_t reg);
void i2c_scan_devices(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

