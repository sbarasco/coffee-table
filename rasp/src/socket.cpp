#include "socket.h"
#include <iostream>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

socket::socket(): m_fd(0)
{
}

socket::~socket()
{
}

ssize_t socket::write(const char *data, int len)
{
    if(m_fd != 0)
        return ::write(m_fd, data, len);
}

bool socket::close()
{
    if(m_fd != 0)
    {
        ::close(m_fd);
        m_fd = 0;
    }
}
