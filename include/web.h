#ifndef _WEB_H
#define _WEB_H

#include <WiFi.h>
#include <WebServer.h>

class CWeb
{
private:
    WebServer SERVER; // Web服务器对象，用于处理HTTP请求和响应

    bool m_wifiState;                 // WiFi连接状态标志，true表示已连接，false表示未连接
    bool m_haveStaPassword;           // STA模式密码存在标志，true表示已设置密码，false表示未设置密码
    static const char m_ssidAp[];     // AP模式WiFi热点名称，用于设备配网时的热点标识
    static const char m_passwordAp[]; // AP模式WiFi热点密码，用于保护配网热点的安全性
    static const char m_webName[];    // Web服务器域名，用于通过域名访问设备网页界面
    char m_ssidSta[40];               // STA模式目标WiFi网络名称，存储要连接的WiFi网络的SSID
    char m_passwordSta[40];           // STA模式目标WiFi网络密码，存储要连接的WiFi网络的密码
    bool m_isOTASuccess;              // OTA升级成功标志，true表示升级成功，false表示升级失败或未进行升级

    void m_handleWifiConfig(void); // WiFi配网页面处理函数，响应配网页面的HTTP请求并处理用户输入的WiFi信息
    void m_handleConnect(void);    // WiFi连接处理函数，处理用户发起的WiFi连接请求并尝试连接指定网络
    void m_handleRoot(void);       // 根目录处理函数，响应网站根路径的访问请求，通常返回主页或控制面板
    void m_handleOta(void);        // OTA升级页面处理函数，响应OTA升级页面的访问请求，显示升级界面
    void m_handleOtaState(void);   // OTA升级状态处理函数，响应OTA升级状态查询请求，返回当前升级进度和状态
    void m_handleOtaUpload(void);  // OTA文件上传处理函数，处理固件文件上传请求并执行OTA升级过程
    void m_handleColor(void);      // 颜色设置处理函数，响应颜色设置请求，更新LED颜色
    void m_handleColorSet(void);   // 颜色设置处理函数，响应颜色设置请求，更新LED颜色
    void m_handleColorGet(void);   // 颜色获取处理函数，响应颜色获取请求，返回当前LED颜色

public:
    CWeb(void);  // 构造函数，初始化Web服务器对象和成员变量
    ~CWeb(void); // 析构函数，清理资源并关闭Web服务器

    bool init(void);  // 初始化函数，设置Web服务器路由和参数，返回初始化是否成功
    void begin(void); // 启动函数，开始Web服务器并启动WiFi热点或连接WiFi网络
    void loop(void);  // 循环处理函数，需要在主循环中调用，用于处理HTTP客户端请求
};

#endif