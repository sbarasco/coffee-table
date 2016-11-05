#include "border.h"
#include "unistd.h"

#define BORDER_COLOR WHITE

border::border(ledMatrix* leds): m_leds(leds)
{
}

void border::step()
{
    for(int i = 0; i < LEDPERLINE; i++)
    {
        m_leds->setPixel(i, 0, WHITE);
        m_leds->setPixel(i, NBLINES-1, WHITE);
    }
    for(int i = 0; i < NBLINES; i++)
    {
        m_leds->setPixel(0, i, WHITE);
        m_leds->setPixel(LEDPERLINE-1, i, WHITE);
    }
    m_leds->update();
}
