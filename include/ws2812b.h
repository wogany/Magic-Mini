#ifndef _WS2812B_H
#define _WS2812B_H

#include <stdint.h>
#include <vector>
#include <Adafruit_NeoPixel.h>

/* RGB灯板布线类型 */
enum RgbConfig
{
    Snakelike = 1, // 蛇形布线
    Parallel = 2   // 并行布线
};

class CWs2812b
{
private:
    Adafruit_NeoPixel PIXEL; // 创建Adafruit_NeoPixel对象

    uint8_t m_Pin;             // 数据引脚
    uint16_t m_Num;            // 灯珠数量
    uint8_t m_Row;             // 灯板布局行数
    uint8_t m_Col;             // 灯板布局列数
    RgbConfig m_Config;        // 灯板布线类型
    uint8_t m_Brightness; // 灯珠亮度

    std::vector<uint16_t> m_MapTable; // 灯珠映射表

public:
    CWs2812b(uint8_t pin, uint16_t num);
    ~CWs2812b(void);

    bool init(uint8_t row, uint8_t col, RgbConfig config);
    void begin(void);
    void test(void);
    void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b);
    void setAllPixelBrightness(uint8_t brightness);

    uint8_t colorRed;   // 红色分量
    uint8_t colorGreen; // 绿色分量
    uint8_t colorBlue;  // 蓝色分量
};

#endif