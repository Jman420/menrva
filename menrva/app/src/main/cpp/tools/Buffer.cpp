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
#include <stdexcept>
#include "Buffer.h"
#include "../audio/Sample.h"

template<class TInputType>
Buffer<TInputType>::Buffer() {
    Free();
}

template<class TInputType>
Buffer<TInputType>::Buffer(TInputType* data, size_t length) {
    SetData(data, length);
}

template<class TInputType>
Buffer<TInputType>::~Buffer() {
    Free();
}

template<class TInputType>
void Buffer<TInputType>::Free() {
    _Length = 0;
    _MemorySize = 0;
    _Data = nullptr;
}

template<class TInputType>
void Buffer<TInputType>::ResetData() {
    if (_Length < 1) {
        return;
    }

    memset(_Data, 0, _MemorySize);
}

template<class TInputType>
void Buffer<TInputType>::ResetData(size_t startIndex) {
    if (_Length < 1) {
        return;
    }

    size_t memSizeNotReset = CalculateMemorySize(startIndex);
    memset(&_Data[startIndex], 0, _MemorySize - memSizeNotReset);
}

template<class TInputType>
size_t Buffer<TInputType>::GetLength() {
    return _Length;
}

template<class TInputType>
void Buffer<TInputType>::SetData(TInputType* data, size_t length) {
    _Data = data;
    _Length = length;
    _MemorySize = CalculateMemorySize(_Length);
}

template<class TInputType>
TInputType* Buffer<TInputType>::GetData() {
    return _Data;
}

template<class TInputType>
TInputType& Buffer<TInputType>::operator[](size_t index) {
    if (index > _Length) {
        throw std::runtime_error("Buffer index out of bounds.");
    }

    return _Data[index];
}

template<class TInputType>
void Buffer<TInputType>::Swap(Buffer* itemA, Buffer* itemB) {
    Buffer* temp = itemA;
    itemA = itemB;
    itemB = temp;
}

template class Buffer<int16_t>;
template class Buffer<int32_t>;
template class Buffer<float>;
template class Buffer<double>;
