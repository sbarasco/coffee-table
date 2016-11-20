#include <complex.h>
#include <fftw3.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#include "simpleaudio.hpp"

#define AUDIO_DEVICE "alsa_output.pci-0000_08_04.0.analog-stereo.monitor"

#define FFT_N 1024
//#define M_PI 3.14159265

static const pa_sample_spec ss = {
    // .format = PA_SAMPLE_S16LE,
    .format = PA_SAMPLE_FLOAT32LE,
    .rate = 44100,
    .channels = 1
};

simpleaudio::simpleaudio(ledMatrix* leds): m_leds(leds)
{
    samples_count = ss.rate/100;
    width = samples_count/2;
    samples =  fftw_alloc_real(samples_count);
    output = fftw_alloc_complex(samples_count);
    memset(samples, 0, sizeof(double)*samples_count);
    memset(output, 0, sizeof(fftw_complex)*samples_count);
    plan = fftw_plan_dft_r2c_1d(samples_count,
                                samples, output, 0);
    height = NBLINES;
    width = LEDPERLINE;
    window  = new float[samples_count];
    for(int i = 0; i < samples_count; i++)
    {
        window[i] = 0.5 * (1 - cos((2 * M_PI * i) / (samples_count - 1)));
    }
}

simpleaudio::~simpleaudio()
{

    delete window;
}

void simpleaudio::start()
{
    pa_simple *s = NULL;

    int error;

    height = NBLINES;
    width = LEDPERLINE;
    // holder.plan = fftw_plan_dft_1d(holder.samples_count,
    //                                 holder.samples, holder.output,
    //                                 FFTW_FORWARD, FFTW_ESTIMATE);

    float buf[samples_count];

    if (!(s = pa_simple_new(NULL, "table.visualization",
                            PA_STREAM_RECORD, AUDIO_DEVICE,
                            "output visualieation", &ss,
                            NULL, NULL, &error)))
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    while(1)
    {
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0)
        {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        for (int a = 0; a < samples_count; a++)
        {
            samples[a] = buf[a]*window[a];
        }
        fftw_execute(plan);
    }

    finish:
    if (s)
        pa_simple_free(s);
}
