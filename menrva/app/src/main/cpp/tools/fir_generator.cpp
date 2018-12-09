// Author : Jman420

#include "fir_generator.h"
#include "log_manager.h"

#include "../kissfft/kiss_fftr.h"

const std::string FIR_Generator::LOG_TAG = "Menrva-FIR_Generator - ";

double* FIR_Generator::Create(int interpolationSize, double* frequencySamples, double* amplitudeSamples, int sampleSize) {
    std::string logPrefix = LOG_TAG + "Create()";

    LogMngr::WriteLog("Validating Frequency Samples...", logPrefix);
    // Validate Frequency Samples
    if (frequencySamples[0] != 0 || frequencySamples[sampleSize - 1] != 1) {
        LogMngr::WriteLog("Invalid Frequency Samples Provided : Frequency Samples must begin with 0 and end with 1", logPrefix, LogLevel::ERROR);
        return 0;
    }
    for (int freqCounter = 0; freqCounter < sampleSize - 1; freqCounter++) {
        if (frequencySamples[freqCounter] >= frequencySamples[freqCounter + 1]) {
            LogMngr::WriteLog("Invalid Frequency Samples Provided : Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
            return 0;
        }
    }

    // Interpolate Amplitudes & Setup Fast Fourier Transform Plan
    double amplitudeIncrement = 0,
           fftRadianScalar = (double)(interpolationSize - 1) * 0.5 * M_PI,
           fftRadians = 0;
    int beginSegmentIndex = 0,
        endSegmentIndex = 0,
        fftFrequencySize = interpolationSize * 2;

    double* interpolatedAmplitudes = (double*)malloc(sizeof(double) * (interpolationSize * 2));
    interpolatedAmplitudes[0] = amplitudeSamples[0];

    kiss_fft_cpx* fftFrequencyData = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * interpolationSize);

    for (int amplitudeCounter = 0; amplitudeCounter < sampleSize - 1; amplitudeCounter++) {
        endSegmentIndex = (int)(frequencySamples[amplitudeCounter + 1] * interpolationSize) - 1;
        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided : Amplitude change too great between indexes" + std::to_string(amplitudeCounter) + " and " + std::to_string(amplitudeCounter + 1);
            LogMngr::WriteLog(msg, logPrefix, LogLevel::ERROR);
            return 0;
        }

        for (int elementCounter = beginSegmentIndex; elementCounter < endSegmentIndex; elementCounter++) {
            // Interpolate Amplitudes
            amplitudeIncrement = (double)(elementCounter - beginSegmentIndex) / (double)(endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitudes[elementCounter] = amplitudeIncrement * amplitudeSamples[amplitudeCounter + 1] + (1.0 - amplitudeIncrement) * amplitudeSamples[amplitudeCounter];

            // Setup FFT Frequencies
            fftRadians = fftRadianScalar * (double)elementCounter / (double)interpolationSize;
            fftFrequencyData[elementCounter].r = fftFrequencyData[fftFrequencySize - elementCounter].r = interpolatedAmplitudes[elementCounter] * cos(fftRadians);
            fftFrequencyData[elementCounter].i = interpolatedAmplitudes[elementCounter] * sin(fftRadians);
            fftFrequencyData[fftFrequencySize - elementCounter].i = (interpolatedAmplitudes[elementCounter] * sin(fftRadians)) * -1.0;
        }

        beginSegmentIndex = endSegmentIndex + 1;
    }

    // Perform FFT
    double* fftOutput = (double*)malloc(sizeof(double) * fftFrequencySize);
    kiss_fftr_cfg fftPlan = kiss_fftr_alloc(fftFrequencySize, 1, 0, 0);
    kiss_fftri(fftPlan, fftFrequencyData, fftOutput);

    // Perform Hamming Window Smoothing
    double hammingIncrement = (double)interpolationSize;
    double fftReductionScalar = 1.0 / interpolationSize;
    double* firArray = (double*)malloc(sizeof(double) * interpolationSize);
    for (int elementCounter = 0; elementCounter < interpolationSize; elementCounter++) {
        firArray[elementCounter] = (0.54 - 0.46 * cos(PI2 * (double)elementCounter / hammingIncrement)) * fftOutput[elementCounter] * fftReductionScalar;
    }

    free(interpolatedAmplitudes);
    free(fftFrequencyData);
    free(fftOutput);
    free(fftPlan);

    return firArray;
}
