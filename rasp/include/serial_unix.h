#ifndef __SERIAL_UNIX_H__
#define __SERIAL_UNIX_H__

#include "serial.h"

class serial_unix: public serial
{
public:
    serial_unix(std::string device);
    ~serial_unix();
    bool open(int baud);
    ssize_t write(const char *data, int len);
    ssize_t read(char *buf, int len);
    bool close();
private:
    int m_fd;
    std::string m_device;
};

#endif
