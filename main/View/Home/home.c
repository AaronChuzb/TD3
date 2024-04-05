/*
 * @Date: 2024-04-04 23:22:08
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 18:02:30
 */
#include "View/Home/home.h"


void init_view(void)
{
  lv_custom_fonts();
  lv_obj_t *label = lv_label_create(lv_scr_act());

  lv_label_set_text(label, "你好");
  lv_obj_add_style(label, &font_style_youyuan_21, 0);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

  // //  LV_IMG_DECLARE(img_bulb_gif);
  lv_obj_t *img;

  img = lv_gif_create(lv_scr_act());
  lv_gif_set_src(img, "S:/gif2.gif");

  // lv_obj_align(img, LV_ALIGN_RIGHT_MID, -20, 0);
  // lv_obj_t *lottie = lv_rlottie_create_from_raw(lv_scr_act(), 120, 120, lv_example_rlottie_approve);
  // lv_obj_center(lottie);
}
