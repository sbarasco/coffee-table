#ifndef __LEDMATRIX_H__
#define __LEDMATRIX_H__

#include <sys/cdefs.h>
#include <stdint.h>
#include "socket.hpp"

#define LEDPERLINE 12
#define COLPERSTRIP 4
#define NBLINES    20
#define LEDPERSTRIP 48


class ledMatrix
{
public:
    ledMatrix(socket* socket);
    ~ledMatrix();
    void setPixel(uint32_t num, int32_t color);
    void setPixel(uint32_t x, uint32_t y, int32_t color);
    void update();
    void clean();
private:
    socket* m_socket;
    int32_t drawingMemory[LEDPERSTRIP*6];
};

#endif
