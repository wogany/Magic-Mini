#include "web.h"
#include "nvs.h"
#include "ws2812b.h"
#include <ESPmDNS.h>
#include <Update.h>

extern CNvs NVS;     // 外部NVS存储对象
extern CWs2812b RGB; // 外部WS2812B对象

const char CWeb::m_ssidAp[] = "Magic-Mini";
const char CWeb::m_passwordAp[] = "12345678";
const char CWeb::m_webName[] = "magic-mini";

const char *pageWifiConfig = "<!DOCTYPE html>"
                             "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>Magic-Mini 配网</title>"
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
                             "<div class='label'>WiFi名称</div>"
                             "<input type='text' name='ssid' placeholder='输入 WiFi 名称'/>"
                             "<div class='label'>WiFi密码</div>"
                             "<input type='password' name='password' placeholder='输入 WiFi 密码'/>"
                             "<button type='submit'>确认</button>"
                             "</form>"
                             "</div>"
                             "</body></html>";

const char *pageConfigSuccess = "<!DOCTYPE html><html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                                "<title>配网成功</title>"
                                "<style>"
                                "html,body{height:100%;margin:0;} "
                                "body{font-family:\"Microsoft YaHei\",Arial,Helvetica,sans-serif;display:flex;justify-content:center;align-items:flex-start;background:#fff;} "
                                ".container{display:flex;flex-direction:column;align-items:center;padding-top:80px;box-sizing:border-box;} "
                                "h1{font-size:36px;color:#4CAF50;margin:0 0 12px 0;} "
                                "p{font-size:20px;color:#333;margin:0;} "
                                "</style>"
                                "</head><body><div class='container'><h1>配网成功</h1><p>5秒后自动重启……</p></div></body></html>";

const char *pageConfigFail = "<!DOCTYPE html><html><head><meta charset='gbk'>"
                             "<meta http-equiv='refresh' content='3; URL=/'/>"
                             "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                             "<title>配网失败</title>"
                             "<style>"
                             "html,body{height:100%;margin:0;} "
                             "body{font-family:\"Microsoft YaHei\",Arial,Helvetica,sans-serif;display:flex;justify-content:center;align-items:center;background:#fff;} "
                             ".box{display:flex;flex-direction:column;align-items:center;text-align:center;padding:20px;box-sizing:border-box;} "
                             "h1{font-size:32px;color:#f44336;margin:0 0 8px 0;} "
                             "p{font-size:18px;color:#333;margin:0 0 8px 0;} "
                             ".count{font-size:16px;color:#666;margin-top:8px;}"
                             "</style>"
                             "</head><body><div class='box'><h1>配网失败</h1><p>正在返回</p><div class='count' id='count'>3 秒后返回</div></div>"
                             "<script>"
                             " (function(){"
                             "  var t=3; var el=document.getElementById('count');"
                             "  var iv=setInterval(function(){ t--; if(t<=0){ clearInterval(iv); return; } el.innerText = t + ' 秒后返回'; },1000);"
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
                       "<a href='/ota' class='btn'>OTA升级</a>"
                       "</div>"
                       "</body></html>";

const char *pageOTA = "<!DOCTYPE html>"
                      "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                      "<title>Magic-Mini OTA升级</title>"
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
                      "<h1>OTA 固件升级</h1>"
                      "<form id='uploadForm' method='post' action='/update' enctype='multipart/form-data' onsubmit='showLoading()'>"
                      "<div class='file-container'>"
                      "<input type='file' name='update' accept='.bin' required />"
                      "<div class='file-info'>请选择固件文件 (.bin)</div>"
                      "</div>"
                      "<button type='submit' id='uploadBtn'>开始升级</button>"
                      "</form>"
                      "<div id='loading' class='loading'>"
                      "<div class='spinner'></div>"
                      "<div class='loading-text'>正在上传固件，请稍候...</div>"
                      "</div>"
                      "<a href='/' id='backLink'>返回首页</a>"
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
                             "<title>升级成功</title>"
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
                             "<h1>固件升级成功</h1>"
                             "<p>新固件已成功写入</p>"
                             "<p class='timer'>设备将在5秒后自动重启...</p>"
                             "</div>"
                             "<script>"
                             "let seconds = 5;"
                             "const timer = document.querySelector('.timer');"
                             "const interval = setInterval(() => {"
                             "  seconds--;"
                             "  timer.textContent = `设备将在${seconds}秒后自动重启...`;"
                             "  if (seconds <= 0) {"
                             "    clearInterval(interval);"
                             "    window.location.href = '/';"
                             "  }"
                             "}, 1000);"
                             "</script>"
                             "</body></html>";

const char *pageOTAFail = "<!DOCTYPE html>"
                          "<html><head><meta charset='gbk'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                          "<title>升级失败</title>"
                          "<style>"
                          "html,body{height:100%;margin:0;} "
                          "body{font-family:Arial,Helvetica,sans-serif;background:#fff;display:flex;justify-content:center;align-items:flex-start;} "
                          ".wrap{width:100%;max-width:420px;display:flex;flex-direction:column;align-items:center;box-sizing:border-box;padding-top:80px;} "
                          ".icon{width:80px;height:80px;border-radius:50%;background:#f44336;display:flex;justify-content:center;align-items:center;margin-bottom:24px;} "
                          ".icon:before{content:'×';color:#fff;font-size:48px;} "
                          "h1{font-size:28px;margin:0 0 16px 0;text-align:center;color:#222;} "
                          "p{font-size:16px;margin:0 0 24px 0;text-align:center;color:#666;} "
                          ".btn{display:inline-block;padding:10px 24px;font-size:16px;text-decoration:none;color:#fff;background:#007bff;border-radius:6px;transition:background 0.3s;} "
                          ".btn:hover{background:#0056b3;} "
                          "</style></head><body>"
                          "<div class='wrap'>"
                          "<div class='icon'></div>"
                          "<h1>固件升级失败</h1>"
                          "<p>无法写入固件，请检查文件后重试</p>"
                          "<a href='/ota' class='btn'>返回重试</a>"
                          "</div>"
                          "</body></html>";

/*
    @brief  构造函数
    @param  无
    @return 无
*/
CWeb::CWeb(void) : SERVER(80)
{
}

/*
    @brief  析构函数
    @param  无
    @return 无
*/
CWeb::~CWeb(void)
{
}

/*
    @brief  wifi初始化
    @param  无
    @return 成功与否
*/
bool CWeb::init(void)
{
    NVS.getWifiState(m_wifiState);
    if (m_wifiState)
    {
        /* 使用STA模式 */
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

        /* 等待连接 */
        unsigned long start = millis();
        const unsigned long timeout = 10000; // 10 秒
        while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout)
        {
            delay(200);
        }

        /* 连接失败，关闭wifi */
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.disconnect(true, true);
            return false;
        }
    }
    else
    {
        /* 使用AP模式 */
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(m_ssidAp, m_passwordAp, 1, 0, 1);
    }

    /* 使用mDNS */
    MDNS.begin(m_webName);

    return true;
}

/*
    @brief  Web服务器初始化
    @param  无
    @return 无
*/
void CWeb::begin(void)
{
    if (!m_wifiState)
    {
        /* 配网模式 */
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
    @brief  Web服务器循环处理
    @param  无
    @return 无
*/
void CWeb::loop(void)
{
    SERVER.handleClient();
}

/*
    @brief  WiFi配网网页处理函数
    @param  无
    @return 无
*/
void CWeb::m_handleWifiConfig(void)
{
    SERVER.send(200, "text/html; charset=gbk", pageWifiConfig);
}

/*
    @brief  连接WiFi处理函数
    @param  无
    @return 无
*/
void CWeb::m_handleConnect(void)
{
    String ssid = SERVER.arg("ssid");
    String password = SERVER.arg("password");

    /* 检查是否有SSID */
    if (ssid.length() == 0)
    {
        SERVER.send(200, "text/html; charset=gbk", pageConfigFail);
        return;
    }

    /* 尝试连接WiFi */
    if (password.length() > 0)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
    }
    else
    {
        WiFi.begin(ssid.c_str());
    }

    unsigned long start = millis();
    const unsigned long timeout = 10000; // 10 秒
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout)
    {
        delay(200);
    }

    /* 连接失败，重新配网 */
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect(true, true);
        SERVER.send(200, "text/html; charset=gbk", pageConfigFail);
        return;
    }

    /* 连接成功 */
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
    @brief  根目录处理函数
    @param  无
    @return 无
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
    @brief  ota升级状态页面处理函数
    @param  无
    @return 无
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
    @brief  OTA升级文件上传处理函数
    @param  无
    @return 无
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