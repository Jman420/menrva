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

#include "ConversionBuffer.h"

template<class TInputType, class TOutputType>
ConversionBuffer<TInputType, TOutputType>::ConversionBuffer(){
    _DataBuffer = new Buffer<TInputType>();
}

template<class TInputType, class TOutputType>
ConversionBuffer<TInputType, TOutputType>::~ConversionBuffer() {
    delete _DataBuffer;
}

template<class TInputType, class TOutputType>
size_t ConversionBuffer<TInputType, TOutputType>::GetLength() {
    return _DataBuffer->GetLength();
}

template<class TInputType, class TOutputType>
void ConversionBuffer<TInputType, TOutputType>::ResetData() {
    _DataBuffer->ResetData();
}

template<class TInputType, class TOutputType>
void ConversionBuffer<TInputType, TOutputType>::Free() {
    _DataBuffer->Free();
}

template<class TInputType, class TOutputType>
void ConversionBuffer<TInputType, TOutputType>::SetData(TInputType* data, size_t length) {
    _DataBuffer->SetData(data, length);
}

template<class TInputType, class TOutputType>
TOutputType ConversionBuffer<TInputType, TOutputType>::operator[](size_t index) const {
    return (TOutputType)(*_DataBuffer)[index];
}

template<class TInputType, class TOutputType>
TInputType &ConversionBuffer<TInputType, TOutputType>::operator[](size_t index) {
    return (*_DataBuffer)[index];
}

template<class TInputType, class TOutputType>
TInputType* ConversionBuffer<TInputType, TOutputType>::GetData() {
    return _DataBuffer->GetData();
}
