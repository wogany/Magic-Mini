#include <ws2812b.h>
#include <Adafruit_NeoPixel.h>

#define RGB_NUM 40 // 灯珠数量
#define RGB_PIN 7  // 灯珠信号输入引脚

Adafruit_NeoPixel PIXEL = Adafruit_NeoPixel(RGB_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

/*
    @brief  初始化灯板
    @param  无
    @return 无
*/
void CWs2812b::Init(void)
{
    PIXEL.begin();
    PIXEL.clear();
    PIXEL.show();
    PIXEL.setBrightness(50);
}

/*
    @brief  测试灯板
    @param  无
    @return 无
*/
void CWs2812b::Test(void)
{
    PIXEL.fill(PIXEL.Color(255, 0, 0), 0, 40); // 红色
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(0, 255, 0), 0, 40); // 绿色
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(0, 0, 255), 0, 40); // 蓝色
    PIXEL.show();
    delay(1000);
    PIXEL.fill(PIXEL.Color(255, 255, 255), 0, 40); // 白色
    PIXEL.show();
}
