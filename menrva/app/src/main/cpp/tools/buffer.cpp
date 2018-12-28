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

#include <cstring>
#include <cassert>
#include "buffer.h"
#include "../abstracts/fft_interface_base.h"

Buffer::Buffer(FFTInterfaceBase* fftEngine, size_t size) {
    _FftEngine = fftEngine;
    Initialize(size);
}

Buffer::Buffer(FFTInterfaceBase* fftEngine, sample* data, size_t size) {
    _FftEngine = fftEngine;
    SetData(data, size);
}

Buffer::~Buffer() {
    Free();
}

void Buffer::Free() {
    if (!_Data) {
        return;
    }

    _FftEngine->Deallocate(_Data);
    _Size = 0;
}

void Buffer::Resize(size_t size) {
    if (_Size == size) {
        ResetData();
        return;
    }

    Free();
    Initialize(size);
}

void Buffer::ResetData() {
    memset(_Data, 0, sizeof(sample) * _Size);
}

bool Buffer::CloneFrom(const Buffer* source) {
    if (source->_Size != _Size || source == this) {
        return false;
    }

    memcpy(_Data, source->_Data, sizeof(sample) * _Size);
    return true;
}

sample& Buffer::operator[](size_t index) {
    assert(_Size > 0 && index < _Size);
    return _Data[index];
}

size_t Buffer::GetSize() {
    return _Size;
}

sample* Buffer::GetData() {
    return _Data;
}

void Buffer::Initialize(size_t size) {
    _Size = size;

    if (size > 0) {
        _Data = _FftEngine->Allocate(size);
    }
}

void Buffer::Swap(Buffer* itemA, Buffer* itemB) {
    Buffer* temp = itemA;
    itemA = itemB;
    itemB = temp;
}

void Buffer::SetData(sample* data, size_t size, bool freeExisting) {
    if (_Data && freeExisting) {
        Free();
    }

    _Data = data;
    _Size = size;
}
