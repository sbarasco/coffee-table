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
#include "serial.h"
#include "socket.hpp"
#include "tcpsock.hpp"

#define BAUD 115200

int main(int argc, char **argv)
{
    std::string device("/dev/ttyACM0");
    char *socket_opt = NULL;
    int c;
    int use_tcp = 0;
    int index;
    socket *comm;
    while ((c = getopt (argc, argv, "t")) != -1)
    {
        switch (c)
        {
        case 't':
            use_tcp = 1;
            break;
        case '?':
        {
            if (isprint (optopt))
            {
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            {
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            }
            return 1;
        }
        default:
            abort ();
        }
    }
    for (index = optind; index < argc; index++)
    {
        socket_opt = argv[index];
    }
    if(use_tcp)
    {
        /* open tcp socket */
        if(!socket_opt)
        {
            return 1;
        }
        device = std::string(socket_opt);
        tcpsock tcpcomm(device);
        comm = &tcpcomm;
    }
    else
    {
        if(socket_opt)
        {
            device = std::string(socket_opt);
        }
        serial serial(device);
        serial.setbaud(BAUD);
        comm = &serial;
    }
    std::cout << "open device " << device << std::endl;
    if(!comm->open())
    {
        return 1;
    }
    ledMatrix leds(comm);
    // rainbow rb(&leds, VERTICAL);
    // rb.start();
    plasma pl(&leds);
    pl.start();
    return 0;
}
