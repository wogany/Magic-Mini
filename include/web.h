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
    static const char m_webName[];    // web����
    char m_ssidSta[40];               // WiFiվ������
    char m_passwordSta[40];           // WiFiվ������
    bool m_isOTASuccess;              // OTA�����Ƿ�ɹ�

    void m_handleWifiConfig(void); // WiFi������ҳ������
    void m_handleConnect(void);    // ����WiFi������
    void m_handleRoot(void);       // ��Ŀ¼������
    void m_handleOta(void);        // ota����ҳ�洦����
    void m_handleOtaState(void);   // ota����״̬ҳ�洦����
    void m_handleOtaUpload(void);  // OTA�����ļ��ϴ�������

public:
    CWeb(void);
    ~CWeb(void);

    bool init(void);
    void begin(void);
    void loop(void);
};

#endif