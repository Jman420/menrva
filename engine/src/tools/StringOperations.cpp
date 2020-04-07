/* Menrva - Over-Engineered Tunable Android Audio Effects
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

#include <cstdarg>
#include <vector>
#include "StringOperations.h"

std::string StringOperations::FormatString(std::string format, ...) {
    va_list args;
    const char* formatCStr = format.c_str();

    va_start(args, format);
    size_t outputLength = static_cast<size_t>(std::vsnprintf(NULL, 0, formatCStr, args));
    outputLength++;  // Accommodate for the appended null character
    va_end(args);

    va_start(args, format);
    std::vector<char> resultBuffer(outputLength);
    char* resultPtr = &resultBuffer[0];
    std::vsnprintf(resultPtr, outputLength, formatCStr, args);
    va_end(args);

    return resultPtr;
}

std::string StringOperations::Convert(bool value)
{
    return value ? "true" : "false";
}
