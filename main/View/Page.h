
/*
 * @Date: 2024-04-05 21:06:40
 * @LastEditors: AaronChu
 * @LastEditTime: 2024-04-05 21:58:46
 */
#ifndef PAGE_H
#define PAGE_H
 /* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include <stdio.h>


// 生命周期钩子
typedef void (*EventHook)(void);
// 回调钩子
typedef void (*CallBack)(void *, int);
struct PageType // 页面结构体
{
  char name[32];         // 页面名称
  int show_status_bar;   // 是否显示状态栏
  lv_obj_t *PageContent; // 页面对象
  EventHook BeforeEnter; // 独享路由守卫
  EventHook Created;     // 页面创建回调钩子
  EventHook Update;      // 页面更新
  EventHook Destroy;     // 页面销毁
  CallBack Method;       // 页面事件回调
};

void Page_Init();       // 页面管理框架初始化
bool Page_Register(struct PageType Page);            // 注册页面
bool Page_Push(char *name);                   // 页面跳转
bool Page_Replace(char *name);                // 页面跳转
bool Page_Back(uint16_t delt);                // 返回页数
lv_obj_t *create_new_screen();
void Page_Update();





#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
