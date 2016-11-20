#ifndef __PONG_H__
#define __PONG_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class pong: public animation
{
public:
    pong(ledMatrix* leds);
    void step();
    std::string get_page_name();
    void get_page(std::stringstream& page_stream);
    void set_parameters(char *params);
private:
    ledMatrix* m_leds;
    uint32_t m_color_p1;
    uint32_t m_color_p2;
};

#endif
