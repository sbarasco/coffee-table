#ifndef __RAINBOW_H__
#define __RAINBOW_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class rainbow: public animation
{
public:
    rainbow(ledMatrix* leds, int sens = VERTICAL);
    void step();
    virtual std::string get_page_name(){return std::string();}
    virtual void get_page(std::stringstream& page_stream){}
    void set_parameters(char *params){}
private:
    int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);
    unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);
    int rainbowColors[180];
    int m_sens;
    ledMatrix* m_leds;
    int m_color;
};

#endif
