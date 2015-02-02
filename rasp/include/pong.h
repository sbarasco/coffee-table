#ifndef __PONG_H__
#define __PONG_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class pong: public animation
{
public:
    pong(ledMatrix* leds);
    void start();
private:
    ledMatrix* m_leds;
};

#endif
