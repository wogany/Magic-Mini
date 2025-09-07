#include <Arduino.h>
#include "ws2812b.h"
#include "nvs.h"

#define RGB_NUM 40 // 灯珠数量
#define RGB_ROW 5  // 灯板行数
#define RGB_COL 8  // 灯板列数

#define RGB_PIN 10           // 信号输入引脚
#define BUTTON_LEFT_PIN 18   // 左按键引脚
#define BUTTON_MIDDLE_PIN 19 // 中按键引脚
#define BUTTON_RIGHT_PIN 3   // 右按键引脚

#define BUTTON_DEBOUNCE_DELAY 10 // 按键消抖延时
#define BUTTON_HOLD_DELAY 50     // 按键长按任务单元时间

unsigned long lastTime = 0;                      // 按键长按任务上次触发时间
volatile unsigned long leftButtonLastTime = 0;   // 左按键上次触发时间
volatile unsigned long middleButtonLastTime = 0; // 中按键上次触发时间
volatile unsigned long rightButtonLastTime = 0;  // 右按键上次触发时间

volatile bool isButtonPressed = false;     // 有按键按下标志
volatile bool leftButtonPressed = false;   // 左按键按下标志
volatile bool middleButtonPressed = false; // 中按键按下标志
volatile bool rightButtonPressed = false;  // 右按键按下标志
bool isMiddleButtonHold = false;           // 中按键长按任务标志

uint8_t *colorChoose = nullptr; // 颜色选择指针
uint8_t middleButtonCount = 0;  // 中按键计数

CWs2812b RGB = CWs2812b(RGB_PIN, RGB_NUM); // 灯板对象
CNvs NVS;                                  // NVS存储对象

void buttonSetup(void);
void buttonLoop(void);
void leftButtonHandle(void);
void middleButtonHandle(void);
void rightButtonHandle(void);
void IRAM_ATTR leftButtonISR(void);
void IRAM_ATTR middleButtonISR(void);
void IRAM_ATTR rightButtonISR(void);

void setup()
{
    buttonSetup();
    NVS.init();
    delay(100);
    RGB.init(RGB_ROW, RGB_COL, Snakelike);
    delay(100);
    RGB.begin();
    delay(100);
}

void loop()
{
    if (isButtonPressed)
    {
        buttonLoop();
        isButtonPressed = false;
        leftButtonPressed = false;
        middleButtonPressed = false;
        rightButtonPressed = false;
    }
}

/*
    @brief  按键引脚初始化,上拉输入，下降沿触发中断
    @param  无
    @return 无
*/
void buttonSetup(void)
{
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_MIDDLE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

    attachInterrupt(BUTTON_LEFT_PIN, leftButtonISR, FALLING);
    attachInterrupt(BUTTON_MIDDLE_PIN, middleButtonISR, FALLING);
    attachInterrupt(BUTTON_RIGHT_PIN, rightButtonISR, FALLING);

    colorChoose = &RGB.colorRed; // 默认选择红色分量
}

/*
    @brief  按键处理函数
    @param  无
    @return 无
*/
void buttonLoop(void)
{
    /* 判断被按下的按键 */
    uint8_t buttonPin;
    if (leftButtonPressed)
    {
        buttonPin = BUTTON_LEFT_PIN;
    }
    else if (middleButtonPressed)
    {
        buttonPin = BUTTON_MIDDLE_PIN;
    }
    else if (rightButtonPressed)
    {
        buttonPin = BUTTON_RIGHT_PIN;
    }

    /* 判断被按下按键的状态 */
    bool buttonState = false;
    buttonState = digitalRead(buttonPin);

    /* 当按键长按时执行 */
    while (buttonState == false)
    {
        unsigned long now = millis();

        /* 每经过一次长按任务单元时间执行*/
        if (now - lastTime > BUTTON_HOLD_DELAY)
        {
            lastTime = now;
            if (leftButtonPressed)
            {
                leftButtonHandle();
            }
            else if (middleButtonPressed)
            {
                middleButtonHandle();
                if (isMiddleButtonHold == false)
                {
                    break; // 退出长按处理循环
                }
            }
            else if (rightButtonPressed)
            {
                rightButtonHandle();
            }
            RGB.setAllPixelColor(RGB.colorRed, RGB.colorGreen, RGB.colorBlue);
        }

        buttonState = digitalRead(buttonPin); // 读取按键状态
    }

    /* 按键释放后执行 */
    NVS.saveColor(RGB.colorRed, RGB.colorGreen, RGB.colorBlue);
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
        // 进入配网模式
        middleButtonCount = 0;
        isMiddleButtonHold = false;
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
    unsigned long now = millis();
    if (now - middleButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        middleButtonLastTime = now;

        if (!isButtonPressed && digitalRead(BUTTON_MIDDLE_PIN) == LOW)
        {
            isButtonPressed = true;
            middleButtonPressed = true;
            isMiddleButtonHold = true;
        }

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
