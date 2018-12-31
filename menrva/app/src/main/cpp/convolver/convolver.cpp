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
#include <cstdlib>
#include <cstring>
#include "convolver.h"
#include "../tools/math_operations.h"

const float Convolver::SIGNAL_THRESHOLD = 0.000001f;

Convolver::Convolver(FftInterfaceBase* fftEngine, ConvolutionOperationsBase* convolutionOperations) {
    _FftEngine = fftEngine;
    _ConvolutionOperations = convolutionOperations;

    Reset();
}

Convolver::~Convolver() {
    Reset();
}

void Convolver::Reset() {
    if (!_Initialized) {
        return;
    }

    _Initialized = false;
    for (int segmentCounter = 0; segmentCounter < _FilterSegmentsLength; segmentCounter++) {
        delete _FilterSegments[segmentCounter];
    }
    delete _FilterSegments;

    delete _WorkingSignal;
    delete _WorkingComponents;
    delete _OverlapSignal;

    _FilterSegmentsLength = 0;
    _FrameLength = 0;
    _FrameSize = 0;
    _SignalScalar = 1;
}

bool Convolver::Initialize(size_t frameLength, AudioBuffer* filterImpulseResponse) {
    if (_Initialized) {
        Reset();
    }

    if (frameLength < 1) {
        return false;
    }

    size_t validFilterLength = FindImpulseResponseLength(*filterImpulseResponse);
    if (validFilterLength < 1) {
        return true;
    }

    // Calculate Segment Count & Size
    _FrameLength = frameLength;
    _FrameSize = sizeof(sample) * _FrameLength;
    size_t segmentLength = MathOperations::RoundToNextPowerOf2(_FrameLength),
           segmentComponentsLength = segmentLength + 1,
           segmentSignalLength = segmentLength * 2;
    _FilterSegmentsLength = (segmentLength + validFilterLength - 1) / segmentLength;
    _FftEngine->Initialize(segmentSignalLength, segmentComponentsLength);
    _SignalScalar = ONE_HALF / _FrameLength;

    // Allocate Impulse Response Filter Segments
    _FilterSegments = (AudioComponentsBuffer**)malloc(sizeof(AudioComponentsBuffer*) * _FilterSegmentsLength);

    // Allocate & Calculate Impulse Response Components for all except the Last Segment
    size_t lastSegmentIndex = _FilterSegmentsLength - 1;
    AudioBuffer* impulseSignalSegment = new AudioBuffer(_FftEngine, segmentSignalLength);
    for (int segmentCounter = 0; segmentCounter < _FilterSegmentsLength; segmentCounter++) {
        _FilterSegments[segmentCounter] = new AudioComponentsBuffer(_FftEngine, segmentComponentsLength);

        // Copy Current Segment of Impulse Response to Beginning Half of our Impulse Signal Segment, leaving last half as 0's
        size_t copySize = (segmentCounter != lastSegmentIndex) ? segmentLength : validFilterLength - (lastSegmentIndex * segmentLength);
        memcpy(impulseSignalSegment->GetData(), &filterImpulseResponse[segmentCounter * segmentLength], sizeof(sample) * copySize);

        // Calculate & Store Impulse Segment's Components
        _FftEngine->SignalToComponents(impulseSignalSegment, _FilterSegments[segmentCounter]);
    }

    // Allocate Convolution Buffers
    _WorkingSignal = new AudioBuffer(_FftEngine, segmentSignalLength);
    _WorkingComponents = new AudioComponentsBuffer(_FftEngine, segmentComponentsLength);
    _OverlapSignal = new AudioBuffer(_FftEngine, _FrameLength);

    _Initialized = true;
    return true;
}

void Convolver::Process(AudioBuffer* input, AudioBuffer* output) {
    // TODO : Verify that the Input & Output Buffers will be of the correct length (_FrameLength)

    // Copy Input Frame into First Half & Zero out the Last Half of Working Signal
    memcpy(_WorkingSignal->GetData(), input->GetData(), _FrameSize);
    memset(&_WorkingSignal->GetData()[_FrameLength], 0, _FrameSize);

    // Calculate Input Frame's Components
    _FftEngine->SignalToComponents(_WorkingSignal, _WorkingComponents);

    // Multiply & Accumulate each of the Filter Segments
    for (int segmentCounter = 0; segmentCounter < _FilterSegmentsLength; segmentCounter++) {
        AudioComponentsBuffer* filterSegment = _FilterSegments[segmentCounter];
        _ConvolutionOperations->ComplexMultiplyAccumulate(_WorkingComponents, filterSegment, _WorkingComponents);
    }

    // Calculate Accumulated Signal
    _FftEngine->ComponentsToSignal(_WorkingComponents, _WorkingSignal);

    // Sum And Scale First Half of Accumulated Signal & Overlap into Output
    _ConvolutionOperations->SumAndScale(*_OverlapSignal, *_WorkingSignal, *output, _SignalScalar);

    // Store Last Half of Accumulated Signal in Overlap
    memcpy(_OverlapSignal->GetData(), &_WorkingSignal[_FrameLength], _FrameSize);
}

size_t Convolver::FindImpulseResponseLength(AudioBuffer& impulseResponse) {
    size_t sampleCounter = impulseResponse.GetLength() - 1;
    while (sampleCounter > 0 && fabs(impulseResponse[sampleCounter]) < SIGNAL_THRESHOLD) {
        sampleCounter--;
    }

    return sampleCounter;
}
