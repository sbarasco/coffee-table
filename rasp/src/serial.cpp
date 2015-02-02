#include "serial.h"
#include <iostream>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

serial::serial(std::string device): m_fd(0), m_device(device)
{
}

serial::~serial()
{
    close();
}

bool serial::open(int baud)
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

ssize_t serial::write(const char *data, int len)
{
    if(m_fd != 0)
        return ::write(m_fd, data, len);
}

ssize_t serial::read(char *buf, int len)
{
//    int count=0;
// int r;
// int retry=0;
// //char buf[512];

// if (len > sizeof(buf) || len < 1) return -1;
// // non-blocking read mode
// fcntl(port, F_SETFL, fcntl(port, F_GETFL) | O_NONBLOCK);
// while (count < len) {
//     r = read(port, buf + count, len - count);
//     //printf("read, r = %d\n", r);
//     if (r < 0 && errno != EAGAIN && errno != EINTR) return -1;
//     else if (r > 0) count += r;
//     else {
//         // no data available right now, must wait
//         fd_set fds;
//         struct timeval t;
//         FD_ZERO(&fds);
//         FD_SET(port, &fds);
//         t.tv_sec = 1;
//         t.tv_usec = 0;
//         r = select(port+1, &fds, NULL, NULL, &t);
//         //printf("select, r = %d\n", r);
//         if (r < 0) return -1;
//         if (r == 0) return count; // timeout
//     }
//     retry++;
//     if (retry > 1000) return -100; // no input
// }
// fcntl(port, F_SETFL, fcntl(port, F_GETFL) & ~O_NONBLOCK);
// return count;
    return 1;
}

bool serial::close()
{
    if(m_fd != 0)
    {
        ::close(m_fd);
        m_fd = 0;
    }
}

