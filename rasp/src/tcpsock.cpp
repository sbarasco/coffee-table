#include "tcpsock.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <regex>

tcpsock::tcpsock(std::string address): socket(), m_addr(address)
{
}

tcpsock::~tcpsock()
{
    close();
}

bool tcpsock::open()
{
    int rv;
    struct addrinfo hints, *servinfo, *p;
    std::string hostname("localhost");
    int hostport;
    std::string hostport_str;
    std::regex exp ("([^:]*):([0-9]+)");
    std::cmatch cm;
    std::regex_match(m_addr.c_str(), cm, exp, std::regex_constants::match_default);
    if(cm.size() == 3)
    {
        if(!std::string(cm[1]).empty())
        {
            hostname = cm[1];
        }
        hostport = std::stoi(cm[2], nullptr, 0);
        hostport_str = cm[2];
    }
    else
    {
        return false;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(hostname.c_str(), hostport_str.c_str(), &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo: " <<  gai_strerror(rv) << std::endl;
        return false;
    }
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((m_fd = ::socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }
        if (connect(m_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            ::close(m_fd);
            perror("client: connect");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        std::cerr << "client: failed to connect" << std::endl;
        return false;
    }
    return true;
}


