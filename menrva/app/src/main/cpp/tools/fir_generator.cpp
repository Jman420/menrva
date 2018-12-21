// Author : Jman420

#include "fir_generator.h"
#include "logger.h"

#include "../libs/kissfft/kiss_fftr.h"
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

// TODO : Convert double to float; AudioFlinger provides & expects float32 as largest data type
double* FIR_Generator::Create(unsigned int filterSize, double* frequencySamples, double* amplitudeSamples, int sampleSize) {
    std::string logPrefix = LOG_TAG + "Create()";

    Logger::WriteLog("Validating Frequency Samples...", logPrefix, LogLevel::DEBUG);
    Logger::WriteLog("Sample Size : %d", logPrefix, LogLevel::VERBOSE, sampleSize);
    // Validate Frequency Samples
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
    double amplitudeIncrement = 0,
           interpolatedAmplitude = 0,
           fftRadianScalar = (double)(filterSize - 1) * 0.5 * M_PI,
           fftRadians = 0;
    int interpolationSize = RoundToNextPowerOf2(filterSize) + 1,
        beginSegmentIndex = 0,
        endSegmentIndex = 0,
        fftFrequencySize = interpolationSize * 2;

    kiss_fft_cpx* fftFrequencyData = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * fftFrequencySize);

    for (int amplitudeCounter = 0; amplitudeCounter < sampleSize - 1; amplitudeCounter++) {
        endSegmentIndex = (int)(frequencySamples[amplitudeCounter + 1] * interpolationSize) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided : Amplitude change too great between indexes" + std::to_string(amplitudeCounter) + " and " + std::to_string(amplitudeCounter + 1);
            Logger::WriteLog(msg, logPrefix, LogLevel::ERROR);
            return 0;
        }

        for (int elementCounter = beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            // Interpolate Amplitude
            amplitudeIncrement = (double)(elementCounter - beginSegmentIndex) / (double)(endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[amplitudeCounter + 1] + (1.0 - amplitudeIncrement) * amplitudeSamples[amplitudeCounter];

            // Setup FFT Frequencies
            fftRadians = fftRadianScalar * (double)elementCounter / ((double)interpolationSize - 1.0);
            double realFreqData = interpolatedAmplitude * cos(fftRadians),
                   imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));

            fftFrequencyData[elementCounter].r = realFreqData;
            fftFrequencyData[elementCounter].i = imaginaryFreqData * -1.0;

            int reverseElementCounter = fftFrequencySize - elementCounter - 1;
            fftFrequencyData[reverseElementCounter].r = realFreqData;
            fftFrequencyData[reverseElementCounter].i = imaginaryFreqData;
        }

        beginSegmentIndex = endSegmentIndex + 1;
    }

    // Perform Inverse FFT (turn frequencies into a signal)
    int fftCalcSize = fftFrequencySize - 2;
    double* fftOutputSignal = (double*)malloc(sizeof(double) * fftFrequencySize);
    kiss_fftr_cfg fftPlan = kiss_fftr_alloc(fftCalcSize, 1, 0, 0);
    kiss_fftri(fftPlan, fftFrequencyData, fftOutputSignal);

    // Perform Hamming Window Smoothing
    double hammingIncrement = (double)filterSize - 1;
    double fftReductionScalar = 1.0 / fftCalcSize;
    double* firArray = (double*)malloc(sizeof(double) * filterSize);
    for (int elementCounter = 0; elementCounter < filterSize; elementCounter++) {
        firArray[elementCounter] = (0.54 - 0.46 * cos(PI2 * (double)elementCounter / hammingIncrement)) * fftOutputSignal[elementCounter] * fftReductionScalar;
    }

    free(fftFrequencyData);
    free(fftOutputSignal);
    free(fftPlan);

    return firArray;
}
