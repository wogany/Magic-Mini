#include "nvs.h"

#define NAME "my_config"

/*
    @brief  ���캯��
    @param  ��
    @return ��
*/
CNvs::CNvs(void)
{
}

/*
    @brief  ��������
    @param  ��
    @return ��
*/
CNvs::~CNvs(void)
{
}

/*
    @brief  NVS��ʼ��
    @param  ��
    @return �ɹ����
*/
bool CNvs::init(void)
{
    PREFS.begin(NAME, false);
    PREFS.end();
    return true;
}

/*
    @brief  ������ɫ��NVS
    @param  r ��ɫ����
    @param  g ��ɫ����
    @param  b ��ɫ����
    @return ��
*/
void CNvs::saveColor(uint8_t r, uint8_t g, uint8_t b)
{
    PREFS.begin(NAME, false);
    uint8_t lastR, lastG, lastB;
    /*  ��ȡ�ϴα������ɫ */
    lastR = PREFS.getUChar("colorR", 0);
    lastG = PREFS.getUChar("colorG", 0);
    lastB = PREFS.getUChar("colorB", 0);
    /* �����ɫ�Ƿ�仯 */
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
    @brief  ��NVS������ɫ
    @param  r ��ɫ��������
    @param  g ��ɫ��������
    @param  b ��ɫ��������
    @return ��
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
    @brief  ����WiFi����״̬��NVS
    @param  state ����״̬
    @return ��
*/
void CNvs::saveWifiState(bool state)
{
    PREFS.begin(NAME, false);
    PREFS.putBool("wifiState", state);
    PREFS.end();
}

/*
    @brief  ��NVS��ȡWiFi����״̬
    @param  state ����״̬
    @return ��
*/
void CNvs::getWifiState(bool &state)
{
    PREFS.begin(NAME, true);
    state = PREFS.getBool("wifiState", false);
    PREFS.end();
}

/*
    @brief  ����WiFi��Ϣ��NVS
    @param  ssid WiFi����
    @param  password WiFi����
    @param  hasPassword �Ƿ�������
    @return ��
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
    @brief  ��NVS����WiFi��Ϣ
    @param  ssid WiFi����
    @param  password WiFi����
    @param  hasPassword �Ƿ�������
    @return ��
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
