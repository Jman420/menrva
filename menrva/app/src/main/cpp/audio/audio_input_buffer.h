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

#ifndef MENRVA_AUDIOINPUTBUFFER_H
#define MENRVA_AUDIOINPUTBUFFER_H

#include <cstdint>
#include "audio_format.h"
#include "sample.h"
#include "../tools/conversion_buffer.h"
#include "../abstracts/logging_base.h"

union audio_input_buffer_u {
    ConversionBuffer<int16_t, sample>* PCM_16;
    ConversionBuffer<int32_t, sample>* PCM_32;
    ConversionBuffer<float, sample>* PCM_Float;
};

class AudioInputBuffer : public LoggingBase {
public:
    AudioInputBuffer(LoggerBase* logger);
    ~AudioInputBuffer();

    size_t GetLength();
    void ResetData();
    void Free();

    void SetData(AudioFormat audioFormat, void* data, size_t length);
    void* GetData();
    sample operator[](size_t index) const;  // Read-Only Subscript Operator

private:
    AudioFormat _AudioFormat;
    audio_input_buffer_u* _BufferWrapper;
};

#endif //MENRVA_AUDIOINPUTBUFFER_H
