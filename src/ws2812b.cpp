#include "ws2812b.h"

/*
    @brief  ���캯������ʼ���������ź͵�������
    @param  pin  ��������
    @param  num  ��������
    @return ��
*/
CWs2812b::CWs2812b(uint8_t pin, uint16_t num) : m_Pin(pin), m_Num(num), PIXEL(num, pin, NEO_GRB + NEO_KHZ800)
{
    PIXEL.setBrightness(m_Brightness); // ����Ĭ������
    colorRed = 255;                    // Ĭ�Ϻ�ɫ����
    colorGreen = 255;                  // Ĭ����ɫ����
    colorBlue = 255;                   // Ĭ����ɫ����
}

/*
    @brief  ��������
    @param  ��
    @return ��
*/
CWs2812b::~CWs2812b()
{
}

/*
    @brief  ��ʼ���ư�
    @param  row  �ư�����
    @param  col  �ư�����
    @param  config  �ư岼������
    @return �ɹ����
*/
bool CWs2812b::init(uint8_t row, uint8_t col, RgbConfig config)
{
    /* �������������������Ƿ�ƥ�� */
    if (row * col != m_Num)
    {
        return false;
    }

    /* ����ư岼�� */
    m_Row = row;
    m_Col = col;
    m_Config = config;

    /* ����ƥ�����������ӳ��� */
    m_MapTable.resize(m_Num);

    switch (m_Config)
    {
    case Snakelike: // ���β���
        for (uint8_t r = 0; r < row; r++)
        {
            for (uint8_t c = 0; c < col; c++)
            {
                uint8_t index = r * col + c; // ��ǰ��������
                if (r % 2 == 0)
                {
                    m_MapTable[index] = index; // �ư������У�˳��
                }
                else
                {
                    m_MapTable[index] = (r + 1) * col - c - 1; // �ư�ż���У�����
                }
            }
        }
        break;
    case Parallel: // ���в���
        for (uint16_t i = 0; i < m_Num; i++)
        {
            m_MapTable[i] = i; // ֱ��ӳ��
        }
        break;
    default:
        return false; // ��֧�ֵĲ�������
    }

    return true;
}

/*
    @brief  ���Ժ��������ε���ÿ������
    @param  ��
    @return ��
*/
void CWs2812b::test()
{
    for (uint16_t i = 0; i < m_Num; i++)
    {
        PIXEL.setPixelColor(m_MapTable[i], PIXEL.Color(255, 255, 255)); // ������ɫ
        PIXEL.show();
        delay(50);
    }
}

/*
    @brief  �������е�����ɫ
    @param  r  ��ɫ����
    @param  g  ��ɫ����
    @param  b  ��ɫ����
    @return �ɹ����
*/
void CWs2812b::setAllPixelColor(uint8_t r, uint8_t g, uint8_t b)
{
    PIXEL.fill(PIXEL.Color(r, g, b)); // �������е�����ɫ
    PIXEL.show();
}