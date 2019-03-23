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

#include "audio_buffer.h"
#include "../tools/buffer.cpp"
#include "../abstracts/fft_interface_base.h"

AudioBuffer::AudioBuffer() {
    _FftEngine = nullptr;
    _DisposeData = false;
    _Length = 0;
}

AudioBuffer::AudioBuffer(FftInterfaceBase* fftEngine, size_t length) {
    CreateData(fftEngine, length);
}

AudioBuffer::AudioBuffer(sample* data, size_t length) {
    _FftEngine = nullptr;
    SetData(data, length);
}

AudioBuffer::~AudioBuffer() {
    DisposeData();
}

void AudioBuffer::SetData(sample* data, size_t length) {
    DisposeData();

    Buffer::SetData(data, length);
    _DisposeData = false;
}

void AudioBuffer::CreateData(FftInterfaceBase* fftEngine, size_t length) {
    DisposeData();

    _FftEngine = fftEngine;
    SetData(_FftEngine->Allocate(length), length);
    _DisposeData = true;
}

void AudioBuffer::DisposeData() {
    if (!_DisposeData || !_Data) {
        return;
    }

    _FftEngine->Deallocate(_Data);
}
