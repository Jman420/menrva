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

#include <cstdlib>
#include "audio_components_buffer.h"

AudioComponentsBuffer::AudioComponentsBuffer(FFTInterfaceBase* fftEngine, size_t size) {
    _Size = size;
    _RealBuffer = new AudioBuffer(fftEngine, size);
    _ImagBuffer = new AudioBuffer(fftEngine, size);
}

AudioComponentsBuffer::~AudioComponentsBuffer() {
    delete _RealBuffer;
    delete _ImagBuffer;
}

void AudioComponentsBuffer::Clear() {
    _RealBuffer->Free();
    _ImagBuffer->Free();
}

void AudioComponentsBuffer::Resize(size_t size) {
    _RealBuffer->Resize(size);
    _ImagBuffer->Resize(size);
    _Size = size;
}

void AudioComponentsBuffer::ResetData() {
    _RealBuffer->ResetData();
    _ImagBuffer->ResetData();
    _Size = 0;
}

bool AudioComponentsBuffer::CloneFrom(AudioComponentsBuffer* source) {
    bool realCloneResult = _RealBuffer->CloneFrom(source->_RealBuffer),
         imagCloneResult = _RealBuffer->CloneFrom(source->_ImagBuffer);
    _Size = source->_Size;

    return realCloneResult && imagCloneResult;
}

size_t AudioComponentsBuffer::GetSize() {
    return _Size;
}

sample* AudioComponentsBuffer::GetRealData() {
    return _RealBuffer->GetData();
}

Buffer* AudioComponentsBuffer::GetRealBuffer() {
    return _RealBuffer;
}

sample* AudioComponentsBuffer::GetImagData() {
    return _ImagBuffer->GetData();
}

Buffer* AudioComponentsBuffer::GetImagBuffer() {
    return _ImagBuffer;
}
