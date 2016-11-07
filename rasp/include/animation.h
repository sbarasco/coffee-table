#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "color.h"
#include "resources.h"
#include <sstream>

class animation
{
public:
    virtual void step() = 0;
    virtual std::string get_page_name() = 0;
    virtual void get_page(std::stringstream& page_stream) = 0;
    virtual void set_parameters(char *params) = 0;
protected:
    animation(){}
};

#endif
