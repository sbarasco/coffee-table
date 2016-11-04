#ifndef __BORDER_H__
#define __BORDER_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class border: public animation
{
public:
    border(ledMatrix* leds);
    void start();
private:
    ledMatrix* m_leds;
};

#endif
