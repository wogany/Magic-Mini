#ifndef _WEB_H
#define _WEB_H

#include <WiFi.h>
#include <WebServer.h>

class CWeb
{
private:
    WebServer SERVER; // 创建WebServer对象

    bool m_wifiState;                 // WiFi配网状态
    bool m_haveStaPassword;           // 是否有STA密码
    static const char m_ssidAp[];     // WiFi热点名称
    static const char m_passwordAp[]; // WiFi热点密码
    static const char m_webName[];    // web域名
    char m_ssidSta[40];               // WiFi站点名称
    char m_passwordSta[40];           // WiFi站点密码
    bool m_isOTASuccess;              // OTA升级是否成功

    void m_handleWifiConfig(void); // WiFi配网网页处理函数
    void m_handleConnect(void);    // 连接WiFi处理函数
    void m_handleRoot(void);       // 根目录处理函数
    void m_handleOta(void);        // ota升级页面处理函数
    void m_handleOtaState(void);   // ota升级状态页面处理函数
    void m_handleOtaUpload(void);  // OTA升级文件上传处理函数

public:
    CWeb(void);
    ~CWeb(void);

    bool init(void);
    void begin(void);
    void loop(void);
};

#endif