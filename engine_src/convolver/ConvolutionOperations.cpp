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
#include <stdexcept>
#include "ConvolutionOperations.h"
#include "../tools/StringOperations.h"

ConvolutionOperations::ConvolutionOperations(LogWriterBase* logger)
        : LogProducer(logger, __PRETTY_FUNCTION__) {}

void ConvolutionOperations::ResetAndClone(AudioBuffer& source, AudioBuffer& destination) {
    _Logger->WriteLog("Resetting and Cloning Destination Audio Buffer...", LOG_SENDER, __func__);
    if (destination.GetLength() < source.GetLength()) {
        _Logger->WriteLog(StringOperations::FormatString("Unable to ResetConfig and Clone Audio Buffers.  Destination Buffer (%d) smaller than Source Buffer (%d).", destination.GetLength(), source.GetLength()),
                          LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error("Unable to ResetConfig and Clone Audio Buffers.  Destination Buffer smaller than Source Buffer.");
    }

    _Logger->WriteLog("Calculating Clone Length and Memory Size...", LOG_SENDER, __func__);
    size_t cloneLength = destination.GetLength();
    size_t sizeDiff = cloneLength - source.GetLength();
    size_t sampleTypeSize = sizeof(sample);
    sample* destinationData = destination.GetData();
    size_t cloneSize = sampleTypeSize * cloneLength;

    _Logger->WriteLog(StringOperations::FormatString("Cloning Source Buffer Length (%d) with memory size (%d)", cloneLength, cloneSize),
                      LOG_SENDER, __func__);
    memcpy(destinationData, source.GetData(), cloneSize);
    memset(destinationData + cloneSize, 0, sampleTypeSize * sizeDiff);
    _Logger->WriteLog("Successfully Clear and Cloned Source Buffer into Destination Buffer!", LOG_SENDER, __func__);
}

void ConvolutionOperations::SumAndScale(AudioBuffer& bufferA, AudioBuffer& bufferB, AudioBuffer& output, sample scalar) {
    _Logger->WriteLog(StringOperations::FormatString("Summing and Scaling Audio Buffers by (%f)...", scalar),
                      LOG_SENDER, __func__);
    if (output.GetLength() > bufferA.GetLength()) {
        _Logger->WriteLog(StringOperations::FormatString("Unable to Sum and Scale Audio Buffers.  Output Buffer (%d) larger than Source Buffer A (%d).", output.GetLength(), bufferA.GetLength()),
                          LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error("Unable to Sum and Scale Audio Buffers.  Output Buffer larger than Source Buffer A.");
    }
    if (bufferA.GetLength() > bufferB.GetLength()) {
        _Logger->WriteLog(StringOperations::FormatString("Unable to Sum and Scale Audio Buffers.  Source Buffer A (%d) larger than Source Buffer B (%d).", bufferA.GetLength(), bufferB.GetLength()),
                          LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error("Unable to Sum and Scale Audio Buffers.  Source Buffer A larger than Source Buffer B.");
    }

    for (int sampleCounter = 0; sampleCounter < output.GetLength(); sampleCounter++) {
        sample valueA = bufferA[sampleCounter];
        sample valueB = bufferB[sampleCounter];

        _Logger->WriteLog(StringOperations::FormatString("Summing and Scaling values (%f) and (%f)...", valueA, valueB),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);
        sample result = (valueA + valueB) * scalar;
        output[sampleCounter] = result;
        _Logger->WriteLog(StringOperations::FormatString("Successfully Summed and Scaled values into Result (%f).", result),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);
    }
    _Logger->WriteLog(StringOperations::FormatString("Successfully Summed and Scaled AudioBuffers by (%f)!", scalar), LOG_SENDER, __func__);
}

void ConvolutionOperations::ComplexMultiplyAccumulate(AudioComponentsBuffer& bufferA, AudioComponentsBuffer& bufferB, AudioComponentsBuffer& output) {
    _Logger->WriteLog("Multiplying and Accumulating Audio Component Buffers...", LOG_SENDER, __func__);
    if (output.GetLength() > bufferA.GetLength()) {
        std::string msg = StringOperations::FormatString("Unable to Multiply and Accumulate Audio Buffers.  Output Buffer (%d) larger than Source Buffer A (%d).", output.GetLength(), bufferA.GetLength());
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error(msg);
    }
    if (bufferA.GetLength() > bufferB.GetLength()) {
        std::string msg = StringOperations::FormatString("Unable to Multiply and Accumulate Audio Buffers.  Source Buffer A (%d) larger than Source Buffer B (%d).", bufferA.GetLength(), bufferB.GetLength());
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error(msg);
    }

    sample* bufferAReal = bufferA.GetRealBuffer()->GetData();
    sample* bufferAImaginary = bufferA.GetImaginaryBuffer()->GetData();
    sample* bufferBReal = bufferB.GetRealBuffer()->GetData();
    sample* bufferBImaginary = bufferB.GetImaginaryBuffer()->GetData();
    sample* outputReal = output.GetRealBuffer()->GetData();
    sample* outputImaginary = output.GetImaginaryBuffer()->GetData();

    for (int sampleCounter = 0; sampleCounter < output.GetLength(); sampleCounter++) {
        sample valueAReal = bufferAReal[sampleCounter];
        sample valueAImaginary = bufferAImaginary[sampleCounter];
        sample valueBReal = bufferBReal[sampleCounter];
        sample valueBImaginary = bufferBImaginary[sampleCounter];

        _Logger->WriteLog(StringOperations::FormatString("Multiplying Value A (real %f, imaginary %f) and Value B (real %f, imaginary %f)...", valueAReal, valueAImaginary, valueBReal, valueBImaginary),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);
        sample realResult = bufferAReal[sampleCounter] * bufferBReal[sampleCounter] - bufferAImaginary[sampleCounter] * bufferBImaginary[sampleCounter];
        sample imaginaryResult = bufferAReal[sampleCounter] * bufferBImaginary[sampleCounter] + bufferAImaginary[sampleCounter] * bufferBReal[sampleCounter];
        _Logger->WriteLog(StringOperations::FormatString("Successfully Multiplied Values into (real %f, imaginary %f).", realResult, imaginaryResult),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);

        _Logger->WriteLog(StringOperations::FormatString("Accumulating Values with Output Values (real %f, imaginary %f)...", outputReal[sampleCounter], outputImaginary[sampleCounter]),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);
        outputReal[sampleCounter] += realResult;
        outputImaginary[sampleCounter] += imaginaryResult;
        _Logger->WriteLog(StringOperations::FormatString("Successfully Accumulated Values into (real %f, imaginary %f)...", outputReal[sampleCounter], outputImaginary[sampleCounter]),
                          LOG_SENDER, __func__, LogLevel::VERBOSE);
    }
    _Logger->WriteLog("Successfully Multiplied and Accumulated Audio Component Buffers!", LOG_SENDER, __func__);
}
