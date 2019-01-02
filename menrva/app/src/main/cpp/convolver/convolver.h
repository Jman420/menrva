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

#ifndef MENRVA_CONVOLVER_H
#define MENRVA_CONVOLVER_H

#include <cstddef>
#include "../abstracts/fft_interface_base.h"
#include "../audio/audio_buffer.h"
#include "convolution_operations.h"

class Convolver : public LoggingBase {
public:
    Convolver(LoggerBase* logger, FftInterfaceBase* fftEngine, ConvolutionOperationsBase* convolutionOperations);
    ~Convolver();

    void Reset();
    bool Initialize(size_t audioFrameLength, AudioBuffer* filterImpulseResponse);
    void Process(AudioBuffer* input, AudioBuffer* output);

private:
    static constexpr sample ONE_HALF = (sample)0.5;
    static const float SIGNAL_THRESHOLD;

    static size_t FindImpulseResponseLength(AudioBuffer& impulseResponse);

    ConvolutionOperationsBase* _ConvolutionOperations;
    FftInterfaceBase* _FftEngine;

    bool _Initialized;
    size_t _FilterSegmentsLength,
           _FrameLength,
           _FrameSize;
    sample _SignalScalar;
    AudioComponentsBuffer** _FilterSegments;
    AudioBuffer* _WorkingSignal;
    AudioBuffer* _OverlapSignal;
    AudioComponentsBuffer* _WorkingComponents;

    void LogSegmentConfig();
    void LogAudioComponents(AudioComponentsBuffer* audioComponents);
    void LogSignal(AudioBuffer* signal);
};

#endif //MENRVA_CONVOLVER_H
