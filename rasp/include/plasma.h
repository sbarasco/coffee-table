#ifndef __PLASMA_H__
#define __PLASMA_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class plasma: public animation
{
public:
    plasma(ledMatrix* leds);
    void step();
    std::string get_page_name();
    void get_page(std::stringstream& page_stream);
    void set_parameters(char *params);
private:
    ledMatrix* m_leds;
    unsigned long m_frameCount;
};

#endif
