#include <Arduino.h>
#include <ws2812b.h>

CWs2812b RGB;

void setup()
{
    RGB.Init();
    delay(1000);
    RGB.Test();
}

void loop()
{

}