#ifndef __BORDER_H__
#define __BORDER_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class border: public animation
{
public:
    border(ledMatrix* leds);
    void step();
    std::string get_page_name();
    void get_page(std::stringstream& page_stream);
    void set_parameters(char *params);
private:
    ledMatrix* m_leds;
    uint32_t m_color;
};

#endif
