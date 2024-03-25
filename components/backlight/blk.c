/*
 * @Date: 2024-02-01 18:31:36
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-20 22:58:35
 */
#include "blk.h"

ledc_channel_config_t ledc_channel[1];

void init_blk()
{
  ledc_timer_config_t ledc_timer = {
      .duty_resolution = LEDC_TIMER_13_BIT, // PWM占空比分辨率
      .freq_hz = 100,                       // PWM信号频率
      .speed_mode = LEDC_LOW_SPEED_MODE,           // 定时器模式
      .timer_num = LEDC_TIMER_0,           // 定时器序号
      .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
  };
  // Set configuration of timer0 for high speed channels
  ledc_timer_config(&ledc_timer);
  // 配置定时器0的高速通道
  ledc_channel_config_t ledc_channel[1] = {
      {.channel = LEDC_HS_CH0_CHANNEL,
       .duty = 0,
       .gpio_num = LEDC_HS_CH0_GPIO,
       .speed_mode = LEDC_LOW_SPEED_MODE,
       .hpoint = 0,
       .timer_sel = LEDC_TIMER_0}};

  // 配置背光控制器
  ledc_channel_config(&ledc_channel[0]);
  // 初始化淡入淡出服务
  ledc_fade_func_install(0); // 注册LEDC服务，在调用前使用，参数是作为是否允许中断
  // 配置LEDC定时器
  ledc_set_fade_with_time(ledc_channel[0].speed_mode,ledc_channel[0].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
  // 开始渐变
  ledc_fade_start(ledc_channel[0].speed_mode,ledc_channel[0].channel, LEDC_FADE_NO_WAIT);
}


/**
 * @description: 设置背光亮度
 * @param {int} time 渐变时间
 * @param {uint16_t} val 亮度(0~100)
 * @return {*}
 */
void setBackLight(int time, uint16_t val)
{
  uint32_t duty = 3000 / 100 * val;
  // 配置LEDC定时器
  ledc_set_fade_with_time(ledc_channel[0].speed_mode,ledc_channel[0].channel, duty, time);
  // 开始渐变
  ledc_fade_start(ledc_channel[0].speed_mode,ledc_channel[0].channel, LEDC_FADE_NO_WAIT);
}