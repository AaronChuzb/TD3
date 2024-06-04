/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-04 10:06:06
 */
#pragma once

#ifndef QMC5883L_H
#define QMC5883L_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "i2c.h"
#include <esp_log.h>

#define QMC_REG_ID  0x0D

typedef enum
{
  QMC5883L_MODE_STANDBY = 0, //!< Standby low power mode, no measurements 等待模式，低功耗，不测量
  QMC5883L_MODE_CONTINUOUS   //!< Continuous measurements  持续测量
} qmc5883l_mode_t;


/**
 * 输出数据速率
 */
typedef enum {
    QMC5883L_DR_10 = 0, //!< 10Hz
    QMC5883L_DR_50,     //!< 50Hz
    QMC5883L_DR_100,    //!< 100Hz
    QMC5883L_DR_200,    //!< 200Hz
} qmc5883l_odr_t;

/**
 * 过采样速率
 */
typedef enum {
    QMC5883L_OSR_64 = 0, //!< 64 samples
    QMC5883L_OSR_128,    //!< 128 samples
    QMC5883L_OSR_256,    //!< 256 samples
    QMC5883L_OSR_512,    //!< 512 samples
} qmc5883l_osr_t;


/**
 * 磁场范围
 */
typedef enum {
    QMC5883L_RNG_2 = 0,//!< -2G..+2G
    QMC5883L_RNG_8     //!< -8G..+8G
} qmc5883l_range_t;


/**
 * Raw measurement result
 */
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} qmc5883l_raw_data_t;

/**
 * Measurement result, milligauss
 */
typedef struct
{
    float x;
    float y;
    float z;
} qmc5883l_data_t;


esp_err_t qmc5883l_set_mode(qmc5883l_mode_t mode);
esp_err_t qmc5883l_get_mode();
esp_err_t qmc5883l_set_config(qmc5883l_mode_t mode, qmc5883l_odr_t odr, qmc5883l_osr_t osr, qmc5883l_range_t rng);
esp_err_t qmc5883l_get_raw_data(qmc5883l_raw_data_t *raw);
esp_err_t qmc5883l_raw_to_mg(qmc5883l_raw_data_t *raw, qmc5883l_data_t *data);
esp_err_t qmc5883l_get_data(qmc5883l_data_t *data);
esp_err_t qmc5883l_get_raw_temp(int16_t *temp);
float qmc5883l_raw_to_azimuth();
int qmc5883l_azimuth_smooth_output();
char *qmc5883l_azimuth_to_direction();
void init_qmc5883l();

 

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
