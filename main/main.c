
#include "HAL.h"
#include "Model.h"

static const char *TAG = "main";




void app_main()
{
  Pre_HAL_Init();
  lvgl_task_init();
  HAL_init();
  Model_init();
  // init_tone();
  
  // while (1)
  // {
  //   // if(is_sd_present()){
  //   //   printf("sd card is present\n");
  //   // } else {
  //   //   printf("sd card is not present\n");

  //   // }
  //   // get_sram_size();
  //   vTaskDelay(1000 / portTICK_PERIOD_MS);
  // }
}
