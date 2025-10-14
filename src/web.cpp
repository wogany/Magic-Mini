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
                             "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
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

const char *pageConfigSuccess = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
                                "<title>配网成功</title>"
                                "<style>"
                                "html,body{height:100%;margin:0;} "
                                "body{font-family:\"Microsoft YaHei\",Arial,Helvetica,sans-serif;display:flex;justify-content:center;align-items:flex-start;background:#fff;} "
                                ".container{display:flex;flex-direction:column;align-items:center;padding-top:80px;box-sizing:border-box;} "
                                "h1{font-size:36px;color:#4CAF50;margin:0 0 12px 0;} "
                                "p{font-size:20px;color:#333;margin:0;} "
                                "</style>"
                                "</head><body><div class='container'><h1>配网成功</h1><p>5秒后自动重启……</p></div></body></html>";

const char *pageConfigFail = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
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
                       "<html>"
                       "<head>"
                       "<meta charset=\"utf-8\" />"
                       "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />"
                       "<title>Magic-Mini</title>"
                       "<style>"
                       "html,"
                       "body {height: 100%; margin: 0;}"
                       "body {font-family: Arial, Helvetica, sans-serif; background: #fff; display: flex; justify-content: center; align-items: flex-start;}"
                       ".wrap {width: 100%; max-width: 420px; display: flex; flex-direction: column; align-items: center; box-sizing: border-box; padding-top: 60px; padding-left: 16px; padding-right: 16px;}"
                       "h1 {font-size: 42px; margin: 0 0 40px 0; text-align: center; color: #222;}"
                       ".btn {display: inline-block; padding: 12px 28px; font-size: 18px; text-decoration: none; color: #fff; border-radius: 6px; transition: all 0.3s; margin-bottom: 20px; opacity: 1;}"
                       ".btn:hover {opacity: 0.9; transform: scale(0.99); transform:translateY(-2px);}"
                       "</style>"
                       "</head>"
                       "<body>"
                       "<div class=\"wrap\">"
                       "<h1>Magic-Mini</h1>"
                       "<a href=\"/color\" class=\"btn\" style=\"background-color: #007bff;\">调整颜色</a>"
                       "<a href=\"/ota\" class=\"btn\" style=\"background-color: #28a745;\">OTA升级</a>"
                       "</div>"
                       "</body>"
                       "</html>";

const char *pageOTA = "<!DOCTYPE html>"
                      "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
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
                             "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
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
                          "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
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

const char *pageColor = "<!DOCTYPE html>"
                        "<html lang=\"zh-CN\">"
                        "<head>"
                        "<meta charset=\"UTF-8\" />"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />"
                        "<title>Magic-Mini 调光控制</title>"
                        "<style>"
                        "* {margin: 0; padding: 0; box-sizing: border-box; font-family: \"Arial\", \"华文细黑\";}"
                        "body {min-height: 100dvh; max-width: 100vw; padding: 20px;}"
                        ".container {margin: 0 auto; width: 100%; padding: 20px; max-width: 450px;}"
                        "h1 {text-align: center; color: rgb(0, 123, 255); margin: 10px 0 20px 0; font-size: 24px;}"
                        ".color-wheel-container {position: relative; width: 100%; padding-top: 100%; margin-bottom: 30px;}"
                        "#colorWheel {position: absolute; top: 0; left: 0; width: 100%; height: 100%; border-radius: 50%; box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);}"
                        "#colorSelector {position: absolute; width: 20px; height: 20px; border: 2px solid white; border-radius: 50%; box-shadow: 0 0 5px rgba(0, 0, 0, 0.5); transform: translate(-50%, -50%); cursor: grab; user-select: none;}"
                        "#colorSelector:active {cursor: grabbing;}"
                        ".current-color {width: 100%; height: 60px; border-radius: 8px; margin-bottom: 20px; display: flex; align-items: center; justify-content: center; color: white; font-size: 16px; font-weight: bold; text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.7);}"
                        ".controls {display: flex; flex-direction: column; gap: 15px;}"
                        "button {border: none; border-radius: 8px; font-weight: bold; cursor: pointer; padding: 12px; font-size: 14px;}"
                        "#setColorBtn {background-color: rgb(0, 123, 255); color: white;}"
                        "#dialog {position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); width: 150px; height: 150px; border: none; outline: none; border-radius: 12px; background-color: rgba(0, 0, 0, 0.3); color: white;}"
                        "#dialogMessage {font-size: 16px; font-weight: bold; text-align: center; line-height: 150px;}"
                        "</style>"
                        "</head>"
                        "<body>"
                        "<div class=\"container\">"
                        "<h1>Magic-Mini 调光控制</h1>"
                        "<div class=\"color-wheel-container\">"
                        "<canvas id=\"colorWheel\"></canvas>"
                        "<div id=\"colorSelector\"></div>"
                        "</div>"
                        "<div class=\"current-color\" id=\"currentColor\">RGB(0, 0, 0)</div>"
                        "<div class=\"controls\">"
                        "<button id=\"setColorBtn\">应用颜色</button>"
                        "</div>"
                        "</div>"
                        "<dialog id=\"dialog\">"
                        "<p id=\"dialogMessage\">message</p>"
                        "</dialog>"
                        "<script>"
                        "let canvas, ctx; "
                        "let colorWheel, colorSelector, currentColor; "
                        "let setColorBtn; "
                        "let isDragging = false; "
                        "let centerX, centerY, radius; "
                        "let deviceAddress = \"magic-mini.local\"; "
                        "let isConnected = false; "
                        "let dialog, dialogMessage;"
                        "function init() {"
                        "colorWheel = document.getElementById(\"colorWheel\");"
                        "colorSelector = document.getElementById(\"colorSelector\");"
                        "currentColor = document.getElementById(\"currentColor\");"
                        "setColorBtn = document.getElementById(\"setColorBtn\");"
                        "dialog = document.getElementById(\"dialog\");"
                        "dialogMessage = document.getElementById(\"dialogMessage\");"
                        "canvas = colorWheel;"
                        "ctx = canvas.getContext(\"2d\");"
                        "redrawCanvas();"
                        "window.addEventListener(\"resize\", redrawCanvas); "
                        "colorWheel.addEventListener(\"mousedown\", startDrag);"
                        "colorWheel.addEventListener(\"touchstart\", startDrag, { passive: true });"
                        "document.addEventListener(\"mousemove\", drag);"
                        "document.addEventListener(\"touchmove\", drag, { passive: false });"
                        "document.addEventListener(\"mouseup\", endDrag);"
                        "document.addEventListener(\"touchend\", endDrag);"
                        "setColorBtn.addEventListener(\"click\", setColor);"
                        "getColor();"
                        "}"
                        "function redrawCanvas() {"
                        "const container = colorWheel.parentElement;"
                        "const size = container.clientWidth;"
                        "canvas.width = size;"
                        "canvas.height = size;"
                        "centerX = size / 2; "
                        "centerY = size / 2; "
                        "radius = size / 2 - 5; "
                        "drawColorWheel();"
                        "}"
                        "function drawColorWheel() {"
                        "const gradient = ctx.createConicGradient(0, centerX, centerY); "
                        "gradient.addColorStop(0, \"#ff0000\"); "
                        "gradient.addColorStop(1 / 6, \"#ffff00\"); "
                        "gradient.addColorStop(2 / 6, \"#00ff00\"); "
                        "gradient.addColorStop(3 / 6, \"#00ffff\"); "
                        "gradient.addColorStop(4 / 6, \"#0000ff\"); "
                        "gradient.addColorStop(5 / 6, \"#ff00ff\"); "
                        "gradient.addColorStop(1, \"#ff0000\"); "
                        "ctx.beginPath();"
                        "ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);"
                        "ctx.fillStyle = gradient;"
                        "ctx.fill();"
                        "const innerRadius = radius * 0.8;"
                        "const innerGradient = ctx.createRadialGradient("
                        "centerX,"
                        "centerY,"
                        "0,"
                        "centerX,"
                        "centerY,"
                        "innerRadius"
                        ");"
                        "innerGradient.addColorStop(0, \"#ffffff\");"
                        "innerGradient.addColorStop(1, \"rgba(255, 255, 255, 0)\");"
                        "ctx.beginPath();"
                        "ctx.arc(centerX, centerY, innerRadius, 0, 2 * Math.PI);"
                        "ctx.fillStyle = innerGradient;"
                        "ctx.fill();"
                        "}"
                        "function startDrag(e) {"
                        "e.preventDefault(); "
                        "isDragging = true; "
                        "moveSelector(e); "
                        "}"
                        "function drag(e) {"
                        "if (isDragging) {"
                        "e.preventDefault(); "
                        "moveSelector(e); "
                        "}"
                        "}"
                        "function endDrag() {"
                        "isDragging = false; "
                        "}"
                        "function moveSelector(e) {"
                        "let clientX, clientY;"
                        "if (e.type.includes(\"mouse\")) {"
                        "clientX = e.clientX;"
                        "clientY = e.clientY;"
                        "} else {"
                        "clientX = e.touches[0].clientX;"
                        "clientY = e.touches[0].clientY;"
                        "}"
                        "const rect = canvas.getBoundingClientRect();"
                        "const x = clientX - rect.left;"
                        "const y = clientY - rect.top;"
                        "const dx = x - centerX;"
                        "const dy = y - centerY;"
                        "const distance = Math.sqrt(dx * dx + dy * dy);"
                        "let constrainedX = x;"
                        "let constrainedY = y;"
                        "const selectorRadius = 10;"
                        "if (distance > radius - selectorRadius) {"
                        "const ratio = (radius - selectorRadius) / distance;"
                        "constrainedX = centerX + dx * ratio;"
                        "constrainedY = centerY + dy * ratio;"
                        "}"
                        "colorSelector.style.left = `${constrainedX}px`;"
                        "colorSelector.style.top = `${constrainedY}px`;"
                        "updateCurrentColor(constrainedX, constrainedY);"
                        "}"
                        "function updateCurrentColor(x, y) {"
                        "const imageData = ctx.getImageData(x, y, 1, 1);"
                        "const pixel = imageData.data;"
                        "let r = pixel[0];"
                        "let g = pixel[1];"
                        "let b = pixel[2];"
                        "const threshold = 240; "
                        "if (r > threshold && g > threshold && b > threshold) {"
                        "const dx = x - centerX;"
                        "const dy = y - centerY;"
                        "const distance = Math.sqrt(dx * dx + dy * dy);"
                        "const innerRadius = radius * 0.8;"
                        "if (distance < innerRadius) {"
                        "const ratio = distance / innerRadius;"
                        "if (ratio < 0.3) {"
                        "r = 255;"
                        "g = 255;"
                        "b = 255;"
                        "} else {"
                        "r = Math.min(255, Math.max(0, r));"
                        "g = Math.min(255, Math.max(0, g));"
                        "b = Math.min(255, Math.max(0, b));"
                        "}"
                        "}"
                        "}"
                        "currentColor.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;"
                        "currentColor.textContent = `RGB(${r}, ${g}, ${b})`;"
                        "return { r, g, b };"
                        "}"
                        "function findClosestColorPosition(targetR, targetG, targetB) {"
                        "const searchRadius = radius;"
                        "let bestMatch = null;"
                        "let minDifference = Infinity;"
                        "const step = 10;"
                        "for (let x = centerX - searchRadius; x <= centerX + searchRadius; x += step) {"
                        "for (let y = centerY - searchRadius; y <= centerY + searchRadius; y += step) {"
                        "const dx = x - centerX;"
                        "const dy = y - centerY;"
                        "const distance = Math.sqrt(dx * dx + dy * dy);"
                        "if (distance <= searchRadius) {"
                        "const imageData = ctx.getImageData(x, y, 1, 1);"
                        "const pixel = imageData.data;"
                        "const r = pixel[0];"
                        "const g = pixel[1];"
                        "const b = pixel[2];"
                        "const diff = Math.sqrt("
                        "Math.pow(r - targetR, 2) +"
                        "Math.pow(g - targetG, 2) +"
                        "Math.pow(b - targetB, 2)"
                        ");"
                        "if (diff < minDifference) {"
                        "minDifference = diff;"
                        "bestMatch = { x, y, r, g, b, diff };"
                        "}"
                        "}"
                        "}"
                        "}"
                        "return bestMatch;"
                        "}"
                        "function setSelectorPositionByColor(r, g, b) {"
                        "const closest = findClosestColorPosition(r, g, b);"
                        "if (closest) {"
                        "x = closest.x;"
                        "y = closest.y;"
                        "console.log("
                        "`找到更接近的颜色位置: x=${x.toFixed(2)}, y=${y.toFixed("
                        "2"
                        ")}, 差异=${closest.diff.toFixed(2)}`"
                        ");"
                        "}"
                        "colorSelector.style.left = `${x}px`;"
                        "colorSelector.style.top = `${y}px`;"
                        "updateCurrentColor(x, y);"
                        "}"
                        "async function setColor() {"
                        "dialogMessage.textContent = \"改变颜色中...\";"
                        "dialog.showModal(); "
                        "const x = parseFloat(colorSelector.style.left);"
                        "const y = parseFloat(colorSelector.style.top);"
                        "const { r, g, b } = updateCurrentColor(x, y);"
                        "try {"
                        "const response = await fetch("
                        "`http://${deviceAddress}/set-color?r=${r}&g=${g}&b=${b}`,"
                        "{"
                        "method: \"POST\","
                        "}"
                        ");"
                        "if (response.ok) {"
                        "dialogMessage.textContent = `设置成功`;"
                        "} else {"
                        "throw new Error(\"设置颜色失败\");"
                        "}"
                        "} catch (error) {"
                        "dialog.close(); "
                        "dialogMessage.textContent = error.message;"
                        "dialog.showModal(); "
                        "} finally {"
                        "setTimeout(() => {"
                        "dialog.close(); "
                        "}, 1000);"
                        "}"
                        "}"
                        "async function getColor() {"
                        "dialogMessage.textContent = \"获取颜色中...\";"
                        "dialog.showModal(); "
                        "try {"
                        "const response = await fetch(`http://${deviceAddress}/current-color`, {});"
                        "if (response.ok) {"
                        "dialogMessage.textContent = `获取成功`;"
                        "const text = await response.text();"
                        "const colorValues = text.split(\",\");"
                        "const r = parseInt(colorValues[0]);"
                        "const g = parseInt(colorValues[1]);"
                        "const b = parseInt(colorValues[2]);"
                        "updateCurrentColor(r, g, b);"
                        "setSelectorPositionByColor(r, g, b);"
                        "} else {"
                        "throw new Error(\"获取颜色失败\");"
                        "}"
                        "} catch (error) {"
                        "dialog.close(); "
                        "dialogMessage.textContent = error.message;"
                        "dialog.showModal(); "
                        "setTimeout(() => {"
                        "dialog.close(); "
                        "}, 1000);"
                        "} finally {"
                        "dialog.close(); "
                        "}"
                        "}"
                        "window.addEventListener(\"load\", init);"
                        "</script>"
                        "</body>"
                        "</html>";

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
        SERVER.on("/color", [this]()
                  { m_handleColor(); });
        SERVER.on("/set-color", [this]()
                  { m_handleColorSet(); });
        SERVER.on("/current-color", [this]()
                  { m_handleColorGet(); });
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
    SERVER.send(200, "text/html; charset=utf-8", pageWifiConfig);
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
        SERVER.send(400, "text/html; charset=utf-8", pageConfigFail);
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
        SERVER.send(400, "text/html; charset=utf-8", pageConfigFail);
        return;
    }

    /* 连接成功 */
    SERVER.send(200, "text/html; charset=utf-8", pageConfigSuccess);
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
    SERVER.send(200, "text/html; charset=utf-8", pageRoot);
}

void CWeb::m_handleOta(void)
{
    SERVER.send(200, "text/html; charset=utf-8", pageOTA);
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
        SERVER.send(200, "text/html; charset=utf-8", pageOTASuccess);
        delay(4000);
        RGB.setAllPixelColor(0, 0, 0);
        delay(1000);
        ESP.restart();
    }
    else
    {
        SERVER.send(400, "text/html; charset=utf-8", pageOTAFail);
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

void CWeb::m_handleColor(void)
{
    SERVER.send(200, "text/html; charset=utf-8", pageColor);
}

/*
    @brief  颜色设置处理函数，响应颜色设置请求，更新LED颜色
    @param  无
    @return 无
*/
void CWeb::m_handleColorSet(void) // 颜色设置处理函数，响应颜色设置请求，更新LED颜色
{
    // 从URL参数获取RGB值
    int r = SERVER.arg("r").toInt();
    int g = SERVER.arg("g").toInt();
    int b = SERVER.arg("b").toInt();

    // 设置LED颜色
    RGB.setAllPixelColor(r, g, b);

    // 保存当前颜色到NVS
    NVS.saveColor(r, g, b);

    // 返回成功响应
    SERVER.send(200, "text/plain; charset=utf-8", "OK: Color updated");
}

/*
    @brief  颜色获取处理函数，响应颜色获取请求，返回当前LED颜色
    @param  无
    @return 无
*/
void CWeb::m_handleColorGet(void) // 颜色获取处理函数，响应颜色获取请求，返回当前LED颜色
{
    // 从NVS获取当前颜色
    uint8_t r, g, b;
    NVS.loadColor(r, g, b);

    // 返回当前颜色值，格式为 "r,g,b"
    String response = String(r) + "," + String(g) + "," + String(b);
    SERVER.send(200, "text/plain; charset=utf-8", response);
}
