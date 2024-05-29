/*
 * @Date: 2024-05-29 17:33:05
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-05-29 17:54:49
 */
#include "Model.h"
TaskHandle_t lvgl_task_handle;
static SemaphoreHandle_t lvgl_mux = NULL;

static bool lvgl_lock(int timeout_ms)
{
  assert(lvgl_mux && "bsp_display_start must be called first");

  const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
  return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}

static void lvgl_unlock(void)
{
  assert(lvgl_mux && "bsp_display_start must be called first");
  xSemaphoreGive(lvgl_mux);
}

static void lvgl_port_task(void *arg)
{
  // ESP_LOGI(TAG, "开始LVGL任务");
  uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
  while (1)
  {
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_lock(-1))
    {
      task_delay_ms = lv_timer_handler();
      // Release the mutex
      lvgl_unlock();
    }
    if (task_delay_ms > LVGL_TASK_MAX_DELAY_MS)
    {
      task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
    }
    else if (task_delay_ms < LVGL_TASK_MIN_DELAY_MS)
    {
      task_delay_ms = LVGL_TASK_MIN_DELAY_MS;
    }
    vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
  }
}

void lvgl_task_init(void)
{
  lvgl_mux = xSemaphoreCreateMutex();
  assert(lvgl_mux);
  // Allocate memory for task stack

  // xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);
  // 将堆栈放到外部内存使用
  xTaskCreateWithCaps(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, &lvgl_task_handle, MALLOC_CAP_SPIRAM);
  setBackLightLevel(10);
  // xTaskCreatePinnedToCore(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL, 1);

  // ESP_LOGI(TAG, "运行LVGL实例\n");

  // Lock the mutex due to the LVGL APIs are not thread-safe
  if (lvgl_lock(-1))
  {
    // lv_demo_widgets(); /* A widgets example */
    // lv_demo_music();        /* A modern, smartphone-like music player demo. */
    // lv_demo_stress();       /* A stress test for LVGL. */
    // lv_demo_benchmark();    /* A demo to measure the performance of LVGL or to compare different settings. */
    // lv_example_meter_3();
    init_view();
    // Release the mutex

    lvgl_unlock();
  }
}

void lvgl_task_suspend(void)
{
  vTaskSuspend(lvgl_task_handle);
}

void lvgl_task_resume(void)
{
  vTaskResume(lvgl_task_handle);
}