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

#ifndef MENRVA_AUDIO_OUTPUT_BUFFER_H
#define MENRVA_AUDIO_OUTPUT_BUFFER_H

#include <cstddef>
#include <cstdint>
#include "sample.h"
#include "audio_format.h"
#include "../tools/conversion_buffer.h"
#include "../abstracts/logging_base.h"
#include "../abstracts/audio_io_buffer_base.h"

union audio_output_buffer_u {
    Buffer<int16_t>* PCM_16;
    Buffer<int32_t>* PCM_32;
    Buffer<float>* PCM_Float;
};

class AudioOutputBuffer : public LoggingBase,
                          public AudioIOBufferBase {
public:
    explicit AudioOutputBuffer(LoggerBase* logger);
    AudioOutputBuffer(LoggerBase* logger, AudioFormat audioFormat);
    ~AudioOutputBuffer();

    void ResetData();
    void Free();

    void SetFormat(AudioFormat audioFormat);
    void SetData(void* data, uint32_t channelLength, size_t sampleLength);
    void SetData(AudioFormat audioFormat, void* data, uint32_t channelLength, size_t sampleLength);
    void SetValue(uint32_t channelIndex, size_t sampleIndex, sample value);
    void* GetData();
    void* operator()(uint32_t channelIndex, size_t sampleIndex) const;  // Read-Only Subscript Operator

private:
    AudioFormat _AudioFormat;
    audio_output_buffer_u* _BufferWrapper;
    uint32_t _ChannelLength;
    size_t _SampleLength;

    template<class TOutputType>
    TOutputType Normalize(sample data);
};

#endif //MENRVA_AUDIO_OUTPUT_BUFFER_H
