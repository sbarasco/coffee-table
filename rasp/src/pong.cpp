#include "pong.h"

pong::pong(ledMatrix* leds): m_leds(leds)
{
}

void pong::start()
{
    for(int y = 0; y < NBLINES; y++)
    {
        m_leds->setPixel(5, y, 0xFF0000);
        m_leds->setPixel(6, y, 0x0000FF);
    }
    for(int y = 6; y < 14; y++)
    {
        m_leds->setPixel(0, y, 0xFF0000);
        m_leds->setPixel(LEDPERLINE-1, y, 0x0000FF);
    }
    for(int y = 8; y < 12; y++)
    {
        m_leds->setPixel(1, y, 0xFF0000);
        m_leds->setPixel(LEDPERLINE-2, y, 0x0000FF);
    }
    m_leds->update();
}
