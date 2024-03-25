# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS
  "components/espressif__esp_lcd_gc9b71"
  "components/espressif__esp_lcd_touch"
  "components/espressif__esp_lcd_touch_cst816s"
  "components/espressif__esp_lvgl_port"
  "components/lvgl"
  "components/espressif__cmake_utilities"
  "components/i2c"
  "components/axp173"
  "components/backlight"
  "components/bmp280"
  "components/qmc5883l"
  "components/lsm6ds3"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(TD3-V2.0)