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
#include "../abstracts/fft_interface_base.h"
#include "../audio/sample.h"

class Buffer {
public:
    Buffer(FFTInterfaceBase* fftEngine, size_t size);
    ~Buffer();

    void clear();
    void resize(size_t size);
    void resetData();
    bool cloneFrom(const Buffer* source);

    sample operator[](size_t index);
    size_t getSize();
    sample* getData();

    static void swap(Buffer* itemA, Buffer* itemB);

private:
    size_t _Size;
    sample* _Data;
    FFTInterfaceBase* _FftEngine;

    void initialize(size_t size);
};

#endif //MENRVA_BUFFER_H
