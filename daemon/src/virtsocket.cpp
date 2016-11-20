#include "virtsocket.hpp"
#include <iostream>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


virtsocket::virtsocket(): m_fd(0)
{
}

virtsocket::~virtsocket()
{
}

ssize_t virtsocket::write(const char *data, int len)
{
    if(m_fd != 0)
        return ::write(m_fd, data, len);
}

bool virtsocket::close()
{
    if(m_fd != 0)
    {
        ::close(m_fd);
        m_fd = 0;
    }
}
