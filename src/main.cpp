#include <Arduino.h>
#include <ws2812b.h>

#define RGB_NUM 40 // ��������
#define RGB_ROW 5  // �ư�����
#define RGB_COL 8  // �ư�����

#define RGB_PIN 10           // �ź���������
#define BUTTON_LEFT_PIN 18   // �󰴼�����
#define BUTTON_MIDDLE_PIN 19 // �м䰴������
#define BUTTON_RIGHT_PIN 3   // �Ұ�������

#define BUTTON_DEBOUNCE_DELAY 10 // ����������ʱ
#define BUTTON_HOLD_DELAY 50     // ��������ʱ��

volatile unsigned long lastTime = 0;             // �ϴγ������ʱ��
volatile unsigned long leftButtonLastTime = 0;   // �󰴼��ϴδ���ʱ��
volatile unsigned long middleButtonLastTime = 0; // �м䰴���ϴδ���ʱ��
volatile unsigned long rightButtonLastTime = 0;  // �Ұ����ϴδ���ʱ��

volatile bool isButtonPressed = false;     // �а������±�־
volatile bool leftButtonPressed = false;   // �󰴼����±�־
volatile bool middleButtonPressed = false; // �м䰴�����±�־
volatile bool rightButtonPressed = false;  // �Ұ������±�־

/* ����һ���ư���� */
CWs2812b RGB = CWs2812b(RGB_PIN, RGB_NUM);

void buttonSetup(void);
void buttonLoop(void);
void IRAM_ATTR leftButtonISR(void);
void IRAM_ATTR middleButtonISR(void);
void IRAM_ATTR rightButtonISR(void);

void setup()
{
    buttonSetup();
    RGB.init(RGB_ROW, RGB_COL, Snakelike);
    delay(1000);
    RGB.test();
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
    @brief  �������ų�ʼ��,�������룬�½��ش����ж�
    @param  ��
    @return ��
*/
void buttonSetup(void)
{
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_MIDDLE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

    attachInterrupt(BUTTON_LEFT_PIN, leftButtonISR, FALLING);
    attachInterrupt(BUTTON_MIDDLE_PIN, middleButtonISR, FALLING);
    attachInterrupt(BUTTON_RIGHT_PIN, rightButtonISR, FALLING);
}

/*
    @brief  ����������
    @param  ��
    @return ��
*/
void buttonLoop(void)
{
    uint8_t buttonPin;
    bool buttonState = false;

    /* ��ȡ�������� */
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

    /* ��ȡ����״̬ */
    buttonState = digitalRead(buttonPin);

    /* ��ⰴ������ */
    while (buttonState == false)
    {
        unsigned long now = millis();

        /* ��ⰴ����������100ms */
        if (now - lastTime > BUTTON_HOLD_DELAY)
        {
            lastTime = now;
            if (leftButtonPressed)
            {
                RGB.colorRed++;
                if (RGB.colorRed > 255)
                {
                    RGB.colorRed = 0;
                }
            }
            else if (middleButtonPressed)
            {
                RGB.colorGreen++;
                if (RGB.colorGreen > 255)
                {
                    RGB.colorGreen = 0;
                }
            }
            else if (rightButtonPressed)
            {
                RGB.colorBlue++;
                if (RGB.colorBlue > 255)
                {
                    RGB.colorBlue = 0;
                }
            }
            RGB.setAllPixelColor(RGB.colorRed, RGB.colorGreen, RGB.colorBlue);
        }

        buttonState = digitalRead(buttonPin);
    }
}

/*
    @brief  �󰴼��жϷ�����
    @param  ��
    @return ��
*/
void IRAM_ATTR leftButtonISR(void)
{
    unsigned long now = millis();
    if (now - leftButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        leftButtonLastTime = now;

        if (!isButtonPressed)
        {
            isButtonPressed = true;
            leftButtonPressed = true;
        }
    }
}

/*
    @brief  �м䰴���жϷ�����
    @param  ��
    @return ��
*/
void IRAM_ATTR middleButtonISR(void)
{
    unsigned long now = millis();
    if (now - middleButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        middleButtonLastTime = now;

        if (!isButtonPressed)
        {
            isButtonPressed = true;
            middleButtonPressed = true;
        }
    }
}

/*
    @brief  �Ұ����жϷ�����
    @param  ��
    @return ��
*/
void IRAM_ATTR rightButtonISR(void)
{
    unsigned long now = millis();
    if (now - rightButtonLastTime > BUTTON_DEBOUNCE_DELAY)
    {
        rightButtonLastTime = now;

        if (!isButtonPressed)
        {
            isButtonPressed = true;
            rightButtonPressed = true;
        }
    }
}
