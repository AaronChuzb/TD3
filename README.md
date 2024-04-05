# TD3项目配置

1. 基于`esp-idf5.1.2`版本开发，后续可能会更换威更新的idf版本
2. `lvgl`版本为`8.3.11`，后续可能会升级到9

## 环境配置
拉取下来的代码是不包含`.vscode`文件夹的，需要打开一个idf示例项目,然后复制.`vscode`文件夹到项目根目录下即可

## 目标板配置

1. 设置当前目标板：
左下角点击![1712058009707](image/README/1712058009707.png)随后选择当前的工作区
2. 选择当前第一个如图所示的工作区

![1712058081783](image/README/1712058081783.png)

3. 选择`esp32s3`

![1712058134787](image/README/1712058134787.png)

4. 选择`via ESP-PROG`项


![1712058185628](image/README/1712058185628.png)

## menuconfig内容配置

1. 先点击左下角![1712058290376](image/README/1712058290376.png)弹出![1712058337887](image/README/1712058337887.png)
2. 点击保存随后会自动生成`sdkconfig`文件
3. 将`sdkconfig.temp`文件内的内容复制并替换`sdkconfig`文件内的内容


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
│   ├─Controller // 控制层接收消息队列数据，更新UI
│   ├─HAL   // 硬件抽象层提供硬件的接口
│   ├─Model // 模型层主要处理数据，创建消息队列，创建采集任务
│   └─View  // mvc结构中view层主要放生成的UI
│  .gitignore
│  CMakeLists.txt
│  partitions.csv
│  README.md
│  sdkconfig
│  sdkconfig.old
│  sdkconfig.temp  
```

# 项目开发过程的问题记录

## lvgl使用fatfs

1. menuconfig勾选

`3rd Party Libraries`目录下`File system on top of FatFS`，同时`Set an upper cased letter on which the drive will accessible (e.g. 'A' i.e. 65)
`改成`48`，`>0 to cache this number of bytes in lv_fs_read()`改成`10240`也就是10k

2. cmake修改

`components\lvgl\env_support\cmake\esp.cmake`这个文件内需要在`idf_component_register`的`REQUIRES`内添加`fatfs`才不会报`ff.h`文件找不到的问题

3. 解决` unknown type name 'DIR';`问题

`components\lvgl\src\extra\libs\fsdrv\lv_fs_fatfs.c`文件添加头文件`#include <dirent.h>`

## FreeRTOS如何使用外部内存的问题

参考[官方文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/migration-guides/release-5.x/5.1/system.html)的说明，由于允许将 FreeRTOS 对象（如队列和信号量）放置在外部 RAM 中可能会出现问题，例如如果在访问这些对象时 cache 被禁用（如在 SPI flash 写入操作期间），则会导致 cache 访问错误（详细信息请参阅 严重错误）。

但是需要使用外部内存堆栈的原因是给lvgl创建刷新任务，是任务所以不会存在产生严重错误的问题。

根据附加API的说法添加freertos/esp_additions/include/freertos/idf_additions.h 头文件，包含了 ESP-IDF 添加的与 FreeRTOS 相关的辅助函数。通过 #include "freertos/idf_additions.h" 可添加此头文件。然后就可以使用xTaskCreateWithCaps创建任务。相比于动态创建任务只需要在最后添加一个申请内存方式即可






