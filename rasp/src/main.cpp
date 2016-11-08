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
#include <signal.h>
#include <iostream>
#include <map>

#include "serial.h"
#include "virtsocket.hpp"
#include "tcpsock.hpp"
#include "ledMatrix.h"
#include "rainbow.h"
#include "plasma.h"
#include "pong.h"
#include "border.h"

#include <evhttp.h>
#include "http/index_html.h"
#include "http/header_page_html.h"
#include "http/footer_page_html.h"
#include "http/bootstrap_min_css.h"
#include "http/metisMenu_min_css.h"
#include "http/sb-admin-2_css.h"
#include "http/bootstrap_min_js.h"
#include "http/metisMenu_min_js.h"
#include "http/sb-admin-2_js.h"
#include "http/menu_html.h"
#include "http/jquery_min_js.h"



#define BAUD 115200

using namespace std;

animation* current_anim = NULL;

static void
refresh_cb(evutil_socket_t fd, short event, void *arg)
{
    // animation** anim = (animation**)arg;
    current_anim->step();
}

#define REPLY_PAGE(type, page)                                  \
    evhttp_add_header(evhttp_request_get_output_headers(req),   \
                      "Content-Type", type);                    \
    evbuffer_add(evb, page, page##_len);                        \
    evhttp_send_reply(req, HTTP_OK, "OK", evb);

static void
generic_cb(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = NULL;
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    const char *path;
    char *decoded_path;

    if (evhttp_request_get_command(req) == EVHTTP_REQ_GET)
    {
        evb = evbuffer_new();
        /* Decode the URI */
        decoded = evhttp_uri_parse(uri);
        cout << "GET request " << uri << endl;
        if (!decoded)
        {
            printf("It's not a good URI. Sending BADREQUEST\n");
            evhttp_send_error(req, HTTP_BADREQUEST, 0);
            return;
        }
        /* Let's see what path the user asked for. */
	path = evhttp_uri_get_path(decoded);
	if (!path) path = "/";
        /* We need to decode it, to see what path the user really wanted. */
	decoded_path = evhttp_uridecode(path, 0, NULL);
	if (decoded_path == NULL)
        {
            evhttp_send_error(req, 404, "Document was not found");
        }
        else if(string(decoded_path) == string("/index.html") ||
           string(decoded_path) == string("/"))
        {
            REPLY_PAGE("text/html", index_html);
        }
        else if(string(decoded_path) == string("/bootstrap.min.css"))
        {
            REPLY_PAGE("text/css", bootstrap_min_css);
        }
        else if(string(decoded_path) == string("/metisMenu.min.css"))
        {
            REPLY_PAGE("text/css", metisMenu_min_css);
        }
        else if(string(decoded_path) == string("/sb-admin-2.css"))
        {
            REPLY_PAGE("text/css", sb_admin_2_css);
        }
        else if(string(decoded_path) == string("/bootstrap.min.js"))
        {
            REPLY_PAGE("application/javascript", bootstrap_min_js);
        }
        else if(string(decoded_path) == string("/metisMenu.min.js"))
        {
            REPLY_PAGE("application/javascript", metisMenu_min_js);
        }
        else if(string(decoded_path) == string("/sb-admin-2.js"))
        {
            REPLY_PAGE("application/javascript", sb_admin_2_js);
        }
        else if(string(decoded_path) == string("/menu.html"))
        {
            REPLY_PAGE("text/html", menu_html);
        }
        else if(string(decoded_path) == string("/jquery.min.js"))
        {
            REPLY_PAGE("application/javascript", jquery_min_js);
        }
        else
        {
            evhttp_send_error(req, HTTP_BADREQUEST, 0);
        }
    }
    if (evb)
        evbuffer_free(evb);
}

static void
animation_request_cb(struct evhttp_request *req, void *arg)
{
    std::stringstream resp;
    struct evbuffer *evb = NULL;
    animation* anim = (animation*)arg;
    anim->get_page(resp);
    if (evhttp_request_get_command(req) == EVHTTP_REQ_GET ||
        evhttp_request_get_command(req) == EVHTTP_REQ_POST)
    {
        evb = evbuffer_new();
        evhttp_add_header(evhttp_request_get_output_headers(req),
                          "Content-Type", "text/html");
        evbuffer_add(evb, resp.str().c_str(), resp.str().size());
        evhttp_send_reply(req, HTTP_OK, "OK", evb);
        if(evhttp_request_get_command(req) == EVHTTP_REQ_POST)
        {
            // struct evkeyvalq *headers;
            // struct evkeyval *header;
            struct evbuffer *buf;
            current_anim = anim;
            // headers = evhttp_request_get_input_headers(req);
            // for (header = headers->tqh_first; header;
            //      header = header->next.tqe_next) {
	    //     printf("  %s: %s\n", header->key, header->value);
            // }
            buf = evhttp_request_get_input_buffer(req);
            string param((char*)evbuffer_pullup(buf, -1), evbuffer_get_length(buf));
            cout << param << endl;
            // anim->set_parameters((char*)evbuffer_pullup(buf, -1));
            anim->set_parameters((char*)param.c_str());
            // puts("Input data: <<<");
        }
    }
    if (evb)
        evbuffer_free(evb);
}

int main(int argc, char **argv)
{
    string device("/dev/ttyACM0");
    char *socket_opt = NULL;
    int c;
    int use_tcp = 0;
    int index;
    virtsocket *comm;
    struct event timeout;
    struct timeval tv;
    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;
    int port = 8080;
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
        device = string(socket_opt);
        tcpsock tcpcomm(device);
        comm = &tcpcomm;
    }
    else
    {
        if(socket_opt)
        {
            device = string(socket_opt);
        }
        serial serial(device);
        serial.setbaud(BAUD);
        comm = &serial;
    }
    cout << "open device " << device << endl;
    if(!comm->open())
    {
        return 1;
    }
    ledMatrix leds(comm);
    // animation* anim = NULL;
    // anim = new plasma(&leds);
    current_anim = new rainbow(&leds, VERTICAL);
    // rainbow rb(&leds, VERTICAL);
    // rb.start();
    // plasma pl(&leds);
    // pl.start();
    // pong pg(&leds);
    // pg.start();
    // border br(&leds);
    // br.start();
    signal(SIGPIPE, SIG_IGN);
    base = event_base_new();
    // add event to refresh animation
    event_assign(&timeout, base, -1, EV_PERSIST, refresh_cb, (void*) &current_anim);
    evutil_timerclear(&tv);
    tv.tv_sec = 0;
    /* 60 FPS */
    tv.tv_usec = 1000000/60;
    event_add(&timeout, &tv);
    // add event to manage http server
    http = evhttp_new(base);
    if (!http)
    {
        fprintf(stderr, "couldn't create evhttp. Exiting.\n");
        return 1;
    }
    evhttp_set_gencb(http, generic_cb, (void*) &current_anim);

    evhttp_set_cb(http, "/rainbow.html", animation_request_cb, current_anim);
    evhttp_set_cb(http, "/pong.html", animation_request_cb, new pong(&leds));
    evhttp_set_cb(http, "/plasma.html", animation_request_cb, new plasma(&leds));
    evhttp_set_cb(http, "/border.html", animation_request_cb, new border(&leds));
    /* Now we tell the evhttp what port to listen on */
    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
    if (!handle)
    {
        fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
                (int)port);
        return 1;
    }
    event_base_dispatch(base);

    return 0;
}
