#include "nvs.h"

#define NAME "my_config"

/*
    @brief  构造函数
    @param  无
    @return 无
*/
CNvs::CNvs(void)
{
}

/*
    @brief  析构函数
    @param  无
    @return 无
*/
CNvs::~CNvs(void)
{
}

/*
    @brief  NVS初始化
    @param  无
    @return 成功与否
*/
bool CNvs::init(void)
{
    PREFS.begin(NAME, false);
    PREFS.end();
    return true;
}

/*
    @brief  保存颜色到NVS
    @param  r 红色分量
    @param  g 绿色分量
    @param  b 蓝色分量
    @return 无
*/
void CNvs::saveColor(uint8_t r, uint8_t g, uint8_t b)
{
    PREFS.begin(NAME, false);
    uint8_t lastR, lastG, lastB;
    /*  获取上次保存的颜色 */
    lastR = PREFS.getUChar("colorR", 0);
    lastG = PREFS.getUChar("colorG", 0);
    lastB = PREFS.getUChar("colorB", 0);
    /* 检测颜色是否变化 */
    if (lastR != r)
    {
        PREFS.putUChar("colorR", r);
    }
    if (lastG != g)
    {
        PREFS.putUChar("colorG", g);
    }
    if (lastB != b)
    {
        PREFS.putUChar("colorB", b);
    }
    PREFS.end();
}

/*
    @brief  从NVS加载颜色
    @param  r 红色分量引用
    @param  g 绿色分量引用
    @param  b 蓝色分量引用
    @return 无
*/
void CNvs::loadColor(uint8_t &r, uint8_t &g, uint8_t &b)
{
    PREFS.begin(NAME, true);
    r = PREFS.getUChar("colorR", 255);
    g = PREFS.getUChar("colorG", 255);
    b = PREFS.getUChar("colorB", 255);
    PREFS.end();
}

/*
    @brief  保存WiFi配网状态到NVS
    @param  state 配网状态
    @return 无
*/
void CNvs::saveWifiState(bool state)
{
    PREFS.begin(NAME, false);
    PREFS.putBool("wifiState", state);
    PREFS.end();
}

/*
    @brief  从NVS获取WiFi配网状态
    @param  state 配网状态
    @return 无
*/
void CNvs::getWifiState(bool &state)
{
    PREFS.begin(NAME, true);
    state = PREFS.getBool("wifiState", false);
    PREFS.end();
}

/*
    @brief  保存WiFi信息到NVS
    @param  ssid WiFi名称
    @param  password WiFi密码
    @param  hasPassword 是否有密码
    @return 无
*/
void CNvs::saveWifiInfo(const char* ssid, const char* password, bool hasPassword)
{
    PREFS.begin(NAME, false);
    PREFS.putString("wifiSSID", ssid);
    PREFS.putString("wifiPassword", password);
    PREFS.putBool("wifiHasPassword", hasPassword);
    PREFS.end();
}

/*
    @brief  从NVS加载WiFi信息
    @param  ssid WiFi名称
    @param  password WiFi密码
    @param  hasPassword 是否有密码
    @return 无
*/
void CNvs::loadWifiInfo(char* ssid, char* password, bool &hasPassword)
{
    PREFS.begin(NAME, true);
    String ssidStr = PREFS.getString("wifiSSID", "");
    String passwordStr = PREFS.getString("wifiPassword", "");
    hasPassword = PREFS.getBool("wifiHasPassword", false);
    strncpy(ssid, ssidStr.c_str(), 40);
    strncpy(password, passwordStr.c_str(), 40);
    PREFS.end();
}   
