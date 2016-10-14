#include "tablethread.hpp"
#include "constants.h"

#include <QtNetwork>
#include <iostream>
#include <iomanip>

TableThread::TableThread(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
}

quint32 TableThread::getPixel(quint32 x, quint32 y, char* drawBuffer)
{
    if(y%2 == 0)
    {
        return getPixel(x+y*LEDPERLINE, drawBuffer);
    }
    else
    {
        return getPixel(((y+1)*LEDPERLINE)-(x+1), drawBuffer);
    }
}

quint32 TableThread::getPixel(quint32 num, char* drawBuffer)
{
    quint32 strip, offset, mask;
    quint8 bit, *p;
    quint32 color=0;

    strip = num / LEDPERSTRIP;
    offset = num % LEDPERSTRIP;
    bit = (1<<strip);
    p = ((quint8 *)drawBuffer) + offset * 24;
    for (mask = (1<<23) ; mask ; mask >>= 1) {
        if (*p++ & bit) color |= mask;
    }
    color = ((color<<8)&0xFFFF00) | ((color>>16)&0x0000FF);
    return color;
}

void TableThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    QByteArray draw_data;
    while(tcpSocket.waitForReadyRead(-1))
    {
        QByteArray data = tcpSocket.readAll();
        for(int i = 0; i < data.size(); i++)
        {
            if(data[i] == '*')
            {
                // draw previous buffer
                if(draw_data.size() == (sizeof(quint32)*LEDPERSTRIP*6))
                {
                    QVector<quint32> pixels(NBLINES*LEDPERLINE, 0);
                    for(int y = 0; y <  NBLINES; y ++)
                    {
                        for(int x = 0; x < LEDPERLINE; x ++)
                        {
                            int p = x+LEDPERLINE*y;
                            pixels[p] = getPixel(x, y, draw_data.data());
                        }
                    }
                    emit data_ready(pixels);
                }
                draw_data.clear();
            }
            else
            {
                draw_data.push_back(data[i]);
            }
        }
    }
}

