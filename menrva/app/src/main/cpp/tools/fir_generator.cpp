// Author : Jman420

#include "fir_generator.h"
#include "logger.h"
#include "../libs/fftconvolver/AudioFFT.h"

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

float* FIR_Generator::Create(unsigned int filterSize, float* frequencySamples, float* amplitudeSamples, int sampleSize) {
    std::string logPrefix = LOG_TAG + "Create()";

    // Validate Frequency Samples
    Logger::WriteLog("Validating Frequency Samples...", logPrefix, LogLevel::DEBUG);
    Logger::WriteLog("Sample Size : %d", logPrefix, LogLevel::VERBOSE, sampleSize);
    if (sampleSize < 2) {
        Logger::WriteLog("Invalid Samples Provided : Minimum Sample Size is 2.", logPrefix, LogLevel::ERROR);
        return 0;
    }
    int lastSampleIndex = sampleSize - 1;
    for (int freqCounter = 0; freqCounter < lastSampleIndex; freqCounter++) {
        Logger::WriteLog("Frequency Sample %d : %d", logPrefix, LogLevel::VERBOSE, freqCounter, frequencySamples[freqCounter]);
        if (frequencySamples[freqCounter] >= frequencySamples[freqCounter + 1]) {
            Logger::WriteLog("Invalid Frequency Samples Provided : Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
            return 0;
        }
    }
    Logger::WriteLog("Frequency Sample %d : %d", logPrefix, LogLevel::VERBOSE, lastSampleIndex, frequencySamples[lastSampleIndex]);
    if (frequencySamples[0] != 0 || frequencySamples[sampleSize - 1] != 1) {
        Logger::WriteLog("Invalid Frequency Samples Provided : Frequency Samples must begin with 0 and end with 1 IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
        return 0;
    }

    // Interpolate Amplitudes & Setup Fast Fourier Transform Frequencies
    Logger::WriteLog("Interpolating Amplitudes & Setting up FFT Frequencies...", logPrefix, LogLevel::DEBUG);
    float amplitudeIncrement = 0,
           interpolatedAmplitude = 0,
           fftRadianScalar = (float)((filterSize - 1) * 0.5 * M_PI),
           fftRadians = 0,
           interpolationSize = RoundToNextPowerOf2(filterSize) + 1,
           beginSegmentIndex = 0,
           endSegmentIndex = 0;
    int fftFrequencySize = (int)interpolationSize * 2;

    float* fftFrequenciesReal = (float*)malloc(sizeof(float) * fftFrequencySize);
    float* fftFrequenciesImaginary = (float*)malloc(sizeof(float) * fftFrequencySize);

    for (int amplitudeCounter = 0; amplitudeCounter < sampleSize - 1; amplitudeCounter++) {
        endSegmentIndex = (int)(frequencySamples[amplitudeCounter + 1] * interpolationSize) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided : Amplitude change too great between indexes" + std::to_string(amplitudeCounter) + " and " + std::to_string(amplitudeCounter + 1);
            Logger::WriteLog(msg, logPrefix, LogLevel::ERROR);
            return 0;
        }

        for (float elementCounter = beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            // Interpolate Amplitude
            amplitudeIncrement = (elementCounter - beginSegmentIndex) / (endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[amplitudeCounter + 1] + (1.0f - amplitudeIncrement) * amplitudeSamples[amplitudeCounter];

            // Setup FFT Frequencies
            fftRadians = fftRadianScalar * elementCounter / (interpolationSize - 1.0f);
            float realFreqData = interpolatedAmplitude * cos(fftRadians),
                  imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));

            int elementIndex = (int)elementCounter;
            fftFrequenciesReal[elementIndex] = realFreqData;
            fftFrequenciesImaginary[elementIndex] = imaginaryFreqData * -1.0f;

            int reverseElementCounter = fftFrequencySize - elementIndex - 1;
            fftFrequenciesReal[reverseElementCounter] = realFreqData;
            fftFrequenciesImaginary[reverseElementCounter] = imaginaryFreqData;
        }

        beginSegmentIndex = endSegmentIndex + 1.0f;
    }

    // Perform Inverse FFT (turn frequencies into a signal)
    size_t fftCalcSize = static_cast<size_t>(fftFrequencySize - 2);
    float* fftOutputSignal = (float*)malloc(sizeof(float) * fftFrequencySize);

    audiofft::AudioFFT fftEngine;
    fftEngine.init(fftCalcSize);
    fftEngine.ifft(fftOutputSignal, fftFrequenciesReal, fftFrequenciesImaginary);

    // Perform Hamming Window Smoothing
    float hammingIncrement = (float)filterSize - 1.0f;
    float fftReductionScalar = 1.0f / fftCalcSize;
    float* firArray = (float*)malloc(sizeof(float) * filterSize);
    for (int elementCounter = 0; elementCounter < filterSize; elementCounter++) {
        firArray[elementCounter] = (0.54f - 0.46f * cos(PI2 * (float)elementCounter / hammingIncrement)) * fftOutputSignal[elementCounter] * fftReductionScalar;
    }

    free(fftFrequenciesReal);
    free(fftFrequenciesImaginary);
    free(fftOutputSignal);

    return firArray;
}
