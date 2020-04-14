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

#include "IBuffer.h"

template<class TInputType>
class Buffer 
        : public IBuffer<TInputType> {
public:
    virtual ~Buffer();

    virtual size_t GetLength() override;

    virtual void SetData(TInputType* data, size_t length) override;
    virtual void ResetData() override;
    virtual void ResetData(size_t startIndex) override;
    virtual TInputType* GetData() override;

    virtual TInputType& operator[](size_t index) override;

    static void Swap(Buffer* itemA, Buffer* itemB);

protected:
    size_t _Length,
           _MemorySize;
    TInputType* _Data;

    size_t CalculateMemorySize(size_t length);
};

#endif //MENRVA_BUFFER_H
