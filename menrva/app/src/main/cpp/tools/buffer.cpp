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
#include "buffer.h"
#include "../abstracts/fft_interface_base.h"

Buffer::Buffer(FFTInterfaceBase* fftEngine, size_t size) {
    _FftEngine = fftEngine;
    initialize(size);
}

Buffer::~Buffer() {
    clear();
}

void Buffer::clear() {
    _FftEngine->Deallocate(_Data);
    _Size = 0;
}

void Buffer::resize(size_t size) {
    if (_Size == size) {
        resetData();
        return;
    }

    clear();
    initialize(size);
}

void Buffer::resetData() {
    memset(_Data, 0, sizeof(sample) * _Size);
}

bool Buffer::cloneFrom(const Buffer* source) {
    if (source->_Size != _Size || source == this) {
        return false;
    }

    memcpy(_Data, source->_Data, sizeof(sample) * _Size);
    return true;
}

sample& Buffer::operator[](size_t index) {
    return _Data[index];
}

size_t Buffer::getSize() {
    return _Size;
}

sample* Buffer::getData() {
    return _Data;
}

void Buffer::initialize(size_t size) {
    _Size = size;

    if (size > 0) {
        _Data = _FftEngine->Allocate(size);
    }
}

void Buffer::swap(Buffer* itemA, Buffer* itemB) {
    Buffer* temp = itemA;
    itemA = itemB;
    itemB = temp;
}