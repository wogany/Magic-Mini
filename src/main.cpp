#include <Arduino.h>
#include "ws2812b.h"
#include "nvs.h"
#include "web.h"

#define RGB_NUM 40            // 灯珠数量
#define RGB_ROW 5             // 灯板行数
#define RGB_COL 8             // 灯板列数
#define RGB_MAX_BRIGHTNESS 70 // 最大亮度

#define RGB_PIN 10           // 信号输入引脚
#define BUTTON_LEFT_PIN 18   // 左按键引脚
#define BUTTON_MIDDLE_PIN 19 // 中按键引脚
#define BUTTON_RIGHT_PIN 3   // 右按键引脚

#define BUTTON_DEBOUNCE_DELAY 10 // 按键消抖延时
#define BUTTON_HOLD_DELAY 20     // 按键长按任务单元时间

unsigned long lastTime = 0;                      // 按键长按任务上次触发时间
volatile unsigned long leftButtonLastTime = 0;   // 左按键上次触发时间
volatile unsigned long middleButtonLastTime = 0; // 中按键上次触发时间
volatile unsigned long rightButtonLastTime = 0;  // 右按键上次触发时间

volatile bool isButtonPressed = false;        // 有按键按下标志
volatile bool leftButtonPressed = false;      // 左按键按下标志
volatile bool middleButtonPressed = false;    // 中按键按下标志
volatile bool rightButtonPressed = false;     // 右按键按下标志
volatile bool middleButtonShortPress = false; // 中按键短按任务标志
volatile bool middleButtonRequest = false;    // 中按键短按任务结束标志
bool isMiddleButtonHold = false;              // 中按键长按任务标志

uint8_t *colorChoose = nullptr; // 颜色选择指针
uint8_t middleButtonCount = 0;  // 中按键计数

CWs2812b RGB = CWs2812b(RGB_PIN, RGB_NUM); // 灯板对象
CNvs NVS;                                  // NVS存储对象
CWeb WEB;                                  // Web服务器对象

void buttonSetup(void);
void buttonLoop(void);
void leftButtonHandle(void);
void middleButtonHandle(void);
void rightButtonHandle(void);
void IRAM_ATTR leftButtonISR(void);
void IRAM_ATTR middleButtonISR(void);
void IRAM_ATTR rightButtonISR(void);
void IRAM_ATTR timerISR(void);

void setup()
{
    Serial.begin(115200);
    buttonSetup();
    NVS.init();
    delay(100);
    RGB.init(RGB_ROW, RGB_COL, Snakelike);
    delay(100);
    RGB.begin();
    delay(100);
    WEB.init();
    delay(100);
    WEB.begin();
    delay(100);
}

void loop()
{
    WEB.loop();

    if (isButtonPressed)
    {
        buttonLoop();
        isButtonPressed = false;
        leftButtonPressed = false;
        middleButtonPressed = false;
        rightButtonPressed = false;
    }

    if (middleButtonShortPress)
    {
        RGB.setAllPixelBrightness(RGB_MAX_BRIGHTNESS / 2);

        if (colorChoose == &RGB.colorRed)
        {
            colorChoose = &RGB.colorGreen; // 选择绿色分量
        }
        else if (colorChoose == &RGB.colorGreen)
        {
            colorChoose = &RGB.colorBlue; // 选择蓝色分量
        }
        else if (colorChoose == &RGB.colorBlue)
        {
            colorChoose = &RGB.colorRed; // 选择红色分量
        }

        delay(100);
        RGB.setAllPixelBrightness(RGB_MAX_BRIGHTNESS);

        middleButtonShortPress = false;
    }
}

/*
    @brief  按键引脚初始化,上拉输入，下降沿触发中断
    @param  无
    @return 无
*/
void buttonSetup(void)
{
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);   // 设置左按键引脚为输入上拉模式
    pinMode(BUTTON_MIDDLE_PIN, INPUT_PULLUP); // 设置中按键引脚为输入上拉模式
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);  // 设置右按键引脚为输入上拉模式

    attachInterrupt(BUTTON_LEFT_PIN, leftButtonISR, FALLING);     // 为左按键引脚配置中断
    attachInterrupt(BUTTON_MIDDLE_PIN, middleButtonISR, FALLING); // 为中按键引脚配置中断
    attachInterrupt(BUTTON_RIGHT_PIN, rightButtonISR, FALLING);   // 为右按键引脚配置中断

    colorChoose = &RGB.colorRed;
}

/*
    @brief  按键处理函数
    @param  无
    @return 无
*/
void buttonLoop(void)
{
    /* 判断被按下的按键 */
    uint8_t buttonPin;     // 存储被按下按键的引脚号
    if (leftButtonPressed) // 检测左按键是否被按下
    {
        buttonPin = BUTTON_LEFT_PIN; // 设置为左按键引脚
    }
    else if (middleButtonPressed) // 检测中按键是否被按下
    {
        buttonPin = BUTTON_MIDDLE_PIN; // 设置为中按键引脚
    }
    else if (rightButtonPressed) // 检测右按键是否被按下
    {
        buttonPin = BUTTON_RIGHT_PIN; // 设置为右按键引脚
    }

    /* 判断被按下按键的状态 */
    bool buttonState = false;             // 存储按键状态，false表示按下，true表示释放
    buttonState = digitalRead(buttonPin); // 读取按键引脚的当前状态

    /* 当按键长按时执行 */
    while (buttonState == false) // 当按键保持按下状态时循环执行
    {
        unsigned long now = millis(); // 获取当前系统运行时间（毫秒）

        /* 每经过一次长按任务单元时间执行*/
        if (now - lastTime > BUTTON_HOLD_DELAY) // 判断是否达到长按任务执行间隔
        {
            lastTime = now;        // 更新上次执行时间
            if (leftButtonPressed) // 如果是左按键长按
            {
                leftButtonHandle(); // 调用左按键处理函数，减少当前颜色分量值
            }
            else if (middleButtonPressed) // 如果是中按键长按
            {
                middleButtonHandle();            // 调用中按键处理函数，处理长按逻辑
                if (isMiddleButtonHold == false) // 检查是否需要退出长按处理
                {
                    break; // 退出长按处理循环
                }
            }
            else if (rightButtonPressed) // 如果是右按键长按
            {
                rightButtonHandle(); // 调用右按键处理函数，增加当前颜色分量值
            }
            RGB.setAllPixelColor(RGB.colorRed, RGB.colorGreen, RGB.colorBlue); // 更新所有灯珠的颜色显示
        }

        buttonState = digitalRead(buttonPin); // 重新读取按键状态，检查是否释放
    }

    /* 按键释放后执行 */
    NVS.saveColor(RGB.colorRed, RGB.colorGreen, RGB.colorBlue); // 将当前RGB颜色值保存到NVS非易失性存储器
}

/*
    @brief  左按键处理函数
    @param  无
    @return 无
*/
void leftButtonHandle(void)
{
    if (*colorChoose > 0)
    {
        (*colorChoose)--;
    }
}

/*
    @brief  中按键处理函数
    @param  无
    @return 无
*/
void middleButtonHandle(void)
{
    middleButtonCount++;
    if (middleButtonCount > (10000 / BUTTON_HOLD_DELAY))
    {
        middleButtonCount = 0;
        isMiddleButtonHold = false;

        /* 进入配网模式 */
        RGB.setAllPixelColor(0, 0, 0);
        NVS.saveWifiState(false);
        delay(1000);
        ESP.restart();
    }
}

/*
    @brief  右按键处理函数
    @param  无
    @return 无
*/
void rightButtonHandle(void)
{
    if (*colorChoose < 255)
    {
        (*colorChoose)++;
    }
}

/*
    @brief  左按键中断服务函数
    @param  无
    @return 无
*/
void IRAM_ATTR leftButtonISR(void)
{
    unsigned long now = millis();
    if (now - leftButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        leftButtonLastTime = now;

        if (!isButtonPressed && digitalRead(BUTTON_LEFT_PIN) == LOW)
        {
            isButtonPressed = true;
            leftButtonPressed = true;
        }
    }
}

/*
    @brief  中按键中断服务函数
    @param  无
    @return 无
*/
void IRAM_ATTR middleButtonISR(void)
{
    unsigned long now = millis();                           // 获取当前系统运行时间（毫秒）
    if (now - middleButtonLastTime > BUTTON_DEBOUNCE_DELAY) // 判断是否超过按键消抖延时
    {
        middleButtonLastTime = now; // 更新中按键上次触发时间

        // 检查是否有其他按键正在处理，并确认中按键确实被按下（低电平）
        if (!isButtonPressed && digitalRead(BUTTON_MIDDLE_PIN) == LOW)
        {
            isButtonPressed = true;     // 设置有按键按下标志，防止多按键同时处理
            middleButtonPressed = true; // 设置中按键按下标志
            isMiddleButtonHold = true;  // 设置中按键长按标志，准备处理长按事件
        }

        middleButtonShortPress = true; // 设置中按键短按任务标志，触发短按处理
    }
}

/*
    @brief  右按键中断服务函数
    @param  无
    @return 无
*/
void IRAM_ATTR rightButtonISR(void)
{
    unsigned long now = millis();
    if (now - rightButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        rightButtonLastTime = now;

        if (!isButtonPressed && digitalRead(BUTTON_RIGHT_PIN) == LOW)
        {
            isButtonPressed = true;
            rightButtonPressed = true;
        }
    }
}