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

#include <cmath>
#include "convolver.h"
#include "../tools/math_operations.h"

const float Convolver::SIGNAL_THRESHOLD = 0.000001f;

size_t Convolver::FindImpulseResponseLength(AudioBuffer& impulseResponse) {
    size_t sampleCounter = impulseResponse.GetLength() - 1;
    while (sampleCounter > 0 && abs(impulseResponse[sampleCounter]) < SIGNAL_THRESHOLD) {
        sampleCounter--;
    }

    return sampleCounter;
}

size_t Convolver::CalculateSegmentsCount(size_t segmentLength, size_t filterLength) {
    return (segmentLength + filterLength - 1) / segmentLength;
}

Convolver::Convolver(LoggerBase* logger, FftInterfaceBase* fftEngine, ConvolutionOperationsBase* convolutionOperations)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _FftEngine = fftEngine;
    _ConvolutionOperations = convolutionOperations;

    _Initialized = false;
    _FilterSegmentsLength = 0;
    _FrameLength = 0;
    _FrameSize = 0;
    _SignalScalar = 1;
}

Convolver::~Convolver() {
    Reset();

    delete _ConvolutionOperations;
}

void Convolver::Reset() {
    _Logger->WriteLog("Resetting Convolver Configuration...", LOG_SENDER, __func__);
    if (!_Initialized) {
        _Logger->WriteLog("Skipping resetting Convolver Configuration.  Convolver is Uninitialized.", LOG_SENDER, __func__);
        return;
    }

    _Initialized = false;
    for (int segmentCounter = 0; segmentCounter < _FilterSegmentsLength; segmentCounter++) {
        delete _FilterSegments[segmentCounter];
    }
    delete _FilterSegments;

    for (int mixCounter = 0; mixCounter < _MixedComponentsLength; mixCounter++) {
        delete _MixedComponents[mixCounter];
    }
    delete _MixedComponents;

    delete _WorkingSignal;
    delete _InputComponents;
    delete _OverlapSignal;

    _FrameLength = 0;
    _FrameSize = 0;
    _FilterSegmentsLength = 0;
    _MixedComponentsLength = 0;
    _MixCounter = 0;
    _OperationsPerConvolution = 0;
    _SignalScalar = 1;
    _Logger->WriteLog("Successfully reset Convolver Configuration!", LOG_SENDER, __func__);
}

bool Convolver::Initialize(size_t audioFrameLength, AudioBuffer* filterImpulseResponse, size_t autoConvolveFrames) {
    _Logger->WriteLog("Initializing Convolver Configuration...", LOG_SENDER, __func__);
    if (_Initialized) {
        Reset();
    }

    _Logger->WriteLog("Validating Audio Frame Length and Impulse Response...", LOG_SENDER, __func__);
    if (audioFrameLength < 1) {
        _Logger->WriteLog("Invalid Audio Frame Length provided.  Audio Frame Length must be greater than 0.", LOG_SENDER, __func__, LogLevel::WARN);
        return false;
    }

    size_t validFilterLength = FindImpulseResponseLength(*filterImpulseResponse);
    if (validFilterLength < 1) {
        _Logger->WriteLog("Invalid Impulse Response provided.  Length of non-zero Impulse Response Signal Values must be greater than 0.", LOG_SENDER, __func__, LogLevel::WARN);
        return true;
    }

    _Logger->WriteLog("Calculating Segment Count and Memory Size...", LOG_SENDER, __func__);
    _FrameLength = audioFrameLength;
    _FrameSize = sizeof(sample) * _FrameLength;
    size_t segmentLength = MathOperations::RoundToNextPowerOf2(_FrameLength),
           segmentComponentsLength = segmentLength + 1,
           segmentSignalLength = segmentLength * 2;
    _FilterSegmentsLength = CalculateSegmentsCount(segmentLength, validFilterLength);
    _FftEngine->Initialize(segmentSignalLength, segmentComponentsLength);
    _SignalScalar = ONE_HALF / _FrameLength;
    LogSegmentConfig();

    _Logger->WriteLog("Validating AutoConvolve Frames Length...", LOG_SENDER, __func__);
    if (_FilterSegmentsLength < autoConvolveFrames) {
        _Logger->WriteLog("Invalid AutoConvolve Frames Length provided.  AutoConvolve Frames must be less than or equal to the Filter Segments Length.", LOG_SENDER, __func__);
        return false;
    }

    _Logger->WriteLog("Allocating and Calculating Filter Segments and Components...", LOG_SENDER, __func__);
    _FilterSegments = static_cast<AudioComponentsBuffer**>(malloc(sizeof(AudioComponentsBuffer*) * _FilterSegmentsLength));
    size_t lastSegmentIndex = _FilterSegmentsLength - 1;
    auto* impulseSignalSegment = new AudioBuffer(_FftEngine, segmentSignalLength);
    for (int segmentCounter = 0; segmentCounter < _FilterSegmentsLength; segmentCounter++) {
        _Logger->WriteLog("Allocating Filter Segment for Segment Index (%d)...", LOG_SENDER, __func__, segmentCounter);
        _FilterSegments[segmentCounter] = new AudioComponentsBuffer(_FftEngine, segmentComponentsLength);

        // Copy Current Segment of Impulse Response to Beginning Half of our Impulse Signal Segment, leaving last half as 0's
        _Logger->WriteLog("Preparing Filter Segment Index (%d)...", LOG_SENDER, __func__, segmentCounter);
        size_t copySize = (segmentCounter != lastSegmentIndex) ? segmentLength : validFilterLength - (lastSegmentIndex * segmentLength);
        memcpy(impulseSignalSegment->GetData(), &(*filterImpulseResponse)[segmentCounter * segmentLength], sizeof(sample) * copySize);

        _Logger->WriteLog("Calculating Filter Components for Segment Index (%d)...", LOG_SENDER, __func__, segmentCounter);
        _FftEngine->SignalToComponents(impulseSignalSegment, _FilterSegments[segmentCounter]);
    }

    _Logger->WriteLog("Allocating AutoConvolution Buffers...", LOG_SENDER, __func__);
    _MixedComponentsLength = autoConvolveFrames + 1;
    _MixedComponents = static_cast<AudioComponentsBuffer**>(malloc(sizeof(AudioComponentsBuffer*) * _MixedComponentsLength));
    AudioComponentsBuffer* mixedBuffer;
    for (int bufferCounter = 0; bufferCounter < _MixedComponentsLength; bufferCounter++) {
        mixedBuffer = new AudioComponentsBuffer(_FftEngine, segmentComponentsLength);
        mixedBuffer->ResetData();
        _MixedComponents[bufferCounter] = mixedBuffer;
    }
    _MixCounter = 0;
    _OperationsPerConvolution = std::min(_FilterSegmentsLength, _MixedComponentsLength);

    _Logger->WriteLog("Allocating Convolution Buffers...", LOG_SENDER, __func__);
    _InputComponents = new AudioComponentsBuffer(_FftEngine, segmentComponentsLength);
    _WorkingSignal = new AudioBuffer(_FftEngine, segmentSignalLength);
    _OverlapSignal = new AudioBuffer(_FftEngine, _FrameLength);
    _OverlapSignal->ResetData();
    
    _Initialized = true;
    _Logger->WriteLog("Successfully Initialized Convolver Configuration...", LOG_SENDER, __func__);
    return true;
}

bool Convolver::Initialize(size_t audioFrameLength, AudioBuffer* filterImpulseResponse,
                           bool fullAutoConvolveFilter) {
    size_t autoConvolveFrames = 0;

    if (fullAutoConvolveFilter) {
        size_t validFilterLength = FindImpulseResponseLength(*filterImpulseResponse),
                segmentLength = MathOperations::RoundToNextPowerOf2(audioFrameLength);
        autoConvolveFrames = CalculateSegmentsCount(segmentLength, validFilterLength);
    }

    return Initialize(audioFrameLength, filterImpulseResponse, autoConvolveFrames);
}

bool Convolver::Initialize(size_t audioFrameLength, AudioBuffer* filterImpulseResponse) {
    return Initialize(audioFrameLength, filterImpulseResponse, false);
}

void Convolver::Process(AudioBuffer* input, AudioBuffer* output) {
    _Logger->WriteLog("Processing Audio Frame of length (%d)...", LOG_SENDER, __func__, input->GetLength());
    if (!_Initialized) {
        _Logger->WriteLog("Convolver not Initialized!  Skipping Processing Audio Frame.", LOG_SENDER, __func__, LogLevel::ERROR);
        return;
    }

    _Logger->WriteLog("Preparing Audio Frame...", LOG_SENDER, __func__);
    memcpy(_WorkingSignal->GetData(), input->GetData(), _FrameSize);
    memset(&_WorkingSignal->GetData()[_FrameLength], 0, _FrameSize);

    _Logger->WriteLog("Calculating Audio Frame's Components...", LOG_SENDER, __func__);
    _FftEngine->SignalToComponents(_WorkingSignal, _InputComponents);

    _Logger->WriteLog("MultiplyAccumulate Audio Frame's Components with Filter...", LOG_SENDER, __func__);
    for (size_t bufferCounter = 0; bufferCounter < _OperationsPerConvolution; bufferCounter++) {
        size_t mixBufferIndex = (bufferCounter + _MixCounter) % _MixedComponentsLength;

        AudioComponentsBuffer* filterSegment = _FilterSegments[bufferCounter];
        _ConvolutionOperations->ComplexMultiplyAccumulate(_InputComponents, filterSegment, _MixedComponents[mixBufferIndex]);
    }

    _Logger->WriteLog("Calculating Convolved Frame's Signal...", LOG_SENDER, __func__);
    _FftEngine->ComponentsToSignal(_MixedComponents[_MixCounter], _WorkingSignal);
    _MixedComponents[_MixCounter]->ResetData();
    _MixCounter = (_MixCounter + 1) % _MixedComponentsLength;

    _Logger->WriteLog("Summing Convolved Signal with Overlap Signal and Scaling...", LOG_SENDER, __func__);
    _ConvolutionOperations->SumAndScale(*_OverlapSignal, *_WorkingSignal, *output, _SignalScalar);

    _Logger->WriteLog("Storing Remaining Overlap Signal...", LOG_SENDER, __func__);
    memcpy(_OverlapSignal->GetData(), &(*_WorkingSignal)[_FrameLength], _FrameSize);
}

void Convolver::LogSegmentConfig() {
    _Logger->WriteLog("Frame Length (%d)", LOG_SENDER, __func__, LogLevel::VERBOSE, _FrameLength);
    _Logger->WriteLog("Frame Size (%d)", LOG_SENDER, __func__, LogLevel::VERBOSE, _FrameSize);
    _Logger->WriteLog("Filter Segments Length (%d)", LOG_SENDER, __func__, LogLevel::VERBOSE, _FilterSegmentsLength);
    _Logger->WriteLog("Signal Scalar (%f)", LOG_SENDER, __func__, LogLevel::VERBOSE, _SignalScalar);
}
