/*
 * @Date: 2024-02-01 15:45:05
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-11 21:27:50
 */
/*
 * @Date: 2024-01-31 16:12:07
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-02-01 15:43:34
 */
#include "axp173.h"

static const char *TAG = "AXP173";

static float charger_level = 0.0;
static float discharge_level = 100.0;

inline uint16_t _getMin(uint16_t a, uint16_t b)
{
  return ((a) < (b) ? (a) : (b));
}

inline uint16_t _getMax(uint16_t a, uint16_t b)
{
  return ((a) > (b) ? (a) : (b));
}


uint16_t _getMid(uint16_t input, uint16_t min, uint16_t max)
{
  return _getMax(_getMin(input, max), min);
}


inline float _getMinFloat(float a, float b)
{
  return ((a) < (b) ? (a) : (b));
}

inline float _getMaxFloat(float a, float b)
{
  return ((a) > (b) ? (a) : (b));
}

bool isACINExist()
{
  return (i2c_read_8bit(0x34, 0x00) & 0B10000000) ? true : false;
}

bool isACINAvl()
{
  return (i2c_read_8bit(0x34, 0x00) & 0B01000000) ? true : false;
}

bool isVBUSExist()
{
  return (i2c_read_8bit(0x34, 0x00) & 0B00100000) ? true : false;
}

bool isVBUSAvl()
{
  return (i2c_read_8bit(0x34, 0x00) & 0B00010000) ? true : false;
}

bool getBatCurrentDir()
{
  return (i2c_read_8bit(0x34, 0x00) & 0B00000100) ? true : false;
}

bool isAXP173OverTemp()
{
  return (i2c_read_8bit(0x34, 0x01) & 0B10000000) ? true : false;
}

bool isCharging()
{
  return (i2c_read_8bit(0x34, 0x01) & 0B01000000) ? true : false;
}

bool isBatExist()
{
  return (i2c_read_8bit(0x34, 0x01) & 0B00100000) ? true : false;
}

bool isChargeCsmaller()
{
  return (i2c_read_8bit(0x34, 0x01) & 0B00000100) ? true : false;
}

/**
 * @description: 打开或者关闭输出通道(测试通过)
 * @param {enum OUTPUT_CHANNEL} channel 输出通道
 * @param {bool} state 状态：true:打开 false: 关闭
 * @return {*}
 */
void setOutputEnable(enum OUTPUT_CHANNEL channel, bool state)
{
  uint8_t buff = i2c_read_8bit(0x34, 0x12);
  buff = state ? (buff | (1U << channel)) : (buff & ~(1U << channel));
  i2c_write_byte(0x34, 0x12, buff);
}

/**
 * @description: 设置通道的输出电压
 * @param {enum OUTPUT_CHANNEL} channel 输出通道
 * @param {uint16_t} voltage 输出电压  DCDC1 & LDO4: 700~3500(mV), DCDC2: 700~2275(mV), LDO2 & LDO3: 1800~3300{mV}
 * @return {*}
 */
void setOutputVoltage(enum OUTPUT_CHANNEL channel, uint16_t voltage)
{
  uint8_t buff = 0;
  uint16_t data[2] = {0};
  switch (channel)
  {
  case OP_DCDC1:
    voltage = (_getMid(voltage, 700, 3500) - 700) / 25;
    buff = i2c_read_8bit(0x34, 0x26);
    buff = (buff & 0B10000000) | (voltage & 0B01111111);
    i2c_write_byte(0x34, 0x26, buff);
    break;
  case OP_DCDC2:
    voltage = (_getMid(voltage, 700, 2275) - 700) / 25;
    buff = i2c_read_8bit(0x34, 0x23);
    buff = (buff & 0B11000000) | (voltage & 0B00111111);
    i2c_write_byte(0x34, 0x23, buff);
    break;
  case OP_LDO2:
    voltage = (_getMid(voltage, 1800, 3300) - 1800) / 100;
    buff = i2c_read_8bit(0x34, 0x28);
    buff = (buff & 0B00001111) | (voltage << 4);
    i2c_write_byte(0x34, 0x28, buff);
    break;
  case OP_LDO3:
    voltage = (_getMid(voltage, 1800, 3300) - 1800) / 100;
    buff = i2c_read_8bit(0x34, 0x28);
    buff = (buff & 0B11110000) | (voltage);
    i2c_write_byte(0x34, 0x28, buff);
    break;
  case OP_LDO4:
    voltage = (_getMid(voltage, 700, 3500) - 700) / 25;
    buff = i2c_read_8bit(0x34, 0x27);
    buff = (buff & 0B10000000) | (voltage & 0B01111111);
    i2c_write_byte(0x34, 0x27, buff);
    break;
  default:
    break;
  }
}

void powerOFF()
{
  i2c_write_byte(0x34, 0x32, (i2c_read_8bit(0x34, 0x32) | 0B10000000));
}

void setChargeEnable(bool state)
{
  if (state)
    i2c_write_byte(0x34, 0x33, ((i2c_read_8bit(0x34, 0x33) | 0B10000000)));
  else
    i2c_write_byte(0x34, 0x33, ((i2c_read_8bit(0x34, 0x33) & 0B01111111)));
}

void setChargeCurrent(enum CHARGE_CURRENT current)
{
  i2c_write_byte(0x34, 0x33, ((i2c_read_8bit(0x34, 0x33) & 0B11110000) | current));
}


void setADCEnable(enum ADC_CHANNEL channel, bool state)
{
  uint8_t buff = i2c_read_8bit(0x34, 0x82);
  buff = state ? (buff | (1U << channel)) : (buff & ~(1U << channel));
  i2c_write_byte(0x34, 0x82, buff);
}

void setCoulometer(enum COULOMETER_CTRL option, bool state)
{
  uint8_t buff = i2c_read_8bit(0x34, 0xB8);
  buff = state ? (buff | (1U << option)) : (buff & ~(1U << option));
  i2c_write_byte(0x34, 0xB8, buff);
}

 uint32_t getCoulometerChargeData()
{
  return i2c_read_32bit(0x34, 0xB0);
}

 uint32_t getCoulometerDischargeData()
{
  return i2c_read_32bit(0x34, 0xB4);
}

float getCoulometerData()
{
  uint32_t coin = getCoulometerChargeData();
  uint32_t coout = getCoulometerDischargeData();
  // data = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
  return 65536 * 0.5 * (int32_t)(coin - coout) / 3600.0 / 25.0;
}

float getBatVoltage()
{
  float ADCLSB = 1.1 / 1000.0;
  return i2c_read_12bit(0x34, 0x78) * ADCLSB;
}

float getBatCurrent()
{
  float ADCLSB = 0.5;
  uint16_t CurrentIn = i2c_read_13bit(0x34, 0x7A);
  uint16_t CurrentOut = i2c_read_13bit(0x34, 0x7C);
  return (CurrentIn - CurrentOut) * ADCLSB;
}

float getBatLevel()
{
  const float batVoltage = getBatVoltage();
  const float batPercentage = (batVoltage < 3.248088) ? (0) : (batVoltage - 3.120712) * 100;
  // 防止电量上下跳变
  float percentage = (batPercentage <= 100.00) ? batPercentage : 100.00;
  if(isCharging()){
    charger_level = _getMaxFloat(percentage, charger_level);
    percentage = charger_level;
  } else {
    discharge_level = _getMinFloat(percentage, discharge_level);
    percentage = discharge_level;
  }
  return percentage;
}

float getBatPower()
{
  float VoltageLSB = 1.1;
  float CurrentLCS = 0.5;
  uint32_t ReData = i2c_read_24bit(0x34, 0x70);
  return VoltageLSB * CurrentLCS * ReData / 1000.0;
}

float getVBUSVoltage()
{
  float ADCLSB = 1.7 / 1000.0;
  uint16_t ReData = i2c_read_12bit(0x34, 0x5A);
  return ReData * ADCLSB;
}

float getVBUSCurrent()
{
  float ADCLSB = 0.375;
  uint16_t ReData = i2c_read_12bit(0x34, 0x5C);
  return ReData * ADCLSB;
}

float getAXP173Temp()
{
  float ADCLSB = 0.1;
  const float OFFSET_DEG_C = -144.7;
  uint16_t ReData = i2c_read_12bit(0x34, 0x5E);
  return OFFSET_DEG_C + ReData * ADCLSB;
}

float getTSTemp()
{
  float ADCLSB = 0.1;
  const float OFFSET_DEG_C = -144.7;
  uint16_t ReData = i2c_read_12bit(0x34, 0x62);
  return OFFSET_DEG_C + ReData * ADCLSB;
}


void init_axp173()
{
  // 多设备共用一组i2c设置个延时再去初始化
  // vTaskDelay(50 / portTICK_PERIOD_MS);
  if(i2c_check_dev(0x34) != ESP_OK){
    ESP_LOGI(TAG, "检测不到I2C地址0x34, AXP173初始化失败");
  } else {
    ESP_LOGI(TAG, "AXP173初始化成功");
    setADCEnable(ADC_VBUS_V, true);
    setADCEnable(ADC_VBUS_C, true);
    setADCEnable(ADC_BAT_V, true);
    setADCEnable(ADC_BAT_C, true);
    setCoulometer(COULOMETER_ENABLE, true);
    // 设置280mA充电电流
    setChargeCurrent(CHG_360mA);
    // 加上负载会有0.2V左右的压降，所以提高输出
    // setOutputVoltage(OP_DCDC1, 3100);
    ESP_LOGI(TAG, "电池电压：%.1f(V)", getBatVoltage());
    ESP_LOGI(TAG, "剩余电量：%.1f(%%)", getBatLevel());
    ESP_LOGI(TAG, "电流消耗：%.1f(mA)", getBatCurrent());
    ESP_LOGI(TAG, "库仑计：%.1f(mAh)\n", getCoulometerData());
  }
}