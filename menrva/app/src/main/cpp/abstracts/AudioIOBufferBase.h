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

#ifndef MENRVA_AUDIO_IO_BUFFER_BASE_H
#define MENRVA_AUDIO_IO_BUFFER_BASE_H

#include <cstddef>
#include <cstdint>

class AudioIOBufferBase {
public:
    void SetData(uint32_t channelLength, size_t sampleLength);

    uint32_t GetChannelLength();
    size_t GetSampleLength();
    size_t GetBufferLength();

protected:
    uint32_t _ChannelLength;
    size_t _SampleLength;
    size_t _BufferLength;

    static size_t CalculateBufferIndex(uint32_t channelLength, uint32_t channelIndex, size_t sampleIndex);
};

#endif //MENRVA_AUDIO_IO_BUFFER_BASE_H
