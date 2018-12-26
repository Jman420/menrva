// Author : Jman420

#ifndef MENRVA_FIR_GENERATOR_H
#define MENRVA_FIR_GENERATOR_H

#include <string>
#include <math.h>
#include "../abstracts/fft_interface_base.h"

class FIR_Generator {
public:
    FIR_Generator(FFTInterfaceBase* fftEngine);

    /*
     * Create() - Returns a Finite Impulse Response of size 'interpolationSize' based on the provided
     * Frequency Samples & Amplitudes.
     * interpolationSize - Size of resulting FIR Filter (number of elements in the filter; 2048 or 4096)
     * frequencySamples - Array of Frequency values to interpolate
     * amplitudeSamples - Array of Amplitude values to interpolate
     * sampleSize - Length of Frequency & Amplitude Arrays
     */
    float* Create(unsigned int filterSize, float* frequencySamples, float* amplitudeSamples, unsigned int sampleSize);

private:
    static const std::string LOG_TAG;
    static constexpr float PI2 = (float)M_PI * 2.0f;

    FFTInterfaceBase* _FFTEngine;
};

#endif //MENRVA_FIR_GENERATOR_H
