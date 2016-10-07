#ifndef __TCPSOCK_H__
#define __TCPSOCK_H__

#include <string>
#include "socket.hpp"

class tcpsock: public socket
{
public:
    tcpsock(std::string address);
    ~tcpsock();
    virtual bool open();
private:
    std::string m_addr;
};

#endif
