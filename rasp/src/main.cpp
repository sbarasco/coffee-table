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

#include "serial.h"
#include "virtsocket.hpp"
#include "tcpsock.hpp"
#include "ledMatrix.h"
#include "rainbow.h"
#include "plasma.h"
#include "pong.h"
#include "border.h"

#include <evhttp.h>

// #include <event2/event.h>
// #include <event2/event_struct.h>
// #include <event2/util.h>

#define BAUD 115200

struct timeval lasttime;
static void
refresh_cb(evutil_socket_t fd, short event, void *arg)
{
    animation** anim = (animation**)arg;
    (*anim)->step();
}

int main(int argc, char **argv)
{
    std::string device("/dev/ttyACM0");
    char *socket_opt = NULL;
    int c;
    int use_tcp = 0;
    int index;
    virtsocket *comm;
    struct event timeout;
    struct timeval tv;
    struct event_base *base;

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
    animation* anim;
    // anim = new plasma(&leds);
    anim = new rainbow(&leds, VERTICAL);
    // rainbow rb(&leds, VERTICAL);
    // rb.start();
    // plasma pl(&leds);
    // pl.start();
    // pong pg(&leds);
    // pg.start();
    // border br(&leds);
    // br.start();
    base = event_base_new();
    event_assign(&timeout, base, -1, EV_PERSIST, refresh_cb, (void*) &anim);
    evutil_timerclear(&tv);
    tv.tv_sec = 0;
    /* 60 FPS */
    tv.tv_usec = 1000000/60;
    event_add(&timeout, &tv);
    evutil_gettimeofday(&lasttime, NULL);
    event_base_dispatch(base);

    return 0;
}
