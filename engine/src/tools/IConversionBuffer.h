/* Menrva - Audio Effect Engine supporting Plug'n'Play style DSP Effects
 * Copyright (C) 2020 Justin Giannone (aka Jman420)
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

#ifndef MENRVA_ICONVERSION_BUFFER_H
#define MENRVA_ICONVERSION_BUFFER_H

template<class TInputType, class TOutputType>
class IConversionBuffer {
public:
    virtual ~IConversionBuffer() = default;

    virtual size_t GetLength() = 0;
    virtual void ResetData() = 0;

    virtual void SetData(TInputType* data, size_t length) = 0;
    virtual TOutputType operator[](size_t index) const = 0;  // Read Operations
    virtual TInputType& operator[](size_t index) = 0;        // Write Operations
    virtual TInputType* GetData() = 0;
};

#endif //MENRVA_ICONVERSION_BUFFER_H
