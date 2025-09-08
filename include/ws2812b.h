#ifndef _WS2812B_H
#define _WS2812B_H

#include <stdint.h>
#include <vector>
#include <Adafruit_NeoPixel.h>

/* RGB�ư岼������ */
enum RgbConfig
{
    Snakelike = 1, // ���β���
    Parallel = 2   // ���в���
};

class CWs2812b
{
private:
    Adafruit_NeoPixel PIXEL; // ����Adafruit_NeoPixel����

    uint8_t m_Pin;             // ��������
    uint16_t m_Num;            // ��������
    uint8_t m_Row;             // �ư岼������
    uint8_t m_Col;             // �ư岼������
    RgbConfig m_Config;        // �ư岼������
    uint8_t m_Brightness; // ��������

    std::vector<uint16_t> m_MapTable; // ����ӳ���

public:
    CWs2812b(uint8_t pin, uint16_t num);
    ~CWs2812b(void);

    bool init(uint8_t row, uint8_t col, RgbConfig config);
    void begin(void);
    void test(void);
    void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b);
    void setAllPixelBrightness(uint8_t brightness);

    uint8_t colorRed;   // ��ɫ����
    uint8_t colorGreen; // ��ɫ����
    uint8_t colorBlue;  // ��ɫ����
};

#endif