/*
 * @Date: 2024-02-01 18:31:36
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-31 00:54:23
 */
#include "beep.h"

ledc_channel_config_t ledc_channel_beep[1];







void init_beep()
{
  ledc_timer_config_t ledc_timer = {
      .duty_resolution = LEDC_TIMER_13_BIT, // PWM占空比分辨率
      .freq_hz = 400,                       // PWM信号频率
      .speed_mode = LEDC_LOW_SPEED_MODE,           // 定时器模式
      .timer_num = LEDC_TIMER_1,           // 定时器序号
      .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
  };
  // Set configuration of timer0 for high speed channels
  ledc_timer_config(&ledc_timer);
  // 配置定时器0的高速通道
  ledc_channel_config_t ledc_channel_beep[1] = {
      {.channel = LEDC_CHANNEL_1,
       .duty = 0,
       .gpio_num = (48),
       .speed_mode = LEDC_LOW_SPEED_MODE,
       .hpoint = 0,
       .timer_sel = LEDC_TIMER_1}};

  // 配置蜂鸣器控制器
  ledc_channel_config(&ledc_channel_beep[0]);
  // // 初始化淡入淡出服务
  // ledc_fade_func_install(0); // 注册LEDC服务，在调用前使用，参数是作为是否允许中断
  // // 配置LEDC定时器
  // ledc_set_fade_with_time(ledc_channel_beep[0].speed_mode,ledc_channel_beep[0].channel, 4000, 0);
  // // 开始渐变
  // ledc_fade_start(ledc_channel_beep[0].speed_mode,ledc_channel_beep[0].channel, LEDC_FADE_NO_WAIT);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
  
}


/**
 * @description: 设置背光亮度
 * @param {int} time 渐变时间
 * @param {uint16_t} val 亮度(0~100)
 * @return {*}
 */
void setBeepTone(uint16_t freq)
{
  // uint32_t duty = 2800 / 100 * val;
  if(freq == 0) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
  } else {
     ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 2800);
     ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
     ledc_set_freq(ledc_channel_beep[0].speed_mode, LEDC_TIMER_1, freq);
  }
  
  // 配置LEDC定时器
  // ledc_set_fade_with_time(ledc_channel_beep[0].speed_mode,ledc_channel_beep[0].channel, 4000, 10);
  // // 开始渐变
  // ledc_fade_start(ledc_channel_beep[0].speed_mode,ledc_channel_beep[0].channel, LEDC_FADE_NO_WAIT);
  
}