# Magic-Mini (ESP32-C3) 项目

简要：基于 ESP32-C3 的小型灯板控制固件，内置 Web OTA 升级、AP/STA 配网、及 WS2812B 灯盘控制。

## 功能
- WiFi 配网：AP / STA 模式（NVS 保存状态及凭据）
- Web 管理页面：
  - `/` (pageRoot)：显示大标题和“OTA升级”按钮
  - `/ota` (pageOTA)：选择 `.bin` 文件并提交（包含提交后加载动画）
  - 成功/失败页面：pageOTASuccess / pageOTAFail，成功页倒计时结束后自动返回 `/`
- OTA：通过 Web 表单上传固件（`/update` 路由），分段写入 flash（使用 `Update` 库）
- WS2812B 灯带控制与按键调节颜色/亮度

## 硬件
- ESP32-C3-WROOM-02 或 兼容模块
- WS2812B 灯条（示例中 40 个灯珠）
- 三个按键（左、中、右）用于交互

引脚（示例）
- RGB 信号：GPIO10
- 左按键：GPIO18
- 中按键：GPIO19
- 右按键：GPIO3

## 编译与烧录
- 使用 PlatformIO 或 Arduino IDE 编译
- 编译后首次烧录通过串口（USB），后续可通过 Web OTA 上传 `.bin` 文件

## 使用说明
1. 通过串口监视器查看设备启动日志（115200）。
2. 若设备为 STA 模式，网页地址为 `http://<DEVICE_IP>/`；若启用 mDNS，可用 `http://esp32.local/` 访问。
3. 进入首页后点击 “OTA升级” 跳转到 `/ota` 上传页面。
4. 在 `/ota` 页面选择 `.bin` 文件，点击“开始升级”。页面会显示加载圈表示正在上传。
5. 上传完成后：
   - 写入成功：跳转到成功页并倒计时，页面会在倒计时结束后返回根目录（设备通常会重启并运行新固件）。
   - 写入失败：显示失败页，可返回重试。

命令行测试（Windows / curl）：