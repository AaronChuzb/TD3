#include "Fonts.h"

LV_FONT_DECLARE(lv_youyuan_21)
lv_style_t font_style_youyuan_21;

void lv_custom_fonts(void){
    extern int youyuan_21_init(void);
    youyuan_21_init();
    lv_style_init(&font_style_youyuan_21);
    lv_style_set_text_font(&font_style_youyuan_21, &lv_youyuan_21);
}
