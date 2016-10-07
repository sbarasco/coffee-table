#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
extern "C"
{
#include <unistd.h>
}

class socket
{
public:
    socket();
    ~socket();
    virtual bool open() = 0;
    ssize_t write(const char *data, int len);
    bool close();
protected:
    int m_fd;
};

#endif
