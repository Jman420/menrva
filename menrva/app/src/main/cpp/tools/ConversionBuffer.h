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

#ifndef MENRVA_CONVERSION_BUFFER_H
#define MENRVA_CONVERSION_BUFFER_H

#include <cstddef>
#include "Buffer.h"

template<class TInputType, class TOutputType>
class ConversionBuffer {
public:
    ConversionBuffer();
    ~ConversionBuffer();

    size_t GetLength();
    void ResetData();
    void Free();

    void SetData(TInputType* data, size_t length);
    TOutputType operator[](size_t index) const;  // Read Operations
    TInputType& operator[](size_t index);        // Write Operations
    TInputType* GetData();

private:
    Buffer<TInputType>* _DataBuffer;
};

#endif //MENRVA_CONVERSION_BUFFER_H
