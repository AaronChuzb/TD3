/*
 * @Date: 2024-01-31 16:12:11
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-02-02 10:32:10
 */
#pragma once

#ifndef AXP173_H
#define AXP173_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "i2c.h"

  enum OUTPUT_CHANNEL
  {
    OP_DCDC1 = 0,
    OP_LDO4,
    OP_LDO2,
    OP_LDO3,
    OP_DCDC2,
  };
  enum ADC_CHANNEL
  {
    ADC_TS = 0,
    ADC_APS_V,
    ADC_VBUS_C,
    ADC_VBUS_V,
    ADC_ACIN_C,
    ADC_ACIN_V,
    ADC_BAT_C,
    ADC_BAT_V,
  };
  enum CHARGE_CURRENT
  {
    CHG_100mA = 0,
    CHG_190mA,
    CHG_280mA,
    CHG_360mA,
    CHG_450mA,
    CHG_550mA,
    CHG_630mA,
    CHG_700mA,
    CHG_780mA,
    CHG_880mA,
    CHG_960mA,
    CHG_1000mA,
    CHG_1080mA,
    CHG_1160mA,
    CHG_1240mA,
    CHG_1320mA,
  };
  enum COULOMETER_CTRL
  {
    COULOMETER_RESET = 5,
    COULOMETER_PAUSE,
    COULOMETER_ENABLE,
  };

  bool isACINExist();
  bool isACINAvl();
  bool isVBUSExist();
  bool isVBUSAvl();
  bool getBatCurrentDir();
  bool isAXP173OverTemp();
  bool isCharging();
  bool isBatExist();
  bool isChargeCsmaller();

  void setOutputEnable(enum OUTPUT_CHANNEL channel, bool state);
  void setOutputVoltage(enum OUTPUT_CHANNEL channel, uint16_t voltage);
  void powerOFF();
  void setChargeEnable(bool state);
  void setChargeCurrent(enum CHARGE_CURRENT current);
  void setADCEnable(enum ADC_CHANNEL channel, bool state);
  void setCoulometer(enum COULOMETER_CTRL option, bool state);
  float getCoulometerData();
  float getBatVoltage();
  float getBatLevelWithColumeter();
  float getBatCurrent();
  float getBatLevel();
  float getBatPower();
  float getVBUSVoltage();
  float getVBUSCurrent();
  float getAXP173Temp();
  float getTSTemp();
  void init_axp173();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
