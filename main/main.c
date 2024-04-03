
#include "HAL.h"

static const char *TAG = "main";

void app_main()
{
  HAL_init();

  while (1)
  {
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
