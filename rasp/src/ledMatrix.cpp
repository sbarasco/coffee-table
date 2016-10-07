#include "ledMatrix.h"
#include <string.h>
#include <iostream>

ledMatrix::ledMatrix(socket* socket): m_socket(socket)
{
    clean();
}

ledMatrix::~ledMatrix()
{
}

void ledMatrix::clean()
{
    memset(drawingMemory, 0, sizeof(drawingMemory));
}

void ledMatrix::setPixel(uint32_t num, int32_t color)
{
    int stripLen = LEDPERSTRIP;
    uint32_t strip, offset, mask;
    uint8_t bit, *p;
    color = ((color<<16)&0xFF0000) | ((color>>8)&0x00FFFF);

    strip = num / stripLen;
    offset = num % stripLen;
    bit = (1<<strip);
    p = ((uint8_t *)drawingMemory) + offset * 24;
    for (mask = (1<<23) ; mask ; mask >>= 1) {
        if (color & mask) {
            *p++ |= bit;
        } else {
            *p++ &= ~bit;
        }
    }
}

void ledMatrix::setPixel(uint32_t x, uint32_t y, int32_t color)
{
    if(y%2 == 0)
    {
        setPixel(x+y*LEDPERLINE, color);
    }
    else
    {
        // setPixel((LEDPERLINE-1-x)+(y*LEDPERLINE), color);
        setPixel(((y+1)*LEDPERLINE)-(x+1), color);
    }
}


void ledMatrix::update()
{
    m_socket->write("*", 1);
    if(m_socket->write((char*)drawingMemory, sizeof(drawingMemory)) != sizeof(drawingMemory))
    {
        std::cout << "update incomplete" << std::endl;
    }
}
