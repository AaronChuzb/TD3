/*
 * @Date: 2024-02-01 18:31:36
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-03-30 18:21:35
 */
#include "blk.h"

static const char *TAG = "BACKLIGHT";


int clamp(int value, int min, int max)
{
  return (value < min) ? min : (value > max) ? max
                                             : value;
}

int current = 0;

void init_blk()
{
  gpio_config_t io_conf;
  // 禁用中断
  io_conf.intr_type = GPIO_INTR_DISABLE;
  // 设置为输出模式
  io_conf.mode = GPIO_MODE_OUTPUT;
  // 设置引脚
  io_conf.pin_bit_mask = (1ULL << BACKLIGHT_GPIO);
  // 设置为默认电平（可选）
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&io_conf);
  gpio_set_level(BACKLIGHT_GPIO, 0);
}

void setBackLightLevel(int level)
{
  int value = clamp(level, 0, 15);
  int i, num_clk, num_clk_to, num_clk_from;
  if (value == 0)
  {
    gpio_set_level(BACKLIGHT_GPIO, 0);
    vTaskDelay(3 / portTICK_PERIOD_MS);
  }
  else
  {
    if (current == 0)
    {
      gpio_set_level(BACKLIGHT_GPIO, 1);
      current = 15;
    }
    num_clk_from = 15 - current;
    num_clk_to = 15 - value;
    num_clk = (15 + num_clk_to - num_clk_from) % 15;
    for (i = 0; i < num_clk; i++)
    {
      gpio_set_level(BACKLIGHT_GPIO, 0);
      gpio_set_level(BACKLIGHT_GPIO, 1);
      if (i == 0)
        esp_rom_delay_us(30);
    }
    current = value;
  }
}
