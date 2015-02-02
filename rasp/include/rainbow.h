#ifndef __RAINBOW_H__
#define __RAINBOW_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class rainbow: public animation
{
public:
    rainbow(ledMatrix* leds, int sens = VERTICAL);
    void start();
private:
    int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);
    unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);
    int rainbowColors[180];
    int m_sens;
    ledMatrix* m_leds;
};

#endif
