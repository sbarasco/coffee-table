#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <string>
extern "C"
{
#include <unistd.h>
}

class serial
{
public:
    serial() {}
    ~serial() {}
    virtual bool open(int baud) = 0;
    virtual ssize_t write(const char *data, int len) = 0;
    virtual ssize_t read(char *buf, int len) = 0;
    virtual bool close() = 0;
};

#endif
