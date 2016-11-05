#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "color.h"

class animation
{
public:
    virtual void step() = 0;
protected:
    animation(){}
};

#endif
