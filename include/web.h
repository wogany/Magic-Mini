#ifndef _WEB_H
#define _WEB_H

#include <WiFi.h>
#include <WebServer.h>

class CWeb
{
private:
    WebServer SERVER; // ����WebServer����

    bool m_wifiState;                 // WiFi����״̬
    bool m_haveStaPassword;           // �Ƿ���STA����
    static const char m_ssidAp[];     // WiFi�ȵ�����
    static const char m_passwordAp[]; // WiFi�ȵ�����
    char m_ssidSta[40];               // WiFiվ������
    char m_passwordSta[40];           // WiFiվ������

    void m_handleWifiConfig(void); // WiFi������ҳ������
    void m_handleConnect(void);    // ����WiFi������

public:
    CWeb(void);
    ~CWeb(void);

    bool init(void);
    void begin(void);
    void loop(void);
};

#endif