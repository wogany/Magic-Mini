#include "web.h"
#include "nvs.h"

extern CNvs NVS; // �ⲿNVS�洢����

const char CWeb::m_ssidAp[] = "Magic-Mini";
const char CWeb::m_passwordAp[] = "12345678";

const char *pageWifiConfig = "<!DOCTYPE html>"
                             "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>Magic-Mini ����</title>"
                             "<style>"
                             "body{font-family:Arial,Helvetica,sans-serif;margin:0;padding:0;}"
                             ".container{display:flex;flex-direction:column;align-items:center;margin-top:60px;padding:0 20px;}"
                             "h1{font-size:28px;margin:0 0 20px 0;text-align:center;}"
                             "label{font-size:16px;margin-top:12px;margin-bottom:6px;}"
                             "input[type=text],input[type=password]{width:100%;max-width:320px;padding:10px;font-size:16px;border:1px solid #ccc;border-radius:4px;box-sizing:border-box;}"
                             "button{margin-top:18px;padding:10px 24px;font-size:16px;border:none;border-radius:4px;background:#007bff;color:#fff;cursor:pointer;}"
                             "button:active{transform:translateY(1px);}"
                             "</style></head><body>"
                             "<div class='container'>"
                             "<h1>Magic-Mini����</h1>"
                             "<label>WiFi����</label>"
                             "<input type='text' name='ssid' id='ssid' placeholder='���� WiFi ����'/>"
                             "<label>WiFi����</label>"
                             "<input type='password' name='password' id='password' placeholder='���� WiFi ����'/>"
                             "<button onclick='doSubmit()'>ȷ��</button>"
                             "</div>"
                             "<script>"
                             "function doSubmit(){"
                             "  var s=document.getElementById('ssid').value;"
                             "  var p=document.getElementById('password').value;"
                             "  var xhr=new XMLHttpRequest();"
                             "  xhr.open('POST','/connect',true);"
                             "  xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');"
                             "  xhr.onreadystatechange=function(){ if(xhr.readyState==4){ /* �ɸ��ݷ��ش��� */ } };"
                             "  xhr.send('ssid='+encodeURIComponent(s)+'&password='+encodeURIComponent(p));"
                             "}"
                             "</script>"
                             "</body></html>";

const char *pageConfigSuccess = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                                "<title>�����ɹ�</title>"
                                "<style>body{margin:0;height:100vh;display:flex;align-items:center;justify-content:center;font-family:Arial,Helvetica,sans-serif}h1{font-size:24px}</style>"
                                "</head><body><div><h1>�����ɹ�����������</h1></div></body></html>";

const char *pageConfigFail = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
                             "<meta http-equiv='refresh' content='5; URL=/'/>"
                             "<title>����ʧ��</title>"
                             "<style>body{margin:0;height:100vh;display:flex;align-items:center;justify-content:center;font-family:Arial,Helvetica,sans-serif}h1{font-size:24px}</style>"
                             "</head><body><div><h1>����ʧ�ܣ�����������</h1></div></body></html>";

/*
    @brief  ���캯��
    @param  ��
    @return ��
*/
CWeb::CWeb(void) : SERVER(80)
{
}

/*
    @brief  ��������
    @param  ��
    @return ��
*/
CWeb::~CWeb(void)
{
}

/*
    @brief  wifi��ʼ��
    @param  ��
    @return �ɹ����
*/
bool CWeb::init(void)
{
    NVS.getWifiState(m_wifiState);
    if (m_wifiState)
    {
        /* ʹ��STAģʽ */
        WiFi.mode(WIFI_STA);
        WiFi.setHostname("Magic-Mini");
        NVS.loadWifiInfo(m_ssidSta, m_passwordSta, m_haveStaPassword);
        if (m_haveStaPassword)
        {
            WiFi.begin(m_ssidSta, m_passwordSta);
        }
        else
        {
            WiFi.begin(m_ssidSta);
        }

        /* �ȴ����� */
        unsigned long start = millis();
        const unsigned long timeout = 10000; // 10 ��
        while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout)
        {
            delay(200);
        }

        /* ����ʧ�ܣ��ر�wifi */
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.disconnect(true, true);
            return false;
        }
    }
    else
    {
        /* ʹ��APģʽ */
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(m_ssidAp, m_passwordAp, 1, 0, 1);
    }

    return true;
}

/*
    @brief  Web��������ʼ��
    @param  ��
    @return ��
*/
void CWeb::begin(void)
{
    if (!m_wifiState)
    {
        /* ����ģʽ */
        SERVER.on("/", [this]()
                  { m_handleWifiConfig(); });
        SERVER.on("/connect", [this]()
                  { m_handleConnect(); });
    }
    else
    {
        /* ����ģʽ�´�ӡIP��ַ */
    }

    SERVER.begin();
}

/*
    @brief  Web������ѭ������
    @param  ��
    @return ��
*/
void CWeb::loop(void)
{
    SERVER.handleClient();
}

/*
    @brief  WiFi������ҳ������
    @param  ��
    @return ��
*/
void CWeb::m_handleWifiConfig(void)
{
    SERVER.send(200, "text/html;charset=utf-8", pageWifiConfig);
}

/*
    @brief  ����WiFi������
    @param  ��
    @return ��
*/
void CWeb::m_handleConnect(void)
{
    String ssid = SERVER.arg("ssid");
    String password = SERVER.arg("password");

    /* ����Ƿ���SSID */
    if (ssid.length() == 0)
    {
        SERVER.send(200, "text/html", pageConfigFail);
        return;
    }

    /* ��������WiFi */
    if (password.length() > 0)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
    }
    else
    {
        WiFi.begin(ssid.c_str());
    }

    unsigned long start = millis();
    const unsigned long timeout = 10000; // 10 ��
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout)
    {
        delay(200);
    }

    /* ����ʧ�ܣ��ر�wifi */
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect(true, true);
        SERVER.send(200, "text/html", pageConfigFail);
        return;
    }

    /* ���ӳɹ� */
    SERVER.send(200, "text/html", pageConfigSuccess);
    if (password.length() > 0)
    {
        NVS.saveWifiInfo(ssid.c_str(), password.c_str(), true);
    }
    else
    {
        NVS.saveWifiInfo(ssid.c_str(), "", false);
    }
    NVS.saveWifiState(true);
    delay(3000);
    ESP.restart();
}