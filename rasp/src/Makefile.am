bin_PROGRAMS = tablectrl
tablectrl_SOURCES = main.cpp \
	virtsocket.cpp \
	serial.cpp \
	tcpsock.cpp \
	ledMatrix.cpp \
	rainbow.cpp \
	plasma.cpp \
	pong.cpp \
	simpleaudio.cpp \
	border.cpp

tablectrl_CPPFLAGS = -I@abs_top_srcdir@/include @PULSE_CFLAGS@ @FFTW3_CFLAGS@
tablectrl_LDADD = @PULSE_LIBS@ @FFTW3_LIBS@ @EVENT_LIBS@
