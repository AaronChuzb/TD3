/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_lcd_panel_io_interface.h"
#include "esp_lcd_panel_ops.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "unity_test_utils_memory.h"

#include "esp_lcd_gc9b71.h"

#define TEST_LCD_HOST               SPI2_HOST
#define TEST_LCD_H_RES              (320)
#define TEST_LCD_V_RES              (386)
#define TEST_LCD_BIT_PER_PIXEL      (16)

#define TEST_PIN_NUM_LCD_CS         (GPIO_NUM_9)
#define TEST_PIN_NUM_LCD_PCLK       (GPIO_NUM_10)
#define TEST_PIN_NUM_LCD_DATA0      (GPIO_NUM_11)
#define TEST_PIN_NUM_LCD_DATA1      (GPIO_NUM_12)
#define TEST_PIN_NUM_LCD_DATA2      (GPIO_NUM_13)
#define TEST_PIN_NUM_LCD_DATA3      (GPIO_NUM_14)
#define TEST_PIN_NUM_LCD_RST        (GPIO_NUM_17)
#define TEST_PIN_NUM_LCD_DC         (GPIO_NUM_8)

#define TEST_DELAY_TIME_MS          (3000)

static char *TAG = "gc9b71_test";
static SemaphoreHandle_t refresh_finish = NULL;

static bool test_notify_refresh_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    BaseType_t need_yield = pdFALSE;

    xSemaphoreGiveFromISR(refresh_finish, &need_yield);
    return (need_yield == pdTRUE);
}

IRAM_ATTR static void test_draw_bitmap(esp_lcd_panel_handle_t panel_handle)
{
    refresh_finish = xSemaphoreCreateBinary();
    TEST_ASSERT_NOT_NULL(refresh_finish);

    uint16_t row_line = ((TEST_LCD_V_RES / TEST_LCD_BIT_PER_PIXEL) << 1) >> 1;
    uint8_t byte_per_pixel = TEST_LCD_BIT_PER_PIXEL / 8;
    uint8_t *color = (uint8_t *)heap_caps_calloc(1, row_line * TEST_LCD_H_RES * byte_per_pixel, MALLOC_CAP_DMA);
    TEST_ASSERT_NOT_NULL(color);

    for (int j = 0; j < TEST_LCD_BIT_PER_PIXEL; j++) {
        for (int i = 0; i < row_line * TEST_LCD_H_RES; i++) {
            for (int k = 0; k < byte_per_pixel; k++) {
                color[i * byte_per_pixel + k] = (SPI_SWAP_DATA_TX(BIT(j), TEST_LCD_BIT_PER_PIXEL) >> (k * 8)) & 0xff;
            }
        }
        TEST_ESP_OK(esp_lcd_panel_draw_bitmap(panel_handle, 0, j * row_line, TEST_LCD_H_RES, (j + 1) * row_line, color));
        xSemaphoreTake(refresh_finish, portMAX_DELAY);
    }
    free(color);
    vSemaphoreDelete(refresh_finish);
    vTaskDelay(pdMS_TO_TICKS(TEST_DELAY_TIME_MS));
}

TEST_CASE("test gc9b71 to draw color bar with SPI interface", "[gc9b71][spi]")
{
    ESP_LOGI(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = GC9B71_PANEL_BUS_SPI_CONFIG(TEST_PIN_NUM_LCD_PCLK,
                                                                TEST_PIN_NUM_LCD_DATA0,
                                                                TEST_LCD_H_RES * TEST_LCD_V_RES * TEST_LCD_BIT_PER_PIXEL / 8);
    TEST_ESP_OK(spi_bus_initialize(TEST_LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t io_config = GC9B71_PANEL_IO_SPI_CONFIG(TEST_PIN_NUM_LCD_CS, TEST_PIN_NUM_LCD_DC,
                                                                               test_notify_refresh_ready, NULL);
    // Attach the LCD to the SPI bus
    TEST_ESP_OK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TEST_LCD_HOST, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install GC9B71 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TEST_PIN_NUM_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = TEST_LCD_BIT_PER_PIXEL,
    };
    TEST_ESP_OK(esp_lcd_new_panel_gc9b71(io_handle, &panel_config, &panel_handle));
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, true);

    test_draw_bitmap(panel_handle);

    TEST_ESP_OK(esp_lcd_panel_del(panel_handle));
    TEST_ESP_OK(esp_lcd_panel_io_del(io_handle));
    TEST_ESP_OK(spi_bus_free(TEST_LCD_HOST));
}

TEST_CASE("test gc9b71 to draw color bar with QSPI interface", "[gc9b71][qspi]")
{
    ESP_LOGI(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = GC9B71_PANEL_BUS_QSPI_CONFIG(TEST_PIN_NUM_LCD_PCLK,
                                                                 TEST_PIN_NUM_LCD_DATA0,
                                                                 TEST_PIN_NUM_LCD_DATA1,
                                                                 TEST_PIN_NUM_LCD_DATA2,
                                                                 TEST_PIN_NUM_LCD_DATA3,
                                                                 TEST_LCD_H_RES * TEST_LCD_V_RES * TEST_LCD_BIT_PER_PIXEL / 8);
    TEST_ESP_OK(spi_bus_initialize(TEST_LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t io_config = GC9B71_PANEL_IO_QSPI_CONFIG(TEST_PIN_NUM_LCD_CS, test_notify_refresh_ready, NULL);
    // Attach the LCD to the SPI bus
    TEST_ESP_OK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TEST_LCD_HOST, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install GC9B71 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    const gc9b71_vendor_config_t vendor_config = {
        .flags = {
            .use_qspi_interface = 1,
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TEST_PIN_NUM_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = TEST_LCD_BIT_PER_PIXEL,
        .vendor_config = (void *) &vendor_config,
    };
    TEST_ESP_OK(esp_lcd_new_panel_gc9b71(io_handle, &panel_config, &panel_handle));
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, true);

    test_draw_bitmap(panel_handle);

    TEST_ESP_OK(esp_lcd_panel_del(panel_handle));
    TEST_ESP_OK(esp_lcd_panel_io_del(io_handle));
    TEST_ESP_OK(spi_bus_free(TEST_LCD_HOST));
}

// Some resources are lazy allocated in the LCD driver, the threadhold is left for that case
#define TEST_MEMORY_LEAK_THRESHOLD  (300)

static size_t before_free_8bit;
static size_t before_free_32bit;

void setUp(void)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
}

void tearDown(void)
{
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    unity_utils_check_leak(before_free_8bit, after_free_8bit, "8BIT", TEST_MEMORY_LEAK_THRESHOLD);
    unity_utils_check_leak(before_free_32bit, after_free_32bit, "32BIT", TEST_MEMORY_LEAK_THRESHOLD);
}

void app_main(void)
{
    /**
     *    ___   ___  ___    ___ _____ _
     *   / _ \ / __\/ _ \  / __\___  / |
     *  / /_\// /  | (_) |/__\//  / /| |
     * / /_\\/ /___ \__, / \/  \ / / | |
     * \____/\____/   /_/\_____//_/  |_|
    */
    printf("   ___   ___  ___    ___ _____ _\r\n");
    printf("  / _ \\ / __\\/ _ \\  / __\\___  / |\r\n");
    printf(" / /_\\// /  | (_) |/__\\//  / /| |\r\n");
    printf("/ /_\\\\/ /___ \\__, / \\/  \\ / / | |\r\n");
    printf("\\____/\\____/   /_/\\_____//_/  |_|\r\n");
    unity_run_menu();
}
