#include "nvs.h"

#define NAME "my_config"

/*
    @brief  ���캯��
    @param  ��
    @return ��
*/
CNvs::CNvs(void)
{
}

/*
    @brief  ��������
    @param  ��
    @return ��
*/
CNvs::~CNvs(void)
{
}

/*
    @brief  NVS��ʼ��
    @param  ��
    @return �ɹ����
*/
bool CNvs::init(void)
{
    PREFS.begin(NAME, false);
    PREFS.end();
    return true;
}

/*
    @brief  ������ɫ��NVS
    @param  r ��ɫ����
    @param  g ��ɫ����
    @param  b ��ɫ����
    @return ��
*/
void CNvs::saveColor(uint8_t r, uint8_t g, uint8_t b)
{
    PREFS.begin(NAME, false);
    uint8_t lastR, lastG, lastB;
    /*  ��ȡ�ϴα������ɫ */
    lastR = PREFS.getUChar("colorR", 0);
    lastG = PREFS.getUChar("colorG", 0);
    lastB = PREFS.getUChar("colorB", 0);
    /* �����ɫ�Ƿ�仯 */
    if (lastR != r)
    {
        PREFS.putUChar("colorR", r);
    }
    if (lastG != g)
    {
        PREFS.putUChar("colorG", g);
    }
    if (lastB != b)
    {
        PREFS.putUChar("colorB", b);
    }
    PREFS.end();
}

/*
    @brief  ��NVS������ɫ
    @param  r ��ɫ��������
    @param  g ��ɫ��������
    @param  b ��ɫ��������
    @return ��
*/
void CNvs::loadColor(uint8_t &r, uint8_t &g, uint8_t &b)
{
    PREFS.begin(NAME, true);
    r = PREFS.getUChar("colorR", 255);
    g = PREFS.getUChar("colorG", 255);
    b = PREFS.getUChar("colorB", 255);
    PREFS.end();
}