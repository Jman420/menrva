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

AudioBuffer::AudioBuffer(FftInterfaceBase* fftEngine) {
    _FftEngine = fftEngine;
    _DisposeData = false;
}

AudioBuffer::AudioBuffer(FftInterfaceBase* fftEngine, size_t length) {
    _FftEngine = fftEngine;
    _Length = length;
    SetData(_FftEngine->Allocate(_Length), _Length);
    _DisposeData = true;
}

AudioBuffer::AudioBuffer(FftInterfaceBase* fftEngine, sample* data, size_t length) {
    _FftEngine = fftEngine;
    _Length = length;
    SetData(data, _Length);
}

AudioBuffer::~AudioBuffer() {
    if (!_DisposeData) {
        return;
    }

    delete _Data;
}

void AudioBuffer::SetData(sample* data, size_t length) {
    Buffer::SetData(data, length);
    _DisposeData = false;
}
