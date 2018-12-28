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

void AudioComponentsBuffer::clear() {
    _RealBuffer->clear();
    _ImagBuffer->clear();
}

void AudioComponentsBuffer::resize(size_t size) {
    _RealBuffer->resize(size);
    _ImagBuffer->resize(size);
    _Size = size;
}

void AudioComponentsBuffer::resetData() {
    _RealBuffer->resetData();
    _ImagBuffer->resetData();
    _Size = 0;
}

bool AudioComponentsBuffer::cloneFrom(AudioComponentsBuffer* source) {
    bool realCloneResult = _RealBuffer->cloneFrom(source->_RealBuffer),
         imagCloneResult = _RealBuffer->cloneFrom(source->_ImagBuffer);
    _Size = source->_Size;

    return realCloneResult && imagCloneResult;
}

size_t AudioComponentsBuffer::getSize() {
    return _Size;
}

sample* AudioComponentsBuffer::getRealData() {
    return _RealBuffer->getData();
}

Buffer* AudioComponentsBuffer::getRealBuffer() {
    return _RealBuffer;
}

sample* AudioComponentsBuffer::getImagData() {
    return _ImagBuffer->getData();
}

Buffer* AudioComponentsBuffer::getImagBuffer() {
    return _ImagBuffer;
}
