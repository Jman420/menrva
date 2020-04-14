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

#include <stdexcept>
#include "FirGenerator.h"
#include "WaveGeneratorConstants.h"
#include "../audio/AudioComponentsBuffer.h"
#include "../tools/MathOperations.h"
#include "../tools/StringOperations.h"

FirGenerator::FirGenerator(ILogWriter* logger, FftInterfaceBase *fftEngine)
        : LogProducer(logger, __PRETTY_FUNCTION__) {
    _FftEngine = fftEngine;
}

FirGenerator::~FirGenerator() {
    delete _FftEngine;
}

AudioBuffer* FirGenerator::Calculate(size_t filterLength, sample* frequencySamples, sample* amplitudeSamples, size_t sampleLength) {
    _Logger->WriteLog("Validating Frequency Samples...", LOG_SENDER, __func__);
    if (sampleLength < 2) {
        std::string msg = StringOperations::FormatString("Invalid Samples Provided (%d).  Minimum Sample Size is 2.", sampleLength);
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
        throw std::runtime_error(msg);
    }
    size_t lastSampleIndex = sampleLength - 1;
    if (frequencySamples[0] != 0 || frequencySamples[lastSampleIndex] != 1) {
        std::string msg = "Invalid Frequency Samples Provided.  Frequency Samples must begin with 0 and end with 1 IE. { 0, 0.2, 0.5, 1 }";
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
        throw std::runtime_error(msg);
    }
    for (int sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        sample currentFreq = frequencySamples[sampleCounter];
        sample nextFreq = frequencySamples[sampleCounter + 1];

        _Logger->WriteLog(StringOperations::FormatString("Frequency Index (%d) has value (%d)", sampleCounter, currentFreq),
                          LOG_SENDER, __func__, LogLevel::Verbose);
        if (currentFreq >= nextFreq) {
            std::string msg = StringOperations::FormatString("Invalid Frequency Samples Provided.  Frequency Samples much increase over each element IE. { 0, 0.2, 0.5, 1 }.  Next Frequency Value (%d) vs Current Frequency Value (%d)", nextFreq, currentFreq);
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
        }
    }
    _Logger->WriteLog(StringOperations::FormatString("Frequency Index (%d) has value (%d)", lastSampleIndex, frequencySamples[lastSampleIndex]),
                      LOG_SENDER, __func__, LogLevel::Verbose);

    _Logger->WriteLog("Initializing Interpolation & FFT Variables...", LOG_SENDER, __func__);
    sample amplitudeIncrement = 0;
    sample interpolatedAmplitude = 0;
    sample fftRadianScalar = (filterLength - WaveGeneratorConstants::ONE) * WaveGeneratorConstants::ONE_HALF * WaveGeneratorConstants::PI;
    sample fftRadians = 0;
    sample interpolationLength = MathOperations::RoundToNextPowerOf2(filterLength) + 1;
    sample beginSegmentIndex = 0;
    sample endSegmentIndex = 0;
    size_t fftFrequencyLength = (size_t)interpolationLength * 2;

    AudioComponentsBuffer fftFrequencies(_FftEngine, fftFrequencyLength);
    AudioBuffer fftOutputSignal(_FftEngine, fftFrequencyLength);

    _Logger->WriteLog("Interpolating Frequencies & Amplitudes for Inverse FFT Processing...", LOG_SENDER, __func__);
    for (size_t sampleCounter = 0; sampleCounter < lastSampleIndex; sampleCounter++) {
        size_t nextSampleIndex = sampleCounter + 1;
        _Logger->WriteLog(StringOperations::FormatString("Validating Frequency Change for Indexes (%d) and (%d)...", sampleCounter, nextSampleIndex),
                          LOG_SENDER, __func__, LogLevel::Verbose);
        endSegmentIndex = (int)(frequencySamples[nextSampleIndex] * interpolationLength) - 1;

        if (beginSegmentIndex < 0 || endSegmentIndex > interpolationLength) {
            std::string msg = StringOperations::FormatString("Invalid Amplitudes Provided.  Amplitude change too great between indexes (%d) and (%d).", sampleCounter, nextSampleIndex);
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
        }

        _Logger->WriteLog(StringOperations::FormatString("Calculating Frequency Components for Element Indexes (%g) to (%g)...", beginSegmentIndex, endSegmentIndex),
                          LOG_SENDER, __func__, LogLevel::Verbose);
        for (auto elementCounter = (size_t)beginSegmentIndex; elementCounter <= endSegmentIndex; elementCounter++) {
            auto elementIndex = (sample)elementCounter;
            amplitudeIncrement = (elementIndex - beginSegmentIndex) / (endSegmentIndex - beginSegmentIndex);
            interpolatedAmplitude = amplitudeIncrement * amplitudeSamples[sampleCounter + 1] + (WaveGeneratorConstants::ONE - amplitudeIncrement) * amplitudeSamples[sampleCounter];
            _Logger->WriteLog(StringOperations::FormatString("Interpolated Amplitude for Element Index (%d) is (%f).", elementIndex, interpolatedAmplitude),
                              LOG_SENDER, __func__, LogLevel::Verbose);

            fftRadians = fftRadianScalar * elementIndex / (interpolationLength - WaveGeneratorConstants::ONE);
            sample realFreqData = interpolatedAmplitude * cos(fftRadians);
            sample imaginaryFreqData = (interpolatedAmplitude * sin(fftRadians));
            _Logger->WriteLog(StringOperations::FormatString("Real Frequency Value for Element Index (%d) is (%f)", elementIndex, realFreqData),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            _Logger->WriteLog(StringOperations::FormatString("Imaginary Frequency Value for Element Index (%d) is (%f)", elementIndex, imaginaryFreqData),
                              LOG_SENDER, __func__, LogLevel::Verbose);

            fftFrequencies.SetRealValue(elementCounter, realFreqData);
            fftFrequencies.SetImaginaryValue(elementCounter, imaginaryFreqData * -WaveGeneratorConstants::ONE);

            size_t reverseElementCounter = fftFrequencyLength - elementCounter - 1;
            fftFrequencies.SetRealValue(reverseElementCounter, realFreqData);
            fftFrequencies.SetImaginaryValue(reverseElementCounter, imaginaryFreqData);
            _Logger->WriteLog(StringOperations::FormatString("Successfully set Real & Imaginary Values for Element Indexes (%d) and (%d)!", elementIndex, reverseElementCounter),
                              LOG_SENDER, __func__, LogLevel::Verbose);
        }

        _Logger->WriteLog(StringOperations::FormatString("Successfully calculated Frequency Components for Element Indexes (%g) to (%g)!", beginSegmentIndex, endSegmentIndex),
                          LOG_SENDER, __func__, LogLevel::Verbose);
        beginSegmentIndex = endSegmentIndex + 1;
    }

    _Logger->WriteLog("Calculating FIR Signal from Interpolated Frequency Components...", LOG_SENDER, __func__);
    size_t fftCalcSize = fftFrequencyLength - 2;
    _FftEngine->Initialize(fftCalcSize, fftFrequencyLength);
    _FftEngine->ComponentsToSignal(fftFrequencies, fftOutputSignal);

    _Logger->WriteLog("Performing Hamming Window Smoothing on FIR Signal...", LOG_SENDER, __func__);
    sample hammingIncrement = (sample) filterLength - WaveGeneratorConstants::ONE;
    sample fftReductionScalar = WaveGeneratorConstants::ONE / fftCalcSize;
    auto firBufferPtr = new AudioBuffer(_FftEngine, filterLength);
    sample* firBuffer = firBufferPtr->GetData();
    for (int elementCounter = 0; elementCounter < filterLength; elementCounter++) {
        sample originalValue = fftOutputSignal[elementCounter];
        sample smoothedValue = (WaveGeneratorConstants::HAMMING_054 - WaveGeneratorConstants::HAMMING_046 * cos(WaveGeneratorConstants::PI2 * (sample) elementCounter / hammingIncrement)) * originalValue * fftReductionScalar;
        firBuffer[elementCounter] = smoothedValue;
        _Logger->WriteLog(StringOperations::FormatString("Smoothed FIR Signal Value at Index (%d) from (%f) to (%f).", elementCounter, originalValue, smoothedValue),
                          LOG_SENDER, __func__, LogLevel::Verbose);
    }

    _Logger->WriteLog("Successfully calculated FIR Signal!", LOG_SENDER, __func__);
    return firBufferPtr;
}
