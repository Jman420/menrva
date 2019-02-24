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
#include "../tools/math_operations.h"
#include "wave_generator_constants.h"

FirGenerator::FirGenerator(LoggerBase* logger, FftInterfaceBase *fftEngine)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _FftEngine = fftEngine;
}

FirGenerator::~FirGenerator() {
    delete _FftEngine;
}

AudioBuffer* FirGenerator::Calculate(size_t filterLength, sample* frequencySamples, sample* amplitudeSamples, size_t sampleLength) {
    _Logger->WriteLog("Validating Frequency Samples...", LOG_SENDER, __func__);
    if (sampleLength < 2) {
        _Logger->WriteLog("Invalid Samples Provided (%d).  Minimum Sample Size is 2.", LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error("Invalid Samples Provided.  Minimum Sample Size is 2.");
    }
    size_t lastSampleIndex = sampleLength - 1;
    if (frequencySamples[0] != 0 || frequencySamples[lastSampleIndex] != 1) {
        std::string msg = "Invalid Frequency Samples Provided.  Frequency Samples must begin with 0 and end with 1 IE. { 0, 0.2, 0.5, 1 }";
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error(msg);
    }
    for (int sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        sample currentFreq = frequencySamples[sampleCounter],
               nextFreq = frequencySamples[sampleCounter + 1];

        _Logger->WriteLog("Frequency Index (%d) has value (%d)", LOG_SENDER, __func__, LogLevel::VERBOSE, sampleCounter, currentFreq);
        if (currentFreq >= nextFreq) {
            _Logger->WriteLog("Invalid Frequency Samples Provided.  Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }.  Next Frequency Value (%d) vs Current Frequency Value (%d)", LOG_SENDER, __func__, LogLevel::FATAL, nextFreq, currentFreq);
            throw std::runtime_error("Invalid Frequency Samples Provided.  Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }.");
        }
    }
    _Logger->WriteLog("Frequency Index (%d) has value (%d)", LOG_SENDER, __func__, LogLevel::VERBOSE, lastSampleIndex, frequencySamples[lastSampleIndex]);

    _Logger->WriteLog("Initializing Interpolation & FFT Variables...", LOG_SENDER, __func__);
    sample amplitudeIncrement = 0,
           interpolatedAmplitude = 0,
           fftRadianScalar = (filterLength - WaveGeneratorConstants::ONE) * WaveGeneratorConstants::ONE_HALF * WaveGeneratorConstants::PI,
           fftRadians = 0,
           interpolationLength = MathOperations::RoundToNextPowerOf2(filterLength) + 1,
           beginSegmentIndex = 0,
           endSegmentIndex = 0;
    size_t fftFrequencyLength = (size_t)interpolationLength * 2;

    AudioComponentsBuffer fftFrequencies(_FftEngine, fftFrequencyLength);
    AudioBuffer fftOutputSignal(_FftEngine, fftFrequencyLength);

    _Logger->WriteLog("Interpolating Frequencies & Amplitudes for Inverse FFT Processing...", LOG_SENDER, __func__);
    for (size_t sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        size_t nextSampleIndex = sampleCounter + 1;
        _Logger->WriteLog("Validating Frequency Change for Indexes (%d) and (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, sampleCounter, nextSampleIndex);
        endSegmentIndex = (int)(frequencySamples[nextSampleIndex] * interpolationLength) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationLength) {
            _Logger->WriteLog("Invalid Amplitudes Provided.  Amplitude change too great between indexes (%d) and (%d).", LOG_SENDER, __func__, LogLevel::FATAL, sampleCounter, nextSampleIndex);
            throw std::runtime_error("Invalid Amplitudes Provided.  Amplitude change too great.");
        }

        _Logger->WriteLog("Calculating Frequency Components for Element Indexes (%g) to (%g)...", LOG_SENDER, __func__, LogLevel::VERBOSE, beginSegmentIndex, endSegmentIndex);
        for (auto elementCounter = (size_t)beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            auto elementIndex = (sample)elementCounter;
            amplitudeIncrement = (elementIndex - beginSegmentIndex) / (endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[sampleCounter + 1] + (WaveGeneratorConstants::ONE - amplitudeIncrement) * amplitudeSamples[sampleCounter];
            _Logger->WriteLog("Interpolated Amplitude for Element Index (%d) is (%f).", LOG_SENDER, __func__, LogLevel::VERBOSE, elementIndex, interpolatedAmplitude);

            fftRadians = fftRadianScalar * elementIndex / (interpolationLength - WaveGeneratorConstants::ONE);
            sample realFreqData = interpolatedAmplitude * cos(fftRadians),
                   imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));
            _Logger->WriteLog("Real Frequency Value for Element Index (%d) is (%f)", LOG_SENDER, __func__, LogLevel::VERBOSE, elementIndex, realFreqData);
            _Logger->WriteLog("Imaginary Frequency Value for Element Index (%d) is (%f)", LOG_SENDER, __func__, LogLevel::VERBOSE, elementIndex, imaginaryFreqData);

            fftFrequencies.SetRealValue(elementCounter, realFreqData);
            fftFrequencies.SetImagValue(elementCounter, imaginaryFreqData * -WaveGeneratorConstants::ONE);

            size_t reverseElementCounter = fftFrequencyLength - elementCounter - 1;
            fftFrequencies.SetRealValue(reverseElementCounter, realFreqData);
            fftFrequencies.SetImagValue(reverseElementCounter, imaginaryFreqData);
            _Logger->WriteLog("Successfully set Real & Imaginary Values for Element Indexes (%d) and (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, elementIndex, reverseElementCounter);
        }

        _Logger->WriteLog("Successfully calculated Frequency Components for Element Indexes (%g) to (%g)!", LOG_SENDER, __func__, LogLevel::VERBOSE, beginSegmentIndex, endSegmentIndex);
        beginSegmentIndex = endSegmentIndex + 1;
    }

    _Logger->WriteLog("Calculating FIR Signal from Interpolated Frequency Components...", LOG_SENDER, __func__);
    size_t fftCalcSize = fftFrequencyLength - 2;
    _FftEngine->Initialize(fftCalcSize, fftFrequencyLength);
    _FftEngine->ComponentsToSignal(fftFrequencies, fftOutputSignal);

    _Logger->WriteLog("Performing Hamming Window Smoothing on FIR Signal...", LOG_SENDER, __func__);
    sample hammingIncrement = (sample)filterLength - WaveGeneratorConstants::ONE,
           fftReductionScalar = WaveGeneratorConstants::ONE / fftCalcSize;
    auto firBufferPtr = new AudioBuffer(_FftEngine, filterLength);
    sample* firBuffer = firBufferPtr->GetData();
    for (int elementCounter = 0; elementCounter < filterLength; elementCounter++) {
        sample originalValue = fftOutputSignal[elementCounter],
               smoothedValue = (WaveGeneratorConstants::HAMMING_054 - WaveGeneratorConstants::HAMMING_046 * cos(WaveGeneratorConstants::PI2 * (sample)elementCounter / hammingIncrement)) * originalValue * fftReductionScalar;
        firBuffer[elementCounter] = smoothedValue;
        _Logger->WriteLog("Smoothed FIR Signal Value at Index (%d) from (%f) to (%f).", LOG_SENDER, __func__, LogLevel::VERBOSE, elementCounter, originalValue, smoothedValue);
    }

    _Logger->WriteLog("Successfully calculated FIR Signal!", LOG_SENDER, __func__);
    return firBufferPtr;
}
