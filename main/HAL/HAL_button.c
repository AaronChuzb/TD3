/*
 * @Date: 2024-04-03 15:44:56
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-07 23:59:57
 */

#include "HAL.h"

#define BUTTON_NUM 41
#define BUTTON_ACTIVE_LEVEL 0

static const char *TAG = "button_power_save";

const char *button_event_table[] = {
    "BUTTON_PRESS_DOWN",
    "BUTTON_PRESS_UP",
    "BUTTON_PRESS_REPEAT",
    "BUTTON_PRESS_REPEAT_DONE",
    "BUTTON_SINGLE_CLICK",
    "BUTTON_DOUBLE_CLICK",
    "BUTTON_MULTIPLE_CLICK",
    "BUTTON_LONG_PRESS_START",
    "BUTTON_LONG_PRESS_HOLD",
    "BUTTON_LONG_PRESS_UP",
};

int flag = 1;
/**
 * 按钮事件回调函数
 *
 * 当按钮事件发生时，此函数将被调用。它会记录并打印出按钮事件的名称，以及设备从低功耗模式（如轻度睡眠）唤醒的原因。
 *
 * @param arg 传递给回调函数的用户数据，此示例中未使用。
 * @param data 按钮事件的类型，是一个button_event_t类型的枚举值。
 */
void button_event_cb(void *arg, void *data)
{
  // 打印按钮事件的名称
  // ESP_LOGI(TAG, "Button event %s", button_event_table[(button_event_t)data]);

  // 获取设备唤醒的原因
  // esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

  // // 如果唤醒原因已定义，打印唤醒原因
  // if (cause != ESP_SLEEP_WAKEUP_UNDEFINED)
  // {
  // ESP_LOGI(TAG, "Wake up from light sleep, reason %d", 1);
  // }
  printf("Button is pressed, %d\n", flag);
  if (flag == 1)
  {
    setBackLightLevel(0);
    // lv_msg_send(MSG_NEW_TEMPERATURE, &flag);
    // lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_180);
    flag = 0;
  }
  else
  {
    // lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_NONE);
    setBackLightLevel(10);
    flag = 1;
  }
}

/**
 * 初始化按钮
 * 该函数负责初始化一个按钮，配置其类型、GPIO号、激活电平，并注册单击事件回调函数。
 *
 * 参数:
 * 无
 *
 * 返回值:
 * 无
 */
void button_init()
{
  // 配置按钮参数
  button_config_t btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .short_press_time = 100,
      .gpio_button_config = {
          .gpio_num = 41,                      // 按钮所使用的GPIO号
          .active_level = BUTTON_ACTIVE_LEVEL, // 按钮激活时的电平

          // .enable_power_save = false,      // 启用省电模式
      },
  };
  // 创建按钮实例
  button_handle_t btn = iot_button_create(&btn_cfg);
  assert(btn); // 确保按钮创建成功

  // 注册按钮单击事件回调
  esp_err_t err = iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, button_event_cb, (void *)BUTTON_SINGLE_CLICK);
  ESP_ERROR_CHECK(err); // 检查注册操作是否成功
}
