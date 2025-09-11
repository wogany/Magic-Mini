#include "web.h"
#include "nvs.h"
#include "ws2812b.h"
#include <ESPmDNS.h>
#include <Update.h>

extern CNvs NVS;     // �ⲿNVS�洢����
extern CWs2812b RGB; // �ⲿWS2812B����

const char CWeb::m_ssidAp[] = "Magic-Mini";
const char CWeb::m_passwordAp[] = "12345678";
const char CWeb::m_webName[] = "magic-mini";

const char *pageWifiConfig = "<!DOCTYPE html>"
                             "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>Magic-Mini ����</title>"
                             "<style>"
                             "html,body{height:100%;margin:0;} "
                             "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                             ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:60px;padding-left:16px;padding-right:16px;} "
                             "h1{font-size:34px;margin:0 0 18px 0;text-align:center;color:#222;} "
                             ".label{width:100%;max-width:360px;align-self:center;font-size:16px;color:#333;margin:8px 0 6px 0;padding-left:6px;} "
                             "form{width:100%;max-width:360px;display:flex;flex-direction:column;align-items:center;} "
                             "input[type=text],input[type=password]{width:100%;padding:12px;font-size:16px;border:1px solid #ccc;border-radius:6px;box-sizing:border-box;} "
                             "button{margin-top:20px;padding:12px 28px;font-size:18px;border:none;border-radius:6px;background:#007bff;color:#fff;cursor:pointer;} "
                             "</style></head><body>"
                             "<div class='wrap'>"
                             "<h1>Magic-Mini</h1>"
                             "<form action='/connect' method='POST'>"
                             "<div class='label'>WiFi����</div>"
                             "<input type='text' name='ssid' placeholder='���� WiFi ����'/>"
                             "<div class='label'>WiFi����</div>"
                             "<input type='password' name='password' placeholder='���� WiFi ����'/>"
                             "<button type='submit'>ȷ��</button>"
                             "</form>"
                             "</div>"
                             "</body></html>";

const char *pageConfigSuccess = "<!DOCTYPE html><html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                                "<title>�����ɹ�</title>"
                                "<style>"
                                "html,body{height:100%;margin:0;} "
                                "body{font-family:\"Microsoft YaHei\",Arial,Helvetica,sans-serif;display:flex;justify-content:center;align-items:flex-start;background:#fff;} "
                                ".container{display:flex;flex-direction:column;align-items:center;padding-top:80px;box-sizing:border-box;} "
                                "h1{font-size:36px;color:#4CAF50;margin:0 0 12px 0;} "
                                "p{font-size:20px;color:#333;margin:0;} "
                                "</style>"
                                "</head><body><div class='container'><h1>�����ɹ�</h1><p>5����Զ���������</p></div></body></html>";

const char *pageConfigFail = "<!DOCTYPE html><html><head><meta charset='gbk'>"
                             "<meta http-equiv='refresh' content='3; URL=/'/>"
                             "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>����ʧ��</title>"
                             "<style>"
                             "html,body{height:100%;margin:0;} "
                             "body{font-family:\"Microsoft YaHei\",Arial,Helvetica,sans-serif;display:flex;justify-content:center;align-items:center;background:#fff;} "
                             ".box{display:flex;flex-direction:column;align-items:center;text-align:center;padding:20px;box-sizing:border-box;} "
                             "h1{font-size:32px;color:#f44336;margin:0 0 8px 0;} "
                             "p{font-size:18px;color:#333;margin:0 0 8px 0;} "
                             ".count{font-size:16px;color:#666;margin-top:8px;}"
                             "</style>"
                             "</head><body><div class='box'><h1>����ʧ��</h1><p>���ڷ���</p><div class='count' id='count'>3 ��󷵻�</div></div>"
                             "<script>"
                             " (function(){"
                             "  var t=3; var el=document.getElementById('count');"
                             "  var iv=setInterval(function(){ t--; if(t<=0){ clearInterval(iv); return; } el.innerText = t + ' ��󷵻�'; },1000);"
                             "  setTimeout(function(){ window.location.href = '/'; }, 3000);"
                             " })();"
                             "</script>"
                             "</body></html>";

const char *pageRoot = "<!DOCTYPE html>"
                       "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                       "<title>Magic-Mini</title>"
                       "<style>"
                       "html,body{height:100%;margin:0;} "
                       "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                       ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:60px;padding-left:16px;padding-right:16px;} "
                       "h1{font-size:42px;margin:0 0 40px 0;text-align:center;color:#222;} "
                       ".btn{display:inline-block;padding:12px 28px;font-size:18px;text-decoration:none;color:#fff;background:#007bff;border-radius:6px;transition:background 0.3s;} "
                       ".btn:hover{background:#0056b3;} "
                       "</style></head><body>"
                       "<div class='wrap'>"
                       "<h1>Magic-Mini</h1>"
                       "<a href='/ota' class='btn'>OTA����</a>"
                       "</div>"
                       "</body></html>";

const char *pageOTA = "<!DOCTYPE html>"
                      "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                      "<title>Magic-Mini OTA����</title>"
                      "<style>"
                      "html,body{height:100%;margin:0;} "
                      "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                      ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:60px;padding-left:16px;padding-right:16px;} "
                      "h1{font-size:32px;margin:0 0 24px 0;text-align:center;color:#222;} "
                      "form{width:100%;display:flex;flex-direction:column;align-items:center;} "
                      ".file-container{width:100%;margin-bottom:24px;text-align:center;} "
                      ".file-info{margin-top:8px;font-size:14px;color:#666;} "
                      "input[type=file]{width:100%;padding:10px;margin-top:10px;} "
                      "button{padding:12px 28px;font-size:18px;border:none;border-radius:6px;background:#007bff;color:#fff;cursor:pointer;transition:background 0.3s;} "
                      "button:hover{background:#0056b3;} "
                      "a{margin-top:20px;font-size:16px;color:#007bff;text-decoration:none;} "
                      ".loading{display:none;flex-direction:column;align-items:center;margin-top:24px;} "
                      ".spinner{width:40px;height:40px;border:4px solid rgba(0,123,255,0.2);border-radius:50%;border-top:4px solid #007bff;animation:spin 1s linear infinite;} "
                      ".loading-text{margin-top:12px;font-size:16px;color:#666;} "
                      "@keyframes spin{0%{transform:rotate(0deg);}100%{transform:rotate(360deg);}} "
                      "</style></head><body>"
                      "<div class='wrap'>"
                      "<h1>OTA �̼�����</h1>"
                      "<form id='uploadForm' method='post' action='/update' enctype='multipart/form-data' onsubmit='showLoading()'>"
                      "<div class='file-container'>"
                      "<input type='file' name='update' accept='.bin' required />"
                      "<div class='file-info'>��ѡ��̼��ļ� (.bin)</div>"
                      "</div>"
                      "<button type='submit' id='uploadBtn'>��ʼ����</button>"
                      "</form>"
                      "<div id='loading' class='loading'>"
                      "<div class='spinner'></div>"
                      "<div class='loading-text'>�����ϴ��̼������Ժ�...</div>"
                      "</div>"
                      "<a href='/' id='backLink'>������ҳ</a>"
                      "</div>"
                      "<script>"
                      "function showLoading() {"
                      "  document.getElementById('uploadBtn').disabled = true;"
                      "  document.getElementById('loading').style.display = 'flex';"
                      "  document.getElementById('backLink').style.display = 'none';"
                      "  return true;"
                      "}"
                      "document.getElementById('uploadForm').addEventListener('submit', showLoading);"
                      "</script>"
                      "</body></html>";

const char *pageOTASuccess = "<!DOCTYPE html>"
                             "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>�����ɹ�</title>"
                             "<style>"
                             "html,body{height:100%;margin:0;} "
                             "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                             ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:80px;} "
                             ".icon{width:80px;height:80px;border-radius:50%;background:#4CAF50;display:flex;justify-content:center;align-items:center;margin-bottom:24px;} "
                             ".icon:before{content:'?';color:#fff;font-size:48px;} "
                             "h1{font-size:28px;margin:0 0 16px 0;text-align:center;color:#222;} "
                             "p{font-size:16px;margin:0;text-align:center;color:#666;} "
                             ".timer{margin-top:16px;font-size:16px;color:#888;} "
                             "</style></head><body>"
                             "<div class='wrap'>"
                             "<div class='icon'></div>"
                             "<h1>�̼������ɹ�</h1>"
                             "<p>�¹̼��ѳɹ�д��</p>"
                             "<p class='timer'>�豸����5����Զ�����...</p>"
                             "</div>"
                             "<script>"
                             "let seconds = 5;"
                             "const timer = document.querySelector('.timer');"
                             "const interval = setInterval(() => {"
                             "  seconds--;"
                             "  timer.textContent = `�豸����${seconds}����Զ�����...`;"
                             "  if (seconds <= 0) {"
                             "    clearInterval(interval);"
                             "    window.location.href = '/';"
                             "  }"
                             "}, 1000);"
                             "</script>"
                             "</body></html>";

const char *pageOTAFail = "<!DOCTYPE html>"
                          "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                          "<title>����ʧ��</title>"
                          "<style>"
                          "html,body{height:100%;margin:0;} "
                          "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                          ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:80px;} "
                          ".icon{width:80px;height:80px;border-radius:50%;background:#f44336;display:flex;justify-content:center;align-items:center;margin-bottom:24px;} "
                          ".icon:before{content:'��';color:#fff;font-size:48px;} "
                          "h1{font-size:28px;margin:0 0 16px 0;text-align:center;color:#222;} "
                          "p{font-size:16px;margin:0 0 24px 0;text-align:center;color:#666;} "
                          ".btn{display:inline-block;padding:10px 24px;font-size:16px;text-decoration:none;color:#fff;background:#007bff;border-radius:6px;transition:background 0.3s;} "
                          ".btn:hover{background:#0056b3;} "
                          "</style></head><body>"
                          "<div class='wrap'>"
                          "<div class='icon'></div>"
                          "<h1>�̼�����ʧ��</h1>"
                          "<p>�޷�д��̼��������ļ�������</p>"
                          "<a href='/ota' class='btn'>��������</a>"
                          "</div>"
                          "</body></html>";

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

    /* ʹ��mDNS */
    MDNS.begin(m_webName);

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
        SERVER.on("/", [this]()
                  { m_handleRoot(); });
        SERVER.on("/ota", [this]()
                  { m_handleOta(); });
        SERVER.on("/update", HTTP_POST, [this]()
                  { m_handleOtaState(); }, [this]()
                  { m_handleOtaUpload(); });
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
    SERVER.send(200, "text/html; charset=gbk", pageWifiConfig);
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
        SERVER.send(200, "text/html; charset=gbk", pageConfigFail);
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

    /* ����ʧ�ܣ��������� */
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect(true, true);
        SERVER.send(200, "text/html; charset=gbk", pageConfigFail);
        return;
    }

    /* ���ӳɹ� */
    SERVER.send(200, "text/html; charset=gbk", pageConfigSuccess);
    if (password.length() > 0)
    {
        NVS.saveWifiInfo(ssid.c_str(), password.c_str(), true);
    }
    else
    {
        NVS.saveWifiInfo(ssid.c_str(), "", false);
    }
    NVS.saveWifiState(true);
    delay(4000);
    RGB.setAllPixelColor(0, 0, 0);
    delay(1000);
    ESP.restart();
}

/*
    @brief  ��Ŀ¼������
    @param  ��
    @return ��
*/
void CWeb::m_handleRoot(void)
{
    SERVER.send(200, "text/html; charset=gbk", pageRoot);
}

void CWeb::m_handleOta(void)
{
    SERVER.send(200, "text/html; charset=gbk", pageOTA);
}

/*
    @brief  ota����״̬ҳ�洦����
    @param  ��
    @return ��
*/
void CWeb::m_handleOtaState(void)
{
    if (m_isOTASuccess)
    {
        SERVER.send(200, "text/html; charset=gbk", pageOTASuccess);
        delay(4000);
        RGB.setAllPixelColor(0, 0, 0);
        delay(1000);
        ESP.restart();
    }
    else
    {
        SERVER.send(200, "text/html; charset=gbk", pageOTAFail);
    }
}

/*
    @brief  OTA�����ļ��ϴ�������
    @param  ��
    @return ��
*/
void CWeb::m_handleOtaUpload(void)
{
    HTTPUpload &upload = SERVER.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if (!filename.startsWith("/"))
        {
            filename = "/" + filename;
        }
        Update.begin(UPDATE_SIZE_UNKNOWN);
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        Update.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (Update.end(true))
        {
            m_isOTASuccess = true;
        }
        else
        {
            m_isOTASuccess = false;
        }
    }
}