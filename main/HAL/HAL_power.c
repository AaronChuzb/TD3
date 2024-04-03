/*
 * @Date: 2024-04-03 16:02:58
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-03 16:04:12
 */
#include "HAL.h"

/**
 * 初始化电源节能配置
 * 该函数用于初始化并配置ESP32的电源管理模式，以实现节能效果。
 * 
 * 参数: 无
 * 返回值: 无
 */

void power_save_init(void)
{
  // 配置电源管理设置
  esp_pm_config_t pm_config = {
      .max_freq_mhz = 160,  // 设置CPU的最大频率为160MHz
      .min_freq_mhz = 80,   // 设置CPU的最小频率为80MHz
      .light_sleep_enable = true  // 启用轻量级睡眠模式
  };
  
  // 应用电源管理配置
  ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
}
