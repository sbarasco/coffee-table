#ifndef __PLAIN_H__
#define __PLAIN_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

class plain: public animation
{
public:
    plain(ledMatrix* leds);
    ~plain();
    void step();
    std::string get_page_name();
    void get_page(std::stringstream& page_stream);
    void set_parameters(char *params);
private:
    ledMatrix* m_leds;
    uint32_t m_color;
    uint32_t m_motif;
    uint32_t **m_pattern;
};

#endif
