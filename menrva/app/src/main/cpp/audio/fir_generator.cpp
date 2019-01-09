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

FIR_Generator::FIR_Generator(LoggerBase* logger, FftInterfaceBase *fftEngine)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _FftEngine = fftEngine;
}

AudioBuffer* FIR_Generator::Calculate(unsigned int filterSize, sample* frequencySamples,
                                      sample* amplitudeSamples, unsigned int sampleSize) {
    _Logger->WriteLog("Validating Frequency Samples...", LOG_SENDER, __func__);
    if (sampleSize < 2) {
        _Logger->WriteLog("Invalid Samples Provided (%d).  Minimum Sample Size is 2.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }
    int lastSampleIndex = sampleSize - 1;
    if (frequencySamples[0] != 0 || frequencySamples[lastSampleIndex] != 1) {
        _Logger->WriteLog("Invalid Frequency Samples Provided.  Frequency Samples must begin with 0 and end with 1 IE. { 0, 0.2, 0.5, 1 }", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }
    for (int sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        sample currentFreq = frequencySamples[sampleCounter],
               nextFreq = frequencySamples[sampleCounter + 1];

        _Logger->WriteLog("Frequency Index (%d) has value (%d)", LOG_SENDER, __func__, sampleCounter, currentFreq);
        if (currentFreq >= nextFreq) {
            _Logger->WriteLog("Invalid Frequency Samples Provided.  Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }.  Next Frequency Value (%d) vs Current Frequency Value (%d)", LOG_SENDER, __func__, LogLevel::WARN, nextFreq, currentFreq);
            return 0;
        }
    }
    _Logger->WriteLog("Frequency Index (%d) has value (%d)", LOG_SENDER, __func__, lastSampleIndex, frequencySamples[lastSampleIndex]);

    _Logger->WriteLog("Initializing Interpolation & FFT Variables...", LOG_SENDER, __func__);
    sample amplitudeIncrement = 0,
           interpolatedAmplitude = 0,
           fftRadianScalar = (filterSize - ONE) * ONE_HALF * PI,
           fftRadians = 0,
           interpolationSize = MathOperations::RoundToNextPowerOf2(filterSize) + 1,
           beginSegmentIndex = 0,
           endSegmentIndex = 0;
    unsigned int fftFrequencySize = (unsigned int)interpolationSize * 2;

    AudioComponentsBuffer fftFrequencies = *new AudioComponentsBuffer(_Logger, _FftEngine, fftFrequencySize);
    AudioBuffer fftFrequenciesReal = *fftFrequencies.GetRealBuffer(),
                fftFrequenciesImag = *fftFrequencies.GetImagBuffer(),
                fftOutputSignal = *new AudioBuffer(_FftEngine);

    _Logger->WriteLog("Interpolating Frequencies & Amplitudes for Inverse FFT Processing...", LOG_SENDER, __func__);
    for (int sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        _Logger->WriteLog("Validating Frequency Change...", LOG_SENDER, __func__);
        int nextSampleIndex = sampleCounter + 1;
        endSegmentIndex = (int)(frequencySamples[nextSampleIndex] * interpolationSize) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationSize) {
            std::string msg = "Invalid Amplitudes Provided.  Amplitude change too great between indexes (%d) and (%d).";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::WARN, sampleCounter, nextSampleIndex);
            return 0;
        }

        _Logger->WriteLog("Calculating Frequency Components for Element Indexes (%d) to (%d)...", LOG_SENDER, __func__, beginSegmentIndex, endSegmentIndex);
        for (float elementCounter = beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            int elementIndex = (int)elementCounter;
            amplitudeIncrement = (elementCounter - beginSegmentIndex) / (endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[sampleCounter + 1] + (ONE - amplitudeIncrement) * amplitudeSamples[sampleCounter];
            _Logger->WriteLog("Interpolated Amplitude for Element Index (%d) is (%f).", LOG_SENDER, __func__, elementIndex, interpolatedAmplitude);

            fftRadians = fftRadianScalar * elementCounter / (interpolationSize - ONE);
            float realFreqData = interpolatedAmplitude * cos(fftRadians),
                  imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));
            _Logger->WriteLog("Real Frequency Value for Element Index (%d) is (%f)", LOG_SENDER, __func__, elementIndex, realFreqData);
            _Logger->WriteLog("Imaginary Frequency Value for Element Index (%d) is (%f)", LOG_SENDER, __func__, elementIndex, imaginaryFreqData);

            fftFrequenciesReal[elementIndex] = realFreqData;
            fftFrequenciesImag[elementIndex] = imaginaryFreqData * -ONE;

            int reverseElementIndex = fftFrequencySize - elementIndex - 1;
            fftFrequenciesReal[reverseElementIndex] = realFreqData;
            fftFrequenciesImag[reverseElementIndex] = imaginaryFreqData;
            _Logger->WriteLog("Successfully set Real & Imaginary Values for Element Indexes (%d) and (%d)!", LOG_SENDER, __func__, elementIndex, reverseElementIndex);
        }

        _Logger->WriteLog("Successfully calculated Frequency Components for Element Indexes (%d) to (%d)!", LOG_SENDER, __func__, beginSegmentIndex, endSegmentIndex);
        beginSegmentIndex = endSegmentIndex + 1;
    }

    _Logger->WriteLog("Calculating FIR Signal from Interpolated Frequency Components...", LOG_SENDER, __func__);
    size_t fftCalcSize = fftFrequencySize - 2;
    _FftEngine->Initialize(fftCalcSize, fftFrequencySize);
    _FftEngine->ComponentsToSignal(&fftFrequencies, &fftOutputSignal);

    _Logger->WriteLog("Performing Hamming Window Smoothing on FIR Signal...", LOG_SENDER, __func__);
    sample hammingIncrement = (sample)filterSize - ONE,
           fftReductionScalar = ONE / fftCalcSize;
    AudioBuffer* firBufferPtr = new AudioBuffer(_FftEngine, filterSize);
    sample* firBuffer = firBufferPtr->GetData();
    for (int elementCounter = 0; elementCounter < filterSize; elementCounter++) {
        sample originalValue = fftOutputSignal[elementCounter],
               smoothedValue = (HAMMING_054 - HAMMING_046 * cos(PI2 * (sample)elementCounter / hammingIncrement)) * originalValue * fftReductionScalar;
        firBuffer[elementCounter] = smoothedValue;
        _Logger->WriteLog("Smoothed FIR Signal Value at Index (%d) from (%f) to (%f).", LOG_SENDER, __func__, elementCounter, originalValue, smoothedValue);
    }
    _Logger->WriteLog("Successfully applied Hamming Window Smoothing to FIR Signal!", LOG_SENDER, __func__);

    delete &fftFrequencies;
    delete &fftOutputSignal;

    _Logger->WriteLog("Successfully calculated FIR Signal!", LOG_SENDER, __func__);
    return firBufferPtr;
}
