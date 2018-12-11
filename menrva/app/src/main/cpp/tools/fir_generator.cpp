// Author : Jman420

#include "fir_generator.h"
#include "logger.h"

#include "../kissfft/kiss_fftr.h"

const std::string FIR_Generator::LOG_TAG = "Menrva-FIR_Generator - ";

// The following method is adapted from https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
int RoundToNextPowerOf2(unsigned int value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;

    return value;
}

double* FIR_Generator::Create(unsigned int filterSize, double* frequencySamples, double* amplitudeSamples, int sampleSize) {
    std::string logPrefix = LOG_TAG + "Create()";

    Logger::WriteLog("Validating Frequency Samples...", logPrefix);
    // Validate Frequency Samples
    if (frequencySamples[0] != 0 || frequencySamples[sampleSize - 1] != 1) {
        Logger::WriteLog("Invalid Frequency Samples Provided : Frequency Samples must begin with 0 and end with 1", logPrefix, LogLevel::ERROR);
        return 0;
    }
    for (int freqCounter = 0; freqCounter < sampleSize - 1; freqCounter++) {
        if (frequencySamples[freqCounter] >= frequencySamples[freqCounter + 1]) {
            Logger::WriteLog("Invalid Frequency Samples Provided : Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
            return 0;
        }
    }

    // Interpolate Amplitudes & Setup Fast Fourier Transform Plan
    double amplitudeIncrement = 0,
           fftRadianScalar = (double)(filterSize - 1) * 0.5 * M_PI,
           fftRadians = 0;
    int interpolationSize = RoundToNextPowerOf2(filterSize) + 1,
        beginSegmentIndex = 0,
        endSegmentIndex = 0,
        fftFrequencySize = interpolationSize * 2;

    double* interpolatedAmplitudes = (double*)malloc(sizeof(double) * (interpolationSize * 2));
    interpolatedAmplitudes[0] = amplitudeSamples[0];

    kiss_fft_cpx* fftFrequencyData = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * fftFrequencySize);

    for (int amplitudeCounter = 0; amplitudeCounter < sampleSize - 1; amplitudeCounter++) {
        endSegmentIndex = (int)(frequencySamples[amplitudeCounter + 1] * interpolationSize) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided : Amplitude change too great between indexes" + std::to_string(amplitudeCounter) + " and " + std::to_string(amplitudeCounter + 1);
            Logger::WriteLog(msg, logPrefix, LogLevel::ERROR);
            return 0;
        }

        for (int elementCounter = beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            // Interpolate Amplitudes
            amplitudeIncrement = (double)(elementCounter - beginSegmentIndex) / (double)(endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitudes[elementCounter] = amplitudeIncrement * amplitudeSamples[amplitudeCounter + 1] + (1.0 - amplitudeIncrement) * amplitudeSamples[amplitudeCounter];

            // Setup FFT Frequencies
            fftRadians = fftRadianScalar * (double)elementCounter / ((double)interpolationSize - 1.0);
            double realFreqData = interpolatedAmplitudes[elementCounter] * cos(fftRadians),
                   imaginaryFreqData = (interpolatedAmplitudes[elementCounter] * sin(fftRadians));

            fftFrequencyData[elementCounter].r = realFreqData;
            fftFrequencyData[elementCounter].i = imaginaryFreqData * -1.0;

            int reverseElementCounter = fftFrequencySize - elementCounter - 1;
            fftFrequencyData[reverseElementCounter].r = realFreqData;
            fftFrequencyData[reverseElementCounter].i = imaginaryFreqData;
        }

        beginSegmentIndex = endSegmentIndex + 1;
    }

    // Perform FFT
    int fftCalcSize = fftFrequencySize - 2;
    double* fftOutput = (double*)malloc(sizeof(double) * fftFrequencySize);
    kiss_fftr_cfg fftPlan = kiss_fftr_alloc(fftCalcSize, 1, 0, 0);
    kiss_fftri(fftPlan, fftFrequencyData, fftOutput);

    // Perform Hamming Window Smoothing
    double hammingIncrement = (double)filterSize - 1;
    double fftReductionScalar = 1.0 / fftCalcSize;
    double* firArray = (double*)malloc(sizeof(double) * filterSize);
    for (int elementCounter = 0; elementCounter < filterSize; elementCounter++) {
        firArray[elementCounter] = (0.54 - 0.46 * cos(PI2 * (double)elementCounter / hammingIncrement)) * fftOutput[elementCounter] * fftReductionScalar;
    }

    free(interpolatedAmplitudes);
    free(fftFrequencyData);
    free(fftOutput);
    free(fftPlan);

    return firArray;
}
