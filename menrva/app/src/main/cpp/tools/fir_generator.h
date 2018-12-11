// Author : Jman420

#ifndef MENRVA_FIR_GENERATOR_H
#define MENRVA_FIR_GENERATOR_H

#include <cstdlib>
#include <math.h>
#include <string>

class FIR_Generator {
public:
    /*
     * Create() - Returns a Finite Impulse Response of size 'interpolationSize' based on the provided
     * Frequency Samples & Amplitudes.
     * interpolationSize - Size of resulting FIR Filter (number of elements in the filter; 2048 or 4096)
     * frequencySamples - Array of Frequency values to interpolate
     * amplitudeSamples - Array of Amplitude values to interpolate
     * sampleSize - Length of Frequency & Amplitude Arrays
     */
    double* Create(unsigned int filterSize, double* frequencySamples, double* amplitudeSamples, int sampleSize);

private:
    static const std::string LOG_TAG;
    static constexpr double PI2 = M_PI * 2;
};

#endif //MENRVA_FIR_GENERATOR_H
