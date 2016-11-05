#include "pong.h"
#include "unistd.h"

pong::pong(ledMatrix* leds): m_leds(leds)
{
}

void pong::step()
{
    for(int y = 0; y < NBLINES; y++)
    {
        m_leds->setPixel(5, y, RED);
        m_leds->setPixel(6, y, BLUE);
    }
    for(int y = 6; y < 14; y++)
    {
        m_leds->setPixel(0, y, RED);
        m_leds->setPixel(LEDPERLINE-1, y, BLUE);
    }
    for(int y = 8; y < 12; y++)
    {
        m_leds->setPixel(1, y, RED);
        m_leds->setPixel(LEDPERLINE-2, y, BLUE);
    }
    m_leds->update();

}
