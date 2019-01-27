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

#include <cstring>
#include <algorithm>
#include <cstdlib>
#include "convolution_operations.h"

ConvolutionOperations::ConvolutionOperations(LoggerBase* logger)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {}

void ConvolutionOperations::ResetAndClone(AudioBuffer* source, AudioBuffer* destination) {
    _Logger->WriteLog("Resetting and Cloning Destination Audio Buffer...", LOG_SENDER, __func__);
    assert(destination->GetLength() >= source->GetLength());

    _Logger->WriteLog("Calculating Clone Length and Memory Size...", LOG_SENDER, __func__);
    size_t cloneLength = destination->GetLength();
    size_t sizeDiff = cloneLength - source->GetLength();
    size_t sampleTypeSize = sizeof(sample);
    sample* destinationData = destination->GetData();
    size_t cloneSize = sampleTypeSize * cloneLength;

    _Logger->WriteLog("Cloning Source Buffer Length (%d) with memory size (%d)", LOG_SENDER, __func__, cloneLength, cloneSize);
    memcpy(destinationData, source->GetData(), cloneSize);
    memset(destinationData + cloneSize, 0, sampleTypeSize * sizeDiff);
    _Logger->WriteLog("Successfully Clear and Cloned Source Buffer into Destination Buffer!", LOG_SENDER, __func__);
}

void ConvolutionOperations::SumAndScale(AudioBuffer &bufferA, AudioBuffer &bufferB,
                                        AudioBuffer &output, sample scalar) {
    _Logger->WriteLog("Summing and Scaling Audio Buffers by (%f)...", LOG_SENDER, __func__, scalar);
    assert(output.GetLength() <= bufferA.GetLength());
    assert(bufferA.GetLength() <= bufferB.GetLength());

    for (int sampleCounter = 0; sampleCounter < output.GetLength(); sampleCounter++) {
        sample valueA = bufferA[sampleCounter],
               valueB = bufferB[sampleCounter];

        _Logger->WriteLog("Summing and Scaling values (%f) and (%f)...", LOG_SENDER, __func__, LogLevel::VERBOSE, valueA, valueB);
        sample result = (valueA + valueB) * scalar;
        output[sampleCounter] = result;
        _Logger->WriteLog("Successfully Summed and Scaled values into Result (%f).", LOG_SENDER, __func__, LogLevel::VERBOSE, result);
    }
    _Logger->WriteLog("Successfully Summed and Scaled AudioBuffers by (%f)!", LOG_SENDER, __func__, scalar);
}

void ConvolutionOperations::ComplexMultiplyAccumulate(AudioComponentsBuffer* bufferA,
                                                      AudioComponentsBuffer* bufferB,
                                                      AudioComponentsBuffer* output) {
    _Logger->WriteLog("Multiplying and Accumulating Audio Component Buffers...", LOG_SENDER, __func__);
    assert(output->GetLength() <= bufferA->GetLength());
    assert(bufferA->GetLength() <= bufferB->GetLength());

    sample* bufferAReal = bufferA->GetRealBuffer()->GetData();
    sample* bufferAImag = bufferA->GetImagBuffer()->GetData();
    sample* bufferBReal = bufferB->GetRealBuffer()->GetData();
    sample* bufferBImag = bufferB->GetImagBuffer()->GetData();
    sample* outputReal = output->GetRealBuffer()->GetData();
    sample* outputImag = output->GetImagBuffer()->GetData();

    for (int sampleCounter = 0; sampleCounter < output->GetLength(); sampleCounter++) {
        sample valueAReal = bufferAReal[sampleCounter],
               valueAImag = bufferAImag[sampleCounter],
               valueBReal = bufferBReal[sampleCounter],
               valueBImag = bufferBImag[sampleCounter];

        _Logger->WriteLog("Multiplying Value A (real %f, imaginary %f) and Value B (real %f, imaginary %f)...", LOG_SENDER, __func__, LogLevel::VERBOSE, valueAReal, valueAImag, valueBReal, valueBImag);
        sample realResult = bufferAReal[sampleCounter] * bufferBReal[sampleCounter] - bufferAImag[sampleCounter] * bufferBImag[sampleCounter],
               imaginaryResult = bufferAReal[sampleCounter] * bufferBImag[sampleCounter] + bufferAImag[sampleCounter] * bufferBReal[sampleCounter];
        _Logger->WriteLog("Successfully Multiplied Values into (real %f, imaginary %f).", LOG_SENDER, __func__, LogLevel::VERBOSE, realResult, imaginaryResult);

        _Logger->WriteLog("Accumulating Values with Output Values (real %f, imaginary %f)...", LOG_SENDER, __func__, LogLevel::VERBOSE, outputReal[sampleCounter], outputImag[sampleCounter]);
        outputReal[sampleCounter] += realResult;
        outputImag[sampleCounter] += imaginaryResult;
        _Logger->WriteLog("Successfully Accumulated Values into (real %f, imaginary %f)...", LOG_SENDER, __func__, LogLevel::VERBOSE, outputReal[sampleCounter], outputImag[sampleCounter]);
    }
    _Logger->WriteLog("Successfully Multiplied and Accumulated Audio Component Buffers!", LOG_SENDER, __func__);
}
