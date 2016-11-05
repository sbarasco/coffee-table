#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([table-ctrl], [1], [sbarasco@gmail.com])
AC_CONFIG_SRCDIR([src/processing.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
CXXFLAGS+=" -std=c++11"

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

#check fftw3 for audio mode
PKG_CHECK_MODULES([FFTW3], fftw3, [], AC_MSG_ERROR([libfftw3 not found]))
AC_SUBST(FFTW3_LIBS)
AC_SUBST(FFTW3_CFLAGS)
PKG_CHECK_MODULES([PULSE], libpulse-simple, [], AC_MSG_ERROR([libpulse-simple not found]))
AC_SUBST(PULSE_LIBS)
AC_SUBST(PULSE_CFLAGS)
PKG_CHECK_MODULES([EVENT], libevent, [], AC_MSG_ERROR([libevent not found]))
AC_SUBST(EVENT_LIBS)
AC_SUBST(EVENT_CFLAGS)


# Checks for library functions.
AC_CHECK_FUNCS([memset select])

AC_CONFIG_FILES(src/Makefile Makefile)

AC_OUTPUT
