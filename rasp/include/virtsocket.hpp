#ifndef __VIRTSOCKET_H__
#define __VIRTSOCKET_H__

#include <string>

class virtsocket
{
public:
    virtsocket();
    ~virtsocket();
    virtual bool open() = 0;
    ssize_t write(const char *data, int len);
    bool close();
protected:
    int m_fd;
};

#endif
