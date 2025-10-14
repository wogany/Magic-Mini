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
    Adafruit_NeoPixel PIXEL; // Adafruit_NeoPixel对象，用于控制WS2812B LED灯带

    uint8_t m_Pin;        // 数据引脚，连接LED灯带的数据线
    uint16_t m_Num;       // 灯珠数量，LED灯带上灯珠的总数
    uint8_t m_Row;        // 灯板布局行数，LED灯板的行数
    uint8_t m_Col;        // 灯板布局列数，LED灯板的列数
    RgbConfig m_Config;   // 灯板布线类型，蛇形或并行布线
    uint8_t m_Brightness; // 灯珠亮度，控制LED灯的亮度值

    std::vector<uint16_t> m_MapTable; // 灯珠映射表，用于存储灯珠的物理位置与逻辑位置的映射关系

public:
    CWs2812b(uint8_t pin, uint16_t num); // 构造函数，初始化数据引脚和灯珠数量
    ~CWs2812b(void);                     // 析构函数，清理资源

    bool init(uint8_t row, uint8_t col, RgbConfig config);  // 初始化灯板布局，设置行列数和布线类型
    void begin(void);                                       // 启动灯板，以上一次保存的颜色点亮所有灯珠
    void test(void);                                        // 测试函数，依次点亮每个灯珠
    void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b); // 设置所有灯珠的颜色
    void setAllPixelBrightness(uint8_t brightness);         // 设置所有灯珠的亮度

    uint8_t colorRed;   // 红色分量，存储当前颜色的红色值
    uint8_t colorGreen; // 绿色分量，存储当前颜色的绿色值
    uint8_t colorBlue;  // 蓝色分量，存储当前颜色的蓝色值
};

#endif