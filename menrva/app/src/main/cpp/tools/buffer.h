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

#ifndef MENRVA_BUFFER_H
#define MENRVA_BUFFER_H

#include <cstddef>
#include "../audio/sample.h"

class FftInterfaceBase;  // Forward Declaration to avoid circular reference : ../abstracts/fft_interface_base.h

class Buffer {
public:
    Buffer(FftInterfaceBase* fftEngine, size_t length = 0);
    Buffer(FftInterfaceBase* fftEngine, sample* data, size_t length);
    ~Buffer();

    bool CloneFrom(const Buffer* source);
    void SetData(sample* data, size_t length, bool freeExisting = true);
    void Resize(size_t length);
    void ResetData();
    void Free();

    sample& operator[](size_t index);
    size_t GetLength();
    sample* GetData();

    static void Swap(Buffer* itemA, Buffer* itemB);

private:
    static size_t CalculateMemorySize(size_t length);

    size_t _Length,
           _MemorySize;
    sample* _Data;
    FftInterfaceBase* _FftEngine;

    void Initialize(size_t length);
};

#endif //MENRVA_BUFFER_H
