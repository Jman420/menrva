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

Buffer::Buffer(FftInterfaceBase* fftEngine, size_t length) {
    _FftEngine = fftEngine;
    Initialize(length);
    ResetData();
}

Buffer::Buffer(FftInterfaceBase* fftEngine, sample* data, size_t length) {
    _FftEngine = fftEngine;
    SetData(data, length);
}

Buffer::~Buffer() {
    Reset();

    delete _Data;
}

void Buffer::Reset() {
    _Length = 0;
    _MemorySize = 0;

    if (_Data) {
        _FftEngine->Deallocate(_Data);
    }
}

void Buffer::Resize(size_t length) {
    if (_Length == length) {
        ResetData();
        return;
    }

    Reset();
    Initialize(length);
}

void Buffer::ResetData() {
    if (_Length < 1) {
        return;
    }

    memset(_Data, 0, _MemorySize);
}

bool Buffer::CloneFrom(const Buffer* source) {
    if (source->_Length != _Length || source == this) {
        return false;
    }

    memcpy(_Data, source->_Data, _MemorySize);
    return true;
}

sample& Buffer::operator[](size_t index) {
    assert(_Length > 0 && index < _Length);
    return _Data[index];
}

size_t Buffer::GetLength() {
    return _Length;
}

sample* Buffer::GetData() {
    return _Data;
}

void Buffer::Initialize(size_t length) {
    _Length = length;
    _MemorySize = CalculateMemorySize(_Length);
    if (_Length < 1) {
        return;
    }

    _Data = _FftEngine->Allocate(length);
}

void Buffer::Swap(Buffer* itemA, Buffer* itemB) {
    Buffer* temp = itemA;
    itemA = itemB;
    itemB = temp;
}

void Buffer::SetData(sample* data, size_t length, bool freeExisting) {
    if (_Data && freeExisting) {
        Reset();
    }

    _Data = data;
    _Length = length;
    _MemorySize = CalculateMemorySize(_Length);
}

size_t Buffer::CalculateMemorySize(size_t length) {
    return sizeof(sample) * length;
}
