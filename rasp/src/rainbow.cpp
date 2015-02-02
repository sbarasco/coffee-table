#include "rainbow.h"
#include <unistd.h>


rainbow::rainbow(ledMatrix* leds, int sens): m_leds(leds), m_sens(sens)
{
    int saturation = 100;
    int lightness = 50;
    for (int i=0; i<180; i++)
    {
        int hue = i * 2;
        rainbowColors[i] = makeColor(hue, saturation, lightness);
    }
}

void rainbow::start()
{
    while(1)
    {
        int color, x, y, index;
        while(1)
        {
            for (color=0; color < 180; color++)
            {
                for (x=0; x < LEDPERLINE; x++)
                {
                    if(m_sens == VERTICAL)
                    {
                        index = (color + x*10) % 180;
                    }
                    for (y=0; y < NBLINES; y++)
                    {
                        if(m_sens == HORIZONTAL)
                        {
                            index = (color + y*5) % 180;
                        }
                        m_leds->setPixel(x, y, rainbowColors[index]);
                    }
                }
                m_leds->update();
                usleep(60000);
            }
        }
    }
}

int rainbow::makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
    unsigned int red, green, blue;
    unsigned int var1, var2;

    if (hue > 359) hue = hue % 360;
    if (saturation > 100) saturation = 100;
    if (lightness > 100) lightness = 100;

    if (saturation == 0)
    {
        red = green = blue = lightness * 255 / 100;
    }
    else
    {
        if (lightness < 50)
        {
            var2 = lightness * (100 + saturation);
        }
        else
        {
            var2 = ((lightness + saturation) * 100) - (saturation * lightness);
        }
        var1 = lightness * 200 - var2;
        red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
        green = h2rgb(var1, var2, hue) * 255 / 600000;
        blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
    }
    return (red << 16) | (green << 8) | blue;
}

unsigned int rainbow::h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
    if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
    if (hue < 180) return v2 * 60;
    if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
    return v1 * 60;
}


