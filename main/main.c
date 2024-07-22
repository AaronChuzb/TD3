
#include "HAL.h"
#include "Model.h"

static const char *TAG = "main";




void app_main()
{
  // Pre_HAL_Init();
  HAL_init();
  lvgl_task_init();
  
  Model_init();
  load_config2ram();
  // http_native_request();
  http_process();
  get_sram_size();
  // get_config_item_char("version");
  // update_config_item_char("version", "2.0.0");
  // load_config2ram();
  // get_config_item_char("version");

  // get_config_item_int("size");
  // update_config_item_int("size", 20);
  // load_config2ram();
  // get_config_item_int("size");
  
  // get_config_item_bool("isBool");
  // update_config_item_bool("isBool", false);
  // load_config2ram();
  // get_config_item_bool("isBool");

  // init_tone();
  
  while (1)
  {
    // if(is_sd_present()){
    //   printf("sd card is present\n");
    // } else {
    //   printf("sd card is not present\n");

    // }
    // get_sram_size();
    Page_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
