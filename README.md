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

