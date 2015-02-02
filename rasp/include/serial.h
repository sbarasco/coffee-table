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
    serial(std::string device);
    ~serial();
    bool open(int baud);
    ssize_t write(const char *data, int len);
    ssize_t read(char *buf, int len);
    bool close();
private:
    int m_fd;
    std::string m_device;
};

#endif
