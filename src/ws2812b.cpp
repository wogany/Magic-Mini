#include <ws2812b.h>
#include <Adafruit_NeoPixel.h>

#define RGB_NUM 40 // ��������
#define RGB_PIN 7  // �����ź���������

Adafruit_NeoPixel PIXEL = Adafruit_NeoPixel(RGB_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

/*
    @brief  ��ʼ���ư�
    @param  ��
    @return ��
*/
void CWs2812b::Init(void)
{
    PIXEL.begin();
    PIXEL.clear();
    PIXEL.show();
    PIXEL.setBrightness(50);
}

/*
    @brief  ���Եư�
    @param  ��
    @return ��
*/
void CWs2812b::Test(void)
{
    PIXEL.fill(PIXEL.Color(255, 0, 0), 0, 40); // ��ɫ
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(0, 255, 0), 0, 40); // ��ɫ
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(0, 0, 255), 0, 40); // ��ɫ
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(255, 255, 255), 0, 40); // ��ɫ
    PIXEL.show();
}
