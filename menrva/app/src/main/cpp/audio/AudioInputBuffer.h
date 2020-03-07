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

#ifndef MENRVA_AUDIO_INPUT_BUFFER_H
#define MENRVA_AUDIO_INPUT_BUFFER_H

#include <cstdint>
#include "Sample.h"
#include "AudioFormat.h"
#include "AudioIOBufferBase.h"
#include "../tools/ConversionBuffer.h"
#include "../log/LoggingBase.h"

union audio_input_buffer_u {
    ConversionBuffer<int16_t, sample>* PCM_16;
    ConversionBuffer<int32_t, sample>* PCM_32;
    ConversionBuffer<float, sample>* PCM_Float;
};

class AudioInputBuffer
        : public LoggingBase,
          public AudioIOBufferBase {
public:
    explicit AudioInputBuffer(LoggerBase* logger);
    AudioInputBuffer(LoggerBase* logger, AudioFormat audioFormat);
    ~AudioInputBuffer();

    void ResetData();
    void Free();

    void SetFormat(AudioFormat inputAudioFormat);
    void SetData(void* data, uint32_t channelLength, size_t sampleLength);
    void SetData(AudioFormat inputAudioFormat, void* data, uint32_t channelLength, size_t sampleLength);
    void* GetData();
    sample operator()(uint32_t channelIndex, size_t sampleIndex) const;  // Read-Only Subscript Operator

private:
    AudioFormat _InputAudioFormat;
    audio_input_buffer_u* _BufferWrapper;

    template<class TInputType>
    sample Normalize(TInputType data) const;
};

#endif //MENRVA_AUDIO_INPUT_BUFFER_H
