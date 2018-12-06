// Author : Jman420

#include <cstdlib>
#include "fir_generator.h"

double* FIR_Generator::Create(int interpolationSize, double* frequencySamples, double* amplitudeSamples, int sampleSize) {
    // Validate Frequency Samples
    if (frequencySamples[0] != 0 || frequencySamples[sampleSize - 1] != 1) {
        // TODO : Add Error Log Line re: Invalid Frequency Samples
        return 0;
    }
    for (int freqCounter = 0; freqCounter < sampleSize - 1; freqCounter++) {
        if (frequencySamples[freqCounter] >= frequencySamples[freqCounter + 1]) {
            // TODO : Add Error Log Line re: Invalid Frequency Samples
            return 0;
        }
    }

    // Interpolate Amplitudes
    double amplitudeIncrement;
    int beginSegmentIndex, endSegmentIndex = 0;

    double* interpolatedAmplitudes = (double*)malloc(sizeof(double)*interpolationSize);
    interpolatedAmplitudes[0] = amplitudeSamples[0];

    for (int amplitudeCounter = 0; amplitudeCounter < sampleSize - 1; amplitudeCounter++) {
        endSegmentIndex = (int)(frequencySamples[amplitudeCounter + 1] * interpolationSize) - 1;
        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            // TODO : Add Error Log Line re: Amplitude change out of bounds
            return 0;
        }

        for (int elementCounter = beginSegmentIndex; elementCounter < endSegmentIndex; elementCounter++) {
            amplitudeIncrement = (double)(elementCounter - beginSegmentIndex) / (double)(endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitudes[elementCounter] = amplitudeIncrement * amplitudeSamples[amplitudeCounter + 1] + (1.0 - amplitudeIncrement) * amplitudeSamples[amplitudeCounter];
        }

        beginSegmentIndex = endSegmentIndex + 1;
    }


}