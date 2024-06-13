/*
 * @Date: 2024-04-05 21:06:24
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-06-13 16:06:28
 */
#include "View/View.h"


void init_view(void)
{
  // 加载字体
  lv_custom_fonts();
  // 页面框架初始化
  Page_Init();
  // 注册页面
  StartUp_Init();
  Auth_Init();
  Home_Init();
  Clock_Init();
  Pressure_Init();
  Setting_Init();
  Gyro_Init();
  Files_Init();
  Battery_Init();
  Weather_Init();


  Page_Replace("Home");
  // lv_obj_t *label = lv_label_create(lv_scr_act());

  // lv_label_set_text(label, "你好");
  // lv_obj_add_style(label, &font_style_youyuan_21, 0);
  // lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  // // //  LV_IMG_DECLARE(img_bulb_gif);
  // lv_obj_t *img;

  // img = lv_gif_create(lv_scr_act());
  // lv_gif_set_src(img, "S:/gif2.gif");

  // lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
  // lv_obj_t *lottie = lv_rlottie_create_from_raw(lv_scr_act(), 120, 120, lv_example_rlottie_approve);
  // lv_obj_center(lottie);
}


