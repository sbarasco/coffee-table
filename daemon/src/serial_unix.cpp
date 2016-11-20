#include "serial_unix.h"
#include <iostream>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

serial_unix::serial_unix(std::string device): m_fd(0), m_device(device)
{
}

serial_unix::~serial_unix()
{
    close();
}

bool serial_unix::open(int baud)
{
    struct termios tinfo;
    struct serial_struct kernel_serial_settings;
    int r;
    m_fd = ::open(m_device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        std::cerr << "unable to open port " << m_device << std::endl;
        return false;
    }
    if (tcgetattr(m_fd, &tinfo) < 0)
    {
        std::cerr << "unable to get serial parms" << std::endl;
        return false;
    }
    cfmakeraw(&tinfo);
    if (cfsetspeed(&tinfo, baud) < 0)
    {
        std::cerr << "error in cfsetspeed\n" << std::endl;
        return false;
    }
    if (tcsetattr(m_fd, TCSANOW, &tinfo) < 0)
    {
        std::cerr << "unable to set baud rate\n" << std::endl;
    }
    r = ioctl(m_fd, TIOCGSERIAL, &kernel_serial_settings);
    if (r >= 0)
    {
        kernel_serial_settings.flags |= ASYNC_LOW_LATENCY;
        r = ioctl(m_fd, TIOCSSERIAL, &kernel_serial_settings);
        if (r >= 0)
        {
            std::cout << "set linux low latency mode" << std::endl;
        }
    }
    return true;
}

ssize_t serial_unix::write(const char *data, int len)
{
    if(m_fd != 0)
    {
        struct pollfd fds[1];
        fds[0].fd = m_fd;
        fds[0].events = POLLOUT;
        fds[0].revents = 0;
        poll(fds, 1, -1);
        return ::write(m_fd, data, len);
    }
}

ssize_t serial_unix::read(char *buf, int len)
{
    if(m_fd != 0)
    {
        struct pollfd fds[1];
        fds[0].fd = m_fd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        poll(fds, 1, -1);
        return ::read(m_fd, buf, len);
    }
}

bool serial_unix::close()
{
    if(m_fd != 0)
    {
        ::close(m_fd);
        m_fd = 0;
    }
}

