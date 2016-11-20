#ifndef __SIMPLEAUDIO_H__
#define __SIMPLEAUDIO_H__

#include "ledMatrix.h"
#include "animation.h"
#include "definition.h"

#include <fftw3.h>

class simpleaudio: public animation
{
public:
    simpleaudio(ledMatrix* leds);
    virtual ~simpleaudio();
    void start();
private:
    ledMatrix* m_leds;
    float* window;

    fftw_complex *output;
    fftw_plan plan;
    int height, width;
    uint32_t samples_count;
    double *samples;
    double max;
};

#endif
