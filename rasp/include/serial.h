#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <string>
extern "C"
{
#include <unistd.h>
}
#include "socket.hpp"

class serial: public socket
{
public:
    serial(std::string device);
    ~serial();
    virtual bool open();
    void setbaud(int baud);
private:
    std::string m_device;
    int m_baud;
};

#endif
