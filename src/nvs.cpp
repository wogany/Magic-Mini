#include "nvs.h"

#define NAME "my_config"

/*
    @brief  构造函数
    @param  无
    @return 无
*/
CNvs::CNvs(void)
{
}

/*
    @brief  析构函数
    @param  无
    @return 无
*/
CNvs::~CNvs(void)
{
}

/*
    @brief  NVS初始化
    @param  无
    @return 成功与否
*/
bool CNvs::init(void)
{
    PREFS.begin(NAME, false);
    PREFS.end();
    return true;
}

/*
    @brief  保存颜色到NVS
    @param  r 红色分量
    @param  g 绿色分量
    @param  b 蓝色分量
    @return 无
*/
void CNvs::saveColor(uint8_t r, uint8_t g, uint8_t b)
{
    PREFS.begin(NAME, false);
    uint8_t lastR, lastG, lastB;
    /*  获取上次保存的颜色 */
    lastR = PREFS.getUChar("colorR", 0);
    lastG = PREFS.getUChar("colorG", 0);
    lastB = PREFS.getUChar("colorB", 0);
    /* 检测颜色是否变化 */
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
    @brief  从NVS加载颜色
    @param  r 红色分量引用
    @param  g 绿色分量引用
    @param  b 蓝色分量引用
    @return 无
*/
void CNvs::loadColor(uint8_t &r, uint8_t &g, uint8_t &b)
{
    PREFS.begin(NAME, true);
    r = PREFS.getUChar("colorR", 255);
    g = PREFS.getUChar("colorG", 255);
    b = PREFS.getUChar("colorB", 255);
    PREFS.end();
}