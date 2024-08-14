# TD3-TouchDisplay3.0

<img src="image/README/1712408203453.png" width="400px" />

# 介绍视频

<img src="image/README/content.gif" width="400px" />

[B站早期视频](https://player.bilibili.com/player.html?isOutside=true&aid=1853047000&bvid=BV1cp421R78N&cid=1508688648&p=1)

# 复刻注意事项

- 板子厚度选择1.2mm
- 阻抗匹配无要求
- 晶振要买无源的啊（血的教训）

## 锂电池

锂电池使用 `402530`的大小，容量 `300mAh`，目前同体积没找到更好的替代品，体积超过这个规格不适用我的外壳可能放不进去。

购买链接：[【淘宝】限时满20减2 https://m.tb.cn/h.gPwur8v7pjAurAq?tk=iaC63e7ytO5 MF6563 「3.7V聚合物锂电池对讲机故事机行车记录仪音箱通用大容量内置电池」](https://m.tb.cn/h.gPwur8v7pjAurAq?tk=iaC63e7ytO5)

记得叫商家加 `SH1.0`的头子,加头子是免费的，接口方向参考下图，方向跟板子上的母座方向是适配的，当然可以选择不接，同时也不焊接板子上的母座，直接飞线到板子上，但是SH1.0的座子引脚间距还是比较小的。

![1723601199571](image/README/1723601199571.png)

## 磁吸接口

磁吸接口购买链接尽量使用bom表中的购买地址，3d模型我已经建好了，接口线序我也测过了，直接在那家店买线就ok，未来可能会画一个底座兼容TYPE-C插头。

购买链接：[【淘宝】https://m.tb.cn/h.glr8JNFoAGLjC2Y?tk=W5ul3eFrBDn CZ3460 「pogo pin磁吸式连接器公母带螺丝孔安装 弹簧顶针USB充电导电PIN」
点击链接直接打开 或者 淘宝搜索直接打开](https://m.tb.cn/h.glr8JNFoAGLjC2Y?tk=W5ul3eFrBDn)

购买下面 `4pin`的线，座子 `选弯头3.0mm的但是实际不是要3mm的`，要找客服说要2.54mm的 他们没挂链接，所以只能这样拍,记得一定要联系客服联系客服！！

## 无线充电

目前这块芯片在淘宝上好像只有一家代理商，并且配套的线圈在他们那里也能买到，这块芯片的外围阻容对材质是有要求的，`请先看数据手册的典型电路，再去购买对应的阻容，这一点在bom表中并没有体现出来`。数据手册那些问商家要就好了，项目的docs文件夹里边也有。

购买链接：[【淘宝】https://m.tb.cn/h.glhmi1BjbDgyDQN?tk=bRlC3eFFOLT MF7997 「原装现货代理伏达无线充电接收芯片NU1680/NU1680C，符合Qi标准」
点击链接直接打开 或者 淘宝搜索直接打开](https://m.tb.cn/h.glhmi1BjbDgyDQN?tk=bRlC3eFFOLT)

线圈选他们店里的NU1680配套线圈记得选 `方的`那块

## 屏幕BTB座

型号：`OK-14GF024-04`母座

这座子超过5毛就不要买了，有点难焊，建议淘宝打个钢网配合铁板烧或者风枪吹脚，尽量多买一点。

## 2.4G陶瓷天线

6块钱20片要什么自行车？商家估计还会主动联系帮做匹配电路，其实也不用板子我实测TX可以到18dbm，某C3板子开到15都不行。板子上预留了Π型电路，牛逼的大佬可以自己做天线的阻抗匹配。

购买链接：[【淘宝】https://m.tb.cn/h.glhqlfbUDg5LPQ1?tk=XRp63eFBeyH MF1643 「原装贴片天线3216蓝牙天线CW324WIFI2.4G 2.5G 4G 5G陶瓷1206天线」
点击链接直接打开 或者 淘宝搜索直接打开](https://m.tb.cn/h.glhqlfbUDg5LPQ1?tk=XRp63eFBeyH)

## AXP173电源管理芯片

这片子确实不太好买，定制电压的版本太多，而且停产挺久了，用这个方案主要是我手里还有十几块没用完。
可以在淘宝上购买一两片样片回来测试，`只需要DCDC1输出是3.3V`就OK。

买到合适的可以提一个issure，或者直接提一个pr，更新到这个表当中

| DCDC1输出 | 价格 | 购买链接 |
| --------- | ---- | -------- |
|           |      |          |
|           |      |          |

## 蜂鸣器

淘宝搜`DET402`,不要问我为啥用这么多非标硬件，主要是以前项目的一些遗留品，结合手里的显示屏，做的项目。

## 模型打印

答应我选尼龙好吗，嘉立创三维猴打一个PA12的才15.5，树脂的会有一个问题热熔螺母嵌套不进去。按钮打一个就行了，一般都会多打几个给你 毕竟这么小。

热熔螺母购买链接： [【淘宝】限时满2减1 https://m.tb.cn/h.glbH1XjvrZgsixS?tk=2SyC3ev3ag4 CZ3460 「注塑热熔铜螺母M2M3通孔滚花螺帽土八字预埋铜嵌件铜花母M4M5M6M8」
点击链接直接打开 或者 淘宝搜索直接打开](https://m.tb.cn/h.glbH1XjvrZgsixS?tk=2SyC3ev3ag4)

热熔螺母规格选`M2x3x3.2`

烙铁开到`250度`就可以顶进去，当然我指的是`PA12`，选其他材质打印的请自行测试。



# TD3 项目配置

1. 基于 `esp-idf5.1.2`版本开发，后续可能会更换威更新的 idf 版本
2. `lvgl`版本为 `8.3.11`，后续可能会升级到 `9`

## 环境配置

拉取下来的代码是不包含 `.vscode`文件夹的，需要打开一个 idf 示例项目,然后复制.`vscode`文件夹到项目根目录下即可

## 目标板配置

1. 设置当前目标板：
   左下角点击![1712058009707](image/README/1712058009707.png)随后选择当前的工作区
2. 选择当前第一个如图所示的工作区

![1712058081783](image/README/1712058081783.png)

3. 选择 `esp32s3`

![1712058134787](image/README/1712058134787.png)

4. 选择 `via ESP-PROG`项

![1712058185628](image/README/1712058185628.png)

## menuconfig 内容配置

1. 先点击左下角![1712058290376](image/README/1712058290376.png)弹出
   ![1712058337887](image/README/1712058337887.png)
2. 点击保存随后会自动生成 `sdkconfig`文件
3. 将 `sdkconfig.temp`文件内的内容复制并替换 `sdkconfig`文件内的内容

## 项目文件结构说明

```
├─components
│  ├─axp173       // AXP173电源管理驱动
│  ├─backlight    // AW9364驱动
│  ├─beep         // 蜂鸣器驱动
│  ├─bmp280       // BMP280驱动
│  ├─espressif__cmake_utilities
│  ├─espressif__esp_lcd_gc9b71
│  ├─espressif__esp_lcd_touch
│  ├─espressif__esp_lcd_touch_cst816s
│  ├─i2c          // I2C驱动
│  ├─lsm6ds3      // LSM6DS3驱动
│  ├─lvgl         // LVGL
│  ├─pcf8563      // PCF8563驱动
│  └─qmc5883l     // QMC5883L驱动
│
├─docs
│
├─image
├─main
│   │  CMakeLists.txt
│   │  main.c
│   ├─HAL   // 硬件抽象层提供硬件的接口
│   ├─Model // 模型层主要处理数据，创建消息队列，创建采集任务
│   └─View  // mvvm结构中viewmodel层主要放生成的UI
│  .gitignore
│  CMakeLists.txt
│  partitions.csv
│  README.md
│  sdkconfig
│  sdkconfig.old
│  sdkconfig.temp
```

## flash download tools 烧录（不需要bin文件烧录请忽略）

1. 单独的 bin 文件烧录方式| bin                  | 地址    |
   | -------------------- | ------- |
   | bootloader.bin       | 0x0     |
   | partition-table      | 0x8000  |
   | ota_data_initial.bin | 0xd000  |
   | TD3V2.0.bin          | 0x10000 |
2. 合并的 bin 文件烧录方式| bin        | 地址 |
   | ---------- | ---- |
   | target.bin | 0x0  |
3. SPIFlashConfiig| 配置项    | 选项  |
   | --------- | ----- |
   | SPI SPEED | 80MHz |
   | SPI MODE  | DOUT  |
4. 烧录后手动关机重启即可

# 页面管理框架的介绍

这个页面管理框架是我从老的 TD3-App 项目移植过来的，原项目基于 Arduino 平台开发，所以使用的是 c++，但是 idf 是 c 的，虽然 idf 支持 c++混编但是太麻烦所以改成 c，失去了类跟命名空间。

## 如何添加页面

1. 在 `main\View\Pages`目录下新建你的页面文件夹，文件夹包含 xxx.c 文件和 xxx.h 文件，xxx 为你的页面名。

其中.c 文件需要遵循标准模板如下：

```
#include "Setting.h"

PageType *Setting;


static void Created()
{

  
}

static void Update(void)
{
}

static void Destroy(void)
{
  if (lv_obj_is_valid(Setting->PageContent))
  {
    lv_async_call(lv_obj_clean, Setting->PageContent);
  }
}

static void Method(void *btn, int event)
{
}

void Setting_Init()
{
  Setting = lv_mem_alloc(sizeof(PageType));
  strcpy(Setting->name, "Setting");
  Setting->show_status_bar = 1;
  Setting->BeforeEnter = NULL;
  Setting->Created = Created;
  Setting->Update = Update;
  Setting->Destroy = Destroy;
  Setting->Method = Method;
  Setting->PageContent = create_new_screen();
  Page_Register(*Setting);
}
```

只需要将文件中的全部 `Home`替换成其他名字即可。

.h 文件就比较简单,同样是将 Home 全部换成自己的，然后条件编译那里的大写 HOME 也改掉就行。

```
#ifndef SETTING_H
#define SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Page.h"


void Setting_Init();


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
```

## 优设标题黑取模文字内容

```
电池信息陀螺仪气压计储存卡实时时钟系统设置网络信息天气查询音乐蓝牙蜂鸣器配置串口数据上位机监控连接断开终端
```

2. 修改 `main\CMakeLists.txt`添加 `"View/Pages/xxx"`其中 xxx 为你的页面目录名，至于为什么不写一个统一的 cmake 文件需要时遵循 idf 的项目标准，其实时不会，对就是“不会”。

# 项目开发过程的问题记录（复刻无需关注）

## lvgl 使用 fatfs

1. menuconfig 勾选

`3rd Party Libraries`目录下 `File system on top of FatFS`，同时 `Set an upper cased letter on which the drive will accessible (e.g. 'A' i.e. 65) `改成 `48`，`>0 to cache this number of bytes in lv_fs_read()`改成 `10240`也就是 10k

2. cmake 修改

`components\lvgl\env_support\cmake\esp.cmake`这个文件内需要在 `idf_component_register`的 `REQUIRES`内添加 `fatfs`才不会报 `ff.h`文件找不到的问题

3. 解决 ` unknown type name 'DIR';`问题

`components\lvgl\src\extra\libs\fsdrv\lv_fs_fatfs.c`文件添加头文件 `#include <dirent.h>`

## FreeRTOS 如何使用外部内存的问题

参考[官方文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/migration-guides/release-5.x/5.1/system.html)的说明，由于允许将 FreeRTOS 对象（如队列和信号量）放置在外部 RAM 中可能会出现问题，例如如果在访问这些对象时 cache 被禁用（如在 SPI flash 写入操作期间），则会导致 cache 访问错误（详细信息请参阅 严重错误）。

但是需要使用外部内存堆栈的原因是给 lvgl 创建刷新任务，是任务所以不会存在产生严重错误的问题。

根据附加 API 的说法添加 freertos/esp_additions/include/freertos/idf_additions.h 头文件，包含了 ESP-IDF 添加的与 FreeRTOS 相关的辅助函数。通过 #include "freertos/idf_additions.h" 可添加此头文件。然后就可以使用 xTaskCreateWithCaps 创建任务。相比于动态创建任务只需要在最后添加一个申请内存方式即可

## 移植 rlottie 问题

1. 将 `rlottie`文件夹放入 lvgl 根目录
2. 编辑 `components\lvgl\env_support\cmake`目录下的 `esp.cmake`文件，在文件最上方添加

```
# rlottie
# add_subdirectory(rlottie)
# include_directories(${LVGL_ROOT_DIR}/rlottie/inc)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()

set(BUILD_SHARED_LIBS OFF)

option(LOTTIE_MODULE "Enable LOTTIE MODULE SUPPORT" OFF)
option(LOTTIE_THREAD "Enable LOTTIE THREAD SUPPORT" OFF)
option(LOTTIE_CACHE "Enable LOTTIE CACHE SUPPORT" ON)
option(LOTTIE_TEST "Build LOTTIE AUTOTESTS" OFF)
option(LOTTIE_CCACHE "Enable LOTTIE ccache SUPPORT" OFF)
option(LOTTIE_ASAN "Compile with asan" OFF)


file(GLOB_RECURSE RLOTTIE_SRCS ${LVGL_ROOT_DIR}/rlottie/src/*.cpp)
set(RLOTTIE_INCS
    ${LVGL_ROOT_DIR}/rlottie/inc/
    ${LVGL_ROOT_DIR}/rlottie/src/vector/
)

```

修改 `idf_component_register`行为

```
idf_component_register(SRCS ${SOURCES} ${EXAMPLE_SOURCES} ${DEMO_SOURCES} ${RLOTTIE_SRCS}
      INCLUDE_DIRS ${LVGL_ROOT_DIR} ${LVGL_ROOT_DIR}/src ${LVGL_ROOT_DIR}/../
                   ${LVGL_ROOT_DIR}/examples ${LVGL_ROOT_DIR}/demos ${RLOTTIE_INCS}
      REQUIRES esp_timer fatfs heap)
```

其中 heap 为本次添加

2. 修改 `components\lvgl\rlottie\src\vector\stb\stb_image.h`文件内的内容,注释第 3818 行内容

```
// stbi__errpuc("outofmem", "Out of memory");
```
