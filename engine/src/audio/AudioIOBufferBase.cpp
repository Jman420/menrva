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

#include "AudioIOBufferBase.h"

uint32_t AudioIOBufferBase::GetChannelLength() {
    return _ChannelLength;
}

size_t AudioIOBufferBase::GetSampleLength() {
    return _SampleLength;
}

size_t AudioIOBufferBase::GetBufferLength() {
    return _SampleLength * _ChannelLength;
}

void AudioIOBufferBase::SetLength(uint32_t channelLength, size_t sampleLength) {
    _ChannelLength = channelLength;
    _SampleLength = sampleLength;
}

size_t AudioIOBufferBase::CalculateBufferIndex(uint32_t channelLength, uint32_t channelIndex, size_t sampleIndex) {
    return (sampleIndex * channelLength) + channelIndex;
}
