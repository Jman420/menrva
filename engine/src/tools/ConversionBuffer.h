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
#include "IConversionBuffer.h"
#include "Buffer.h"

template<class TInputType, class TOutputType>
class ConversionBuffer
        : public IConversionBuffer<TInputType, TOutputType> {
public:
    ConversionBuffer();
    virtual ~ConversionBuffer();

    virtual size_t GetLength() override;
    virtual void ResetData() override;

    virtual void SetData(TInputType* data, size_t length) override;
    virtual TOutputType operator[](size_t index) const override;  // Read Operations
    virtual TInputType& operator[](size_t index) override;        // Write Operations
    virtual TInputType* GetData() override;

private:
    Buffer<TInputType>* _DataBuffer;
};

#endif //MENRVA_CONVERSION_BUFFER_H
