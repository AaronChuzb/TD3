
#include "HAL.h"
#include "Model.h"

static const char *TAG = "main";

void app_main()
{
  HAL_init();
  Model_init();
  
  // while (1)
  // {

  //   vTaskDelay(1000 / portTICK_PERIOD_MS);
  // }
}
