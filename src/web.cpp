#include "web.h"
#include "nvs.h"
#include "ws2812b.h"

extern CNvs NVS;     // 外部NVS存储对象
extern CWs2812b RGB; // 外部WS2812B对象

const char CWeb::m_ssidAp[] = "Magic-Mini";
const char CWeb::m_passwordAp[] = "12345678";

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
        /* 连接模式下打印IP地址 */
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
