# Magic-Mini (ESP32-C3) 项目

简要：基于 ESP32-C3 的小型灯板控制固件，内置 Web OTA 升级、AP/STA 配网、及 WS2812B 灯盘控制。

## 功能

-   WiFi 配网：AP / STA 模式（NVS 保存状态及凭据）
-   Web 管理页面：
    -   `/` (pageRoot)：显示大标题和“OTA 升级”按钮
    -   `/ota` (pageOTA)：选择 `.bin` 文件并提交（包含提交后加载动画）
    -   `/color` (pageColor)：调整 `RGB` 颜色（包含初始化颜色和提交颜色 API，由类似微信的模态框来表示是否成功）
    -   成功/失败页面：pageOTASuccess / pageOTAFail，成功页倒计时结束后自动返回 `/`
-   OTA：通过 Web 表单上传固件（`/update` 路由），分段写入 flash（使用 `Update` 库）
-   WS2812B 灯带控制与按键调节颜色/亮度
-   色盘调色：通过色盘页面向`/current-color` 获取当前颜色值以初始化、向 `/set-color` 提交 `RGB` 颜色以更改颜色

## 硬件

-   ESP32-C3-WROOM-02 或 兼容模块
-   WS2812B 灯条（示例中 40 个灯珠）
-   三个按键（左、中、右）用于交互
-   IP5306 电源管理模块控制开关

引脚（示例）

-   RGB 信号：GPIO10
-   左按键：GPIO18
-   中按键：GPIO19
-   右按键：GPIO3

接口

未连接网络

-   `/`配网页面

已连接网络

-   `/` root 页面
-   `/ota` ota 升级页面
-   `/update` ota 升级接口
-   `/color` color 页面
-   `/current-color`：初始化颜色选择器接口
-   `/set-color`：设置灯板颜色接口

## 编译与烧录

-   使用 PlatformIO 或 Arduino IDE 编译
-   编译后首次烧录通过串口（USB），后续可通过 Web OTA 上传 `.bin` 文件

## 使用说明

1. 焊接并按[开源文档](https://oshwhub.com/wogany/magic-mini-tianma-electric-light)进行安装
2. 编译烧录项目到 ESP32-C3
3. 首次上电设备会进入配网模式，用户需要连接到该设备的 WiFi 网络（默认 SSID：`Magic-Mini`，密码：`12345678`）并访问`http://magic-mini.local/`进行配网，然后设备会自动重启。
4. 若设备已联网则会进入 STA 模式，可通过 `http://magic-mini.local/` 访问。

## 项目目录结构

```PlainText
\Magic-Mini
├── README.md                                    # 项目说明文档
├── include\                                     # 头文件目录
│   ├── nvs.h                                    # NVS(非易失性存储)相关头文件
│   ├── web.h                                    # Web服务器相关头文件
│   └── ws2812b.h                                # WS2812B LED灯带控制头文件
├── pages\                                       # 前端页面调试目录
│   ├── README.md                                # 说明文档
│   ├── back\                                    # 色盘后端目录
│   │   └── server.js                            # Node.js服务器文件
│   └── before\                                  # 前端页面目录
│       └── cpp\                                 # C++工具目录
│           ├── HtmlToCppChar.cpp                # HTML转C++字符工具源码
│           ├── HtmlToCppChar.exe                # HTML转C++字符工具可执行文件
│           └── README.md                        # 工具说明文档
├── partitions.csv                               # ESP32分区表配置文件
├── platformio.ini                               # PlatformIO项目配置文件
└── src\                                         # 源代码目录
    ├── main.cpp                                 # 主程序入口文件
    ├── nvs.cpp                                  # NVS(非易失性存储)功能实现
    ├── web.cpp                                  # Web服务器功能实现
    └── ws2812b.cpp                              # WS2812B LED灯带控制实现
```