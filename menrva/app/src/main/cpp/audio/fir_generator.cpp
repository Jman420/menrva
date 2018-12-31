/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2018 Justin Giannone (aka Jman420)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "fir_generator.h"
#include "../tools/android_logger.h"
#include "../tools/math_operations.h"

const std::string FIR_Generator::LOG_SENDER = "FIR_Generator";

FIR_Generator::FIR_Generator(LoggerBase* logger, FftInterfaceBase *fftEngine) {
    _Logger = logger;
    _FFTEngine = fftEngine;
}

AudioBuffer* FIR_Generator::Create(unsigned int filterSize, sample* frequencySamples, sample* amplitudeSamples, unsigned int sampleSize) {
    std::string logPrefix = LOG_SENDER + "Create()";

    // Validate Frequency Samples
    _Logger->WriteLog("Validating Frequency Samples...", logPrefix, LogLevel::DEBUG);
    _Logger->WriteLog("Sample Size : %d", logPrefix, LogLevel::VERBOSE, sampleSize);
    if (sampleSize < 2) {
        _Logger->WriteLog("Invalid Samples Provided : Minimum Sample Size is 2.", logPrefix, LogLevel::ERROR);
        return 0;
    }
    int lastSampleIndex = sampleSize - 1;
    for (int freqCounter = 0; freqCounter < lastSampleIndex; freqCounter++) {
        _Logger->WriteLog("Frequency Sample %d : %d", logPrefix, LogLevel::VERBOSE, freqCounter, frequencySamples[freqCounter]);
        if (frequencySamples[freqCounter] >= frequencySamples[freqCounter + 1]) {
            _Logger->WriteLog("Invalid Frequency Samples Provided : Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
            return 0;
        }
    }
    _Logger->WriteLog("Frequency Sample %d : %d", logPrefix, LogLevel::VERBOSE, lastSampleIndex, frequencySamples[lastSampleIndex]);
    if (frequencySamples[0] != 0 || frequencySamples[sampleSize - 1] != 1) {
        _Logger->WriteLog("Invalid Frequency Samples Provided : Frequency Samples must begin with 0 and end with 1 IE. { 0, 0.2, 0.5, 1 }", logPrefix, LogLevel::ERROR);
        return 0;
    }

    // Interpolate Amplitudes & Setup Fast Fourier Transform Frequencies
    _Logger->WriteLog("Interpolating Amplitudes & Setting up FFT Frequencies...", logPrefix, LogLevel::DEBUG);
    sample amplitudeIncrement = 0,
           interpolatedAmplitude = 0,
           fftRadianScalar = (filterSize - ONE) * ONE_HALF * PI,
           fftRadians = 0,
           interpolationSize = MathOperations::RoundToNextPowerOf2(filterSize) + 1,
           beginSegmentIndex = 0,
           endSegmentIndex = 0;
    unsigned int fftFrequencySize = (unsigned int)interpolationSize * 2;

    AudioComponentsBuffer fftFrequencies = *new AudioComponentsBuffer(_FFTEngine, fftFrequencySize);
    AudioBuffer fftFrequenciesReal = *fftFrequencies.GetRealBuffer(),
                fftFrequenciesImag = *fftFrequencies.GetImagBuffer(),
                fftOutputSignal = *new AudioBuffer(_FFTEngine, fftFrequencySize);

    for (int sampleCounter = 0; sampleCounter < sampleSize - 1; sampleCounter++) {
        endSegmentIndex = (int)(frequencySamples[sampleCounter + 1] * interpolationSize) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided : Amplitude change too great between indexes" + std::to_string(sampleCounter) + " and " + std::to_string(sampleCounter + 1);
            _Logger->WriteLog(msg, logPrefix, LogLevel::ERROR);
            return 0;
        }

        for (float elementCounter = beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            // Interpolate Amplitude
            amplitudeIncrement = (elementCounter - beginSegmentIndex) / (endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[sampleCounter + 1] + (ONE - amplitudeIncrement) * amplitudeSamples[sampleCounter];

            // Setup FFT Frequencies
            fftRadians = fftRadianScalar * elementCounter / (interpolationSize - ONE);
            float realFreqData = interpolatedAmplitude * cos(fftRadians),
                  imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));

            int elementIndex = (int)elementCounter;
            fftFrequenciesReal[elementIndex] = realFreqData;
            fftFrequenciesImag[elementIndex] = imaginaryFreqData * -ONE;

            int reverseElementCounter = fftFrequencySize - elementIndex - 1;
            fftFrequenciesReal[reverseElementCounter] = realFreqData;
            fftFrequenciesImag[reverseElementCounter] = imaginaryFreqData;
        }

        beginSegmentIndex = endSegmentIndex + 1;
    }

    // Perform Inverse FFT (turn frequencies into a signal)
    size_t fftCalcSize = fftFrequencySize - 2;
    _FFTEngine->Initialize(fftCalcSize, fftFrequencySize);
    _FFTEngine->ComponentsToSignal(&fftFrequencies, &fftOutputSignal);

    // Perform Hamming Window Smoothing
    sample hammingIncrement = (sample)filterSize - ONE,
           fftReductionScalar = ONE / fftCalcSize;
    AudioBuffer* firBufferPtr = new AudioBuffer(_FFTEngine, filterSize);
    sample* firBuffer = firBufferPtr->GetData();
    for (int elementCounter = 0; elementCounter < filterSize; elementCounter++) {
        firBuffer[elementCounter] = (HAMMING_054 - HAMMING_046 * cos(PI2 * (sample)elementCounter / hammingIncrement)) * fftOutputSignal[elementCounter] * fftReductionScalar;
    }

    delete &fftFrequencies;
    delete &fftOutputSignal;

    return firBufferPtr;
}
