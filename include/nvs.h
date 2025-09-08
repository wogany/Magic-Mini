#ifndef _NVS_H
#define _NVS_H

#include <Preferences.h>

class CNvs
{
    private:
    Preferences PREFS;

    public:
    CNvs(void);
    ~CNvs(void);

    bool init(void);
    void saveColor(uint8_t r, uint8_t g, uint8_t b);
    void loadColor(uint8_t &r, uint8_t &g, uint8_t &b);
    void saveWifiState(bool state);
    void getWifiState(bool &state);
    void saveWifiInfo(const char* ssid, const char* password, bool hasPassword);
    void loadWifiInfo(char* ssid, char* password, bool &hasPassword);

};

#endif