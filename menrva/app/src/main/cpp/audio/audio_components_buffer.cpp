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

#include "audio_components_buffer.h"

AudioComponentsBuffer::AudioComponentsBuffer() {
    _Length = 0;
    _RealBuffer = new AudioBuffer();
    _ImagBuffer = new AudioBuffer();
}

AudioComponentsBuffer::AudioComponentsBuffer(FftInterfaceBase* fftEngine, size_t length) {
    _Length = length;
    _RealBuffer = new AudioBuffer();
    _ImagBuffer = new AudioBuffer();

    if (length > 0) {
        CreateData(fftEngine, length);
    }
}

AudioComponentsBuffer::~AudioComponentsBuffer() {
    delete _RealBuffer;
    delete _ImagBuffer;
}

void AudioComponentsBuffer::ResetData() {
    _RealBuffer->ResetData();
    _ImagBuffer->ResetData();
}

void AudioComponentsBuffer::CreateData(FftInterfaceBase* fftEngine, size_t length) {
    _RealBuffer->CreateData(fftEngine, length);
    _ImagBuffer->CreateData(fftEngine, length);
    _Length = length;
}

size_t AudioComponentsBuffer::GetLength() {
    return _Length;
}

sample* AudioComponentsBuffer::GetRealData() {
    return _RealBuffer->GetData();
}

AudioBuffer* AudioComponentsBuffer::GetRealBuffer() {
    return _RealBuffer;
}

sample* AudioComponentsBuffer::GetImagData() {
    return _ImagBuffer->GetData();
}

AudioBuffer* AudioComponentsBuffer::GetImagBuffer() {
    return _ImagBuffer;
}

void AudioComponentsBuffer::SetRealValue(size_t index, sample value) {
    (*_RealBuffer)[index] = value;
}

void AudioComponentsBuffer::SetImagValue(size_t index, sample value) {
    (*_ImagBuffer)[index] = value;
}
