#include "serial.h"
#include <iostream>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

serial::serial(std::string device): virtsocket(), m_device(device), m_baud(115200)
{
}

serial::~serial()
{
    close();
}

bool serial::open()
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
    if (cfsetspeed(&tinfo, m_baud) < 0)
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

void serial::setbaud(int baud)
{
    m_baud = baud;
}

