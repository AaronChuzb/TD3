#include "Fonts.h"

LV_FONT_DECLARE(lv_youyuan_21)
LV_FONT_DECLARE(lv_gigi_72)
LV_FONT_DECLARE(lv_youshebiaotihei_24)

lv_style_t font_style_youyuan_21;
lv_style_t font_style_gigi_72;
lv_style_t font_style_youshebiaotihei_24;

void lv_custom_fonts(void)
{
  extern int youyuan_21_init(void);
  youyuan_21_init();
  lv_style_init(&font_style_youyuan_21);
  lv_style_set_text_font(&font_style_youyuan_21, &lv_youyuan_21);

  lv_style_init(&font_style_gigi_72);
  lv_style_set_text_font(&font_style_gigi_72, &lv_gigi_72);

  lv_style_init(&font_style_youshebiaotihei_24);
  lv_style_set_text_font(&font_style_youshebiaotihei_24, &lv_youshebiaotihei_24);
}
