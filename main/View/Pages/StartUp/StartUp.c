/*
 * @Date: 2024-04-05 21:31:50
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-21 00:34:54
 */

#include "StartUp.h"

// 声明页面结构体

// char *pwd = "";
char pwd[9];

struct PageType StartUp;

lv_obj_t *obj2;
lv_obj_t *ta;
static void event_btn1_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e); // 获取回调事件
  if (code == LV_EVENT_CLICKED)
  { // 点击事件
    Page_Push("Home");
    // Page_Push("Settings");
  }
}

static void anim_x_cb(void *var, int32_t v)
{
  lv_obj_set_x((lv_obj_t *)var, v);
}

static void button_anim_in()
{
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj2);
  lv_anim_set_values(&a, -100, 5);
  lv_anim_set_time(&a, 150);
  lv_anim_set_exec_cb(&a, anim_x_cb);
  lv_anim_set_path_cb(&a, lv_anim_path_linear);
  // 页面切换动画执行完毕之后再执行出现动画
  lv_anim_set_delay(&a, 300);
  lv_anim_start(&a);
}

int aList[] = {1, 8, 6, 7, 7, 3, 8, 8, 1, 0, 1, 2, 2, 8, 8, 1, 0};
// 简单的加法哈希函数
unsigned int hash(const char *macAddress)
{
  unsigned int hash = 0;
  for (int i = 0; i < strlen(macAddress); i++)
  {
    hash += (unsigned int)macAddress[i] + aList[i];
  }
  return hash;
}
// 对哈希结果进行一系列操作生成混淆的结果
unsigned int generateObfuscatedValue(unsigned int hash)
{
  // 使用相同的数字来匹配 JavaScript 中的算法
  unsigned int result = (hash * 57138) % 98228810;
  return result;
}

// 将结果转换为8位的纯数字字符串
void toEightDigitString(unsigned int value, char *encryptedString)
{
  sprintf(encryptedString, "%08u", value);
}

static void lv_keyboard_def_event(lv_event_t *e)
{
  // const char *input_text = lv_textarea_get_text(ta);
  // if (strcmp(pwd, input_text) == 0)
  // {
  //   init_wifi();
  // }
  Page_Push("Home");
  // const char *input_text = lv_textarea_get_text(ta);
  // printf("input_text: %s\n", input_text);
  // printf("pwd: %s\n", pwd);
  // if (strcmp(pwd, input_text) == 0)
  // {
  //   Page_Push("Home");
  // }
}

static void msg_event_cb(lv_event_t *e)
{
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);
  switch (lv_msg_get_id(m))
  {
  case MSG_MAC_SET:
    lv_label_set_text(label, lv_msg_get_payload(m));
    // 哈希
    unsigned int hashedValue = hash(lv_msg_get_payload(m));

    // 生成混淆的结果
    unsigned int obfuscatedValue = generateObfuscatedValue(hashedValue);

    // 将结果转换为8位的纯数字字符串
    char encryptedMAC[9];
    toEightDigitString(obfuscatedValue, encryptedMAC);
    // sprintf(pwd, "%s", encryptedMAC);
    strcpy(pwd, encryptedMAC);

    printf("Encrypted MAC: %s\n", encryptedMAC);
    break;
  default:
    break;
  }
}

static void Created()
{

  lv_obj_t *label = lv_label_create(StartUp.PageContent);

  lv_label_set_text(label, "00:00:00:00:00:00");
  lv_obj_add_style(label, &font_style_youyuan_21, 0);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 50);
  // 设置消息回调
  lv_obj_add_event_cb(label, msg_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  // 订阅消息
  lv_msg_subsribe_obj(MSG_MAC_SET, label, NULL);

  // lv_obj_t *page_bg = lv_img_create(StartUp.PageContent);
  // lv_img_set_src(page_bg, &background);
  // lv_obj_set_width(page_bg, LV_SIZE_CONTENT);  /// 1
  // lv_obj_set_height(page_bg, LV_SIZE_CONTENT); /// 1
  // lv_obj_set_align(page_bg, LV_ALIGN_CENTER);
  // lv_obj_add_flag(page_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST); /// Flags
  // lv_obj_clear_flag(page_bg, LV_OBJ_FLAG_SCROLLABLE);
  // lv_obj_set_y(page_bg, -30);
  // lv_img_set_zoom(page_bg, 155);                     /// Flags
  // obj2 = lv_obj_create(StartUp.PageContent);
  // lv_obj_set_size(obj2, 100, 150);
  // lv_obj_set_pos(obj2, -100, 35);
  // lv_obj_add_event_cb(obj2, event_btn1_handler, LV_EVENT_ALL, NULL); /*设置btn1回调函数*/

  // button_anim_in();

  lv_obj_t *kb = lv_keyboard_create(StartUp.PageContent); // 创建键盘矩阵部件
  lv_obj_add_event_cb(kb, lv_keyboard_def_event, LV_EVENT_READY, NULL);
  // 知识点2：关联文本框
  ta = lv_textarea_create(StartUp.PageContent); // 创建文本区域部件
  lv_obj_set_size(ta, 310, 50);
  lv_keyboard_set_textarea(kb, ta); // 关联键盘和文本区域部件

  lv_obj_align(ta, LV_ALIGN_CENTER, 0, -50);
  lv_textarea_set_max_length(ta, 8);
  lv_textarea_set_align(ta, LV_TEXT_ALIGN_CENTER);
  lv_textarea_set_one_line(ta, true);
  lv_obj_set_style_text_font(ta, &lv_font_montserrat_18, 0);
  static const char *kb_map[] = {
      "1", "2", "3", "\n",
      "4", "5", "6", "\n",
      "7", "8", "9", "\n",
      LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_OK,
      ""};
  static const lv_btnmatrix_ctrl_t keyboard_ctrl[] =
      {
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
          LV_BTNMATRIX_CTRL_NO_REPEAT,
      };
  // 知识4：设置键盘模式
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1); // 自定义模式
  lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, keyboard_ctrl);
}

static void Update(void)
{
}

static void Destroy(void)
{
}

static void Method(void *btn, int event)
{
}

void StartUp_Init()
{
  strcpy(StartUp.name, "StartUp");
  StartUp.show_status_bar = 1;
  StartUp.BeforeEnter = NULL;
  StartUp.Created = Created;
  StartUp.Update = Update;
  StartUp.Destroy = Destroy;
  StartUp.Method = Method;
  StartUp.PageContent = create_new_screen();
  Page_Register(StartUp);
}