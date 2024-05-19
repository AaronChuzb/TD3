#include "HAL.h"

const char *TAG = "LCD";
static SemaphoreHandle_t lvgl_mux = NULL;

static esp_lcd_touch_handle_t tp = NULL;

static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv;      // contains callback functions

esp_lcd_panel_handle_t panel_handle = NULL;

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
  return false;
}

static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
  const int offsetx1 = area->x1;
  const int offsetx2 = area->x2;
  const int offsety1 = area->y1;
  const int offsety2 = area->y2;

  // copy a buffer's content to a specific area of the display
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
static void lvgl_update_cb(lv_disp_drv_t *drv)
{
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
  switch (drv->rotated)
  {
  case LV_DISP_ROT_NONE:
    // Rotate LCD display
    esp_lcd_panel_swap_xy(panel_handle, false);
    esp_lcd_panel_mirror(panel_handle, true, false);

    // Rotate LCD touch
    esp_lcd_touch_set_mirror_y(tp, false);
    esp_lcd_touch_set_mirror_x(tp, false);

    break;
  case LV_DISP_ROT_90:
    // Rotate LCD display
    esp_lcd_panel_swap_xy(panel_handle, true);
    esp_lcd_panel_mirror(panel_handle, false, false);

    // Rotate LCD touch
    esp_lcd_touch_set_mirror_y(tp, false);
    esp_lcd_touch_set_mirror_x(tp, false);
    break;
  case LV_DISP_ROT_180:
    // Rotate LCD display
    esp_lcd_panel_swap_xy(panel_handle, false);
    esp_lcd_panel_mirror(panel_handle, false, true);

    // Rotate LCD touch
    esp_lcd_touch_set_mirror_y(tp, false);
    esp_lcd_touch_set_mirror_x(tp, false);

    break;
  case LV_DISP_ROT_270:
    // Rotate LCD display
    esp_lcd_panel_swap_xy(panel_handle, true);
    esp_lcd_panel_mirror(panel_handle, true, true);

    // Rotate LCD touch
    esp_lcd_touch_set_mirror_y(tp, false);
    esp_lcd_touch_set_mirror_x(tp, false);
    break;
  }
}

void lvgl_rounder_cb(struct _lv_disp_drv_t *disp_drv, lv_area_t *area)
{
  uint16_t x1 = area->x1;
  uint16_t x2 = area->x2;

  uint16_t y1 = area->y1;
  uint16_t y2 = area->y2;

  // round the start of coordinate down to the nearest 2M number
  area->x1 = (x1 >> 1) << 1;
  area->y1 = (y1 >> 1) << 1;
  // round the end of coordinate up to the nearest 2N+1 number
  area->x2 = ((x2 >> 1) << 1) + 1;
  area->y2 = ((y2 >> 1) << 1) + 1;
}

static SemaphoreHandle_t touch_mux = NULL;

static void lvgl_touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  esp_lcd_touch_handle_t tp = (esp_lcd_touch_handle_t)drv->user_data;
  assert(tp);

  uint16_t tp_x;
  uint16_t tp_y;
  uint8_t tp_cnt = 0;
  /* Read data from touch controller into memory */
  if (xSemaphoreTake(touch_mux, 0) == pdTRUE)
  {
    esp_lcd_touch_read_data(tp);
  }

  /* Read data from touch controller */
  bool tp_pressed = esp_lcd_touch_get_coordinates(tp, &tp_x, &tp_y, NULL, &tp_cnt, 1);
  if (tp_pressed && tp_cnt > 0)
  {
    data->point.x = tp_x;
    data->point.y = tp_y;
    data->state = LV_INDEV_STATE_PRESSED;
    ESP_LOGD(TAG, "Touch position: %d,%d", tp_x, tp_y);
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static void touch_isr_cb(esp_lcd_touch_handle_t tp)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(touch_mux, &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken)
  {
    portYIELD_FROM_ISR();
  }
}

static void increase_lvgl_tick(void *arg)
{
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

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
  ESP_LOGI(TAG, "开始LVGL任务");
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

void init_lcd()
{

  ESP_LOGI(TAG, "初始化SPI总线");

  const spi_bus_config_t buscfg = GC9B71_PANEL_BUS_QSPI_CONFIG(PIN_NUM_LCD_PCLK,
                                                               PIN_NUM_LCD_DATA0,
                                                               PIN_NUM_LCD_DATA1,
                                                               PIN_NUM_LCD_DATA2,
                                                               PIN_NUM_LCD_DATA3,
                                                               LCD_H_RES * LCD_V_RES * LCD_BIT_PER_PIXEL / 8);

  spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);

  ESP_LOGI(TAG, "载入面板引脚");
  esp_lcd_panel_io_handle_t io_handle = NULL;
  const esp_lcd_panel_io_spi_config_t io_config = GC9B71_PANEL_IO_QSPI_CONFIG(PIN_NUM_LCD_CS,
                                                                              notify_lvgl_flush_ready,
                                                                              &disp_drv);
  gc9b71_vendor_config_t vendor_config = {
      .flags = {
          .use_qspi_interface = 1,
      },
  };

  // Attach the LCD to the SPI bus
  esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle);

  const esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_NUM_LCD_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = LCD_BIT_PER_PIXEL,
      .vendor_config = &vendor_config,
  };

  ESP_LOGI(TAG, "载入GC9B71面板驱动");
  esp_lcd_new_panel_gc9b71(io_handle, &panel_config, &panel_handle);
  esp_lcd_panel_reset(panel_handle);
  esp_lcd_panel_init(panel_handle);
  // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
  esp_lcd_panel_disp_on_off(panel_handle, true);

  ESP_LOGI(TAG, "初始化I2C总线");
  const i2c_config_t i2c_conf = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = PIN_NUM_TOUCH_SDA,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_io_num = PIN_NUM_TOUCH_SCL,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = 400 * 1000,
  };
  i2c_param_config(TOUCH_HOST, &i2c_conf);
  i2c_driver_install(TOUCH_HOST, i2c_conf.mode, 0, 0, 0);

  esp_lcd_panel_io_handle_t tp_io_handle = NULL;

  const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();

  // Attach the TOUCH to the I2C bus
  esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)TOUCH_HOST, &tp_io_config, &tp_io_handle);

  touch_mux = xSemaphoreCreateBinary();
  assert(touch_mux);

  const esp_lcd_touch_config_t tp_cfg = {
      .x_max = LCD_H_RES,
      .y_max = LCD_V_RES,
      .rst_gpio_num = PIN_NUM_TOUCH_RST,
      .int_gpio_num = PIN_NUM_TOUCH_INT,
      .levels = {
          .reset = 0,
          .interrupt = 0,
      },
      .flags = {
          .swap_xy = 0,
          .mirror_x = 0,
          .mirror_y = 0,
      },
      .interrupt_callback = touch_isr_cb,
  };

  ESP_LOGI(TAG, "初始化触摸控制器");
  esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &tp);
}

void init_lvgl_port()
{
  ESP_LOGI(TAG, "初始化LVGL");
  lv_init();

  // alloc draw buffers used by LVGL
  // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
  // 只要开了DMA这里申请的内存都在片内
  lv_color_t *buf1 = malloc(LCD_H_RES * LCD_V_RES / 2 * sizeof(lv_color_t));
  assert(buf1);
  lv_color_t *buf2 = malloc(LCD_H_RES * LCD_V_RES / 2 * sizeof(lv_color_t));
  assert(buf2);

  // initialize LVGL draw buffers
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_H_RES * LCD_V_RES / 2);

  ESP_LOGI(TAG, "注册显示驱动至LVGL");
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LCD_H_RES;
  disp_drv.ver_res = LCD_V_RES;
  disp_drv.flush_cb = lvgl_flush_cb;
  disp_drv.rounder_cb = lvgl_rounder_cb;
  disp_drv.drv_update_cb = lvgl_update_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  disp_drv.rotated = LV_DISP_ROT_NONE;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  ESP_LOGI(TAG, "载入LVGL滴答定时器");
  // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &increase_lvgl_tick,
      .name = "lvgl_tick"};
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

  static lv_indev_drv_t indev_drv; // Input device driver (Touch)
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.disp = disp;
  indev_drv.read_cb = lvgl_touch_cb;
  indev_drv.user_data = tp;

  lv_indev_drv_register(&indev_drv);

  lvgl_mux = xSemaphoreCreateMutex();
  assert(lvgl_mux);
  // Allocate memory for task stack

  // xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);
  // 将堆栈放到外部内存使用
  xTaskCreateWithCaps(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL, MALLOC_CAP_SPIRAM);
  setBackLightLevel(10);
  // xTaskCreatePinnedToCore(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL, 1);

  ESP_LOGI(TAG, "运行LVGL实例\n");

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
