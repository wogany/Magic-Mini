#include "ws2812b.h"

/*
    @brief  构造函数，初始化数据引脚和灯珠数量
    @param  pin  数据引脚
    @param  num  灯珠数量
    @return 无
*/
CWs2812b::CWs2812b(uint8_t pin, uint16_t num) : m_Pin(pin), m_Num(num), PIXEL(num, pin, NEO_GRB + NEO_KHZ800)
{
    PIXEL.setBrightness(m_Brightness); // 设置默认亮度
    colorRed = 255;                    // 默认红色分量
    colorGreen = 255;                  // 默认绿色分量
    colorBlue = 255;                   // 默认蓝色分量
}

/*
    @brief  析构函数
    @param  无
    @return 无
*/
CWs2812b::~CWs2812b()
{
}

/*
    @brief  初始化灯板
    @param  row  灯板行数
    @param  col  灯板列数
    @param  config  灯板布线类型
    @return 成功与否
*/
bool CWs2812b::init(uint8_t row, uint8_t col, RgbConfig config)
{
    /* 检查行列数与灯珠数量是否匹配 */
    if (row * col != m_Num)
    {
        return false;
    }

    /* 保存灯板布局 */
    m_Row = row;
    m_Col = col;
    m_Config = config;

    /* 生成匹配灯珠数量的映射表 */
    m_MapTable.resize(m_Num);

    switch (m_Config)
    {
    case Snakelike: // 蛇形布线
        for (uint8_t r = 0; r < row; r++)
        {
            for (uint8_t c = 0; c < col; c++)
            {
                uint8_t index = r * col + c; // 当前灯珠索引
                if (r % 2 == 0)
                {
                    m_MapTable[index] = index; // 灯板奇数行，顺序
                }
                else
                {
                    m_MapTable[index] = (r + 1) * col - c - 1; // 灯板偶数行，反序
                }
            }
        }
        break;
    case Parallel: // 并行布线
        for (uint16_t i = 0; i < m_Num; i++)
        {
            m_MapTable[i] = i; // 直接映射
        }
        break;
    default:
        return false; // 不支持的布线类型
    }

    return true;
}

/*
    @brief  测试函数，依次点亮每个灯珠
    @param  无
    @return 无
*/
void CWs2812b::test()
{
    for (uint16_t i = 0; i < m_Num; i++)
    {
        PIXEL.setPixelColor(m_MapTable[i], PIXEL.Color(255, 255, 255)); // 点亮白色
        PIXEL.show();
        delay(50);
    }
}

/*
    @brief  设置所有灯珠颜色
    @param  r  红色分量
    @param  g  绿色分量
    @param  b  蓝色分量
    @return 成功与否
*/
void CWs2812b::setAllPixelColor(uint8_t r, uint8_t g, uint8_t b)
{
    PIXEL.fill(PIXEL.Color(r, g, b)); // 设置所有灯珠颜色
    PIXEL.show();
}