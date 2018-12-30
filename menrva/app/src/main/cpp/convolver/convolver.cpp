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

Convolver::Convolver(FftInterfaceBase* fftEngine) {
    _FftEngine = fftEngine;

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
    for (int segmentCounter = 0; segmentCounter < _SegmentCount; segmentCounter++) {
        delete _ImpulseSegments[segmentCounter];
    }
    delete _ImpulseSegments;

    delete _WorkingSignal;
    delete _WorkingComponents;
    delete _OverlapSignal;

    _SignalScalar = 0;
    _SegmentCount = 0;
}

bool Convolver::Initialize(size_t audioInputSize, AudioBuffer* impulseResponse) {
    if (_Initialized) {
        Reset();
    }

    if (audioInputSize < 1) {
        return false;
    }

    size_t validImpulseResponseLength = FindImpulseResponseLength(*impulseResponse);
    if (validImpulseResponseLength < 1) {
        return true;
    }

    // Calculate Segment Count & Size
    size_t segmentSize = MathOperations::RoundToNextPowerOf2(audioInputSize),
           segmentComponentsSize = segmentSize + 1,
           segmentSignalSize = segmentSize * 2;
    _SegmentCount = (segmentSize + validImpulseResponseLength - 1) / segmentSize;
    size_t lastSegmentIndex = _SegmentCount - 1;
    _FftEngine->Initialize(segmentSignalSize, segmentComponentsSize);
    _SignalScalar = ONE_HALF / audioInputSize;

    // Allocate Impulse Response Filter Segments
    _ImpulseSegments = (AudioComponentsBuffer**)malloc(sizeof(AudioComponentsBuffer*) * _SegmentCount);

    // Allocate & Calculate Impulse Response Components for all except the Last Segment
    AudioBuffer* impulseSignalSegment = new AudioBuffer(_FftEngine, segmentSignalSize);
    for (int segmentCounter = 0; segmentCounter < _SegmentCount; segmentCounter++) {
        _ImpulseSegments[segmentCounter] = new AudioComponentsBuffer(_FftEngine, segmentComponentsSize);

        // Copy Current Segment of Impulse Response to Beginning Half of our Impulse Signal Segment, leaving last half as 0's
        size_t copySize = (segmentCounter != lastSegmentIndex) ? segmentSize : validImpulseResponseLength - (lastSegmentIndex * segmentSize);
        memcpy(impulseSignalSegment->GetData(), &impulseResponse[segmentCounter * segmentSize], sizeof(sample) * copySize);

        // Calculate & Store Impulse Segment's Components
        _FftEngine->SignalToComponents(impulseSignalSegment, _ImpulseSegments[segmentCounter]);
    }

    // Allocate Convolution Buffers
    _WorkingSignal = new AudioBuffer(_FftEngine, segmentSignalSize);
    _WorkingComponents = new AudioComponentsBuffer(_FftEngine, segmentComponentsSize);
    _OverlapSignal = new AudioBuffer(_FftEngine, audioInputSize);

    _Initialized = true;
}

void Convolver::Process(AudioBuffer* input, AudioBuffer* output) {
    // TODO : Implement Convolver Process Logic
}

size_t Convolver::FindImpulseResponseLength(AudioBuffer& impulseResponse) {
    size_t sampleCounter = impulseResponse.GetLength() - 1;
    while (sampleCounter > 0 && fabs(impulseResponse[sampleCounter]) < SIGNAL_THRESHOLD) {
        sampleCounter--;
    }

    return sampleCounter;
}
