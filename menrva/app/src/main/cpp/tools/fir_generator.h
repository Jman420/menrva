// Author : Jman420

#ifndef MENRVA_FIR_GENERATOR_H
#define MENRVA_FIR_GENERATOR_H


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
    double* Create(int interpolationSize, double* frequencySamples, double* amplitudeSamples, int sampleSize);
};


#endif //MENRVA_FIR_GENERATOR_H
