/*
 * @Date: 2024-04-27 21:39:04
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-27 22:05:47
 */

#include "HAL.h"

static const int RX_BUF_SIZE = 10240;

#define TXD_PIN (GPIO_NUM_43)
#define RXD_PIN (GPIO_NUM_44)

static void rx_task(void *arg)
{
  static const char *RX_TASK_TAG = "RX_TASK";
  esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
  uint8_t *data = (uint8_t *)heap_caps_malloc(RX_BUF_SIZE + 1, MALLOC_CAP_SPIRAM);

  while (1)
  {
    const int rxBytes = uart_read_bytes(UART_NUM_0, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    if (rxBytes > 0)
    {
      data[rxBytes] = 0;
      ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
      // ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
    }
  }
  free(data);
}

void init_uarts(void)
{
  const uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // We won't use a buffer for sending data.
  uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
  uart_param_config(UART_NUM_0, &uart_config);
  uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  // xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, 4, NULL);
  xTaskCreateWithCaps(rx_task, "uart_rx_task", 20 * 1024, NULL, 4, NULL, MALLOC_CAP_SPIRAM);
}
