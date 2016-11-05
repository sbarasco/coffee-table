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
private:
    ledMatrix* m_leds;
    unsigned long m_frameCount;
};

#endif
