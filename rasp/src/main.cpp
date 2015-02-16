#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include "ledMatrix.h"
#include "rainbow.h"
#include "plasma.h"
#include "pong.h"
#include "serial_unix.h"

#define BAUD 115200

int main(int argc, char **argv)
{
    serial_unix serial("/dev/ttyACM0");
    serial.open(BAUD);
    usleep(100000);
    ledMatrix leds(&serial);
    // rainbow rb(&leds, VERTICAL);
    // rb.start();
    plasma pl(&leds);
    pl.start();
    // pong pg(&leds);
    // pg.start();
    // for(int i = 0; i < LEDPERLINE; i++)
    // {
    //     for(int j = 0; j < NBLINES; j++)
    //     {
    //         leds.setPixel(i, j, 0xFF0000);
    //     }
    // }
    // for(int i = 4; i < 8; i++)
    // {
    //     for(int j = 0; j < NBLINES; j++)
    //     {
    //         leds.setPixel(i, j, 0x00FF00);
    //     }
    // }
    // for(int i = 8; i < 12; i++)
    // {
    //     for(int j = 0; j < NBLINES; j++)
    //     {
    //         leds.setPixel(i, j, 0x0000FF);
    //     }
    // }
    leds.update();
    return 0;
}
