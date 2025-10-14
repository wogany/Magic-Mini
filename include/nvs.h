#ifndef _NVS_H
#define _NVS_H

#include <Preferences.h> // 相当于pinia状态管理

class CNvs
{
private:
    Preferences PREFS; // ESP32的Preferences库实例，用于NVS(非易失性存储)操作

public:
    CNvs(void);  // 构造函数，初始化NVS存储对象
    ~CNvs(void); // 析构函数，释放NVS存储资源

    bool init(void);                                                             // 初始化NVS存储系统，返回初始化是否成功
    void saveColor(uint8_t r, uint8_t g, uint8_t b);                             // 保存RGB颜色值到NVS存储
    void loadColor(uint8_t &r, uint8_t &g, uint8_t &b);                          // 从NVS存储加载RGB颜色值
    void saveWifiState(bool state);                                              // 保存WiFi状态(连接/断开)到NVS存储
    void getWifiState(bool &state);                                              // 从NVS存储获取WiFi状态
    void saveWifiInfo(const char *ssid, const char *password, bool hasPassword); // 保存WiFi连接信息(SSID和密码)到NVS存储
    void loadWifiInfo(char *ssid, char *password, bool &hasPassword);            // 从NVS存储加载WiFi连接信息
};

#endif