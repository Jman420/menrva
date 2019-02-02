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
#include <malloc.h>
#include "fft_interface_base.h"

FftInterfaceBase::FftInterfaceBase() {
    _SignalSize = 0;
    _ComponentSize = 0;
}

size_t FftInterfaceBase::GetSignalSize() {
    return _SignalSize;
}

size_t FftInterfaceBase::GetComponentSize() {
    return _ComponentSize;
}

size_t FftInterfaceBase::Initialize(size_t signalSize, size_t componentSize) {
    if (signalSize > 0 && componentSize < 1) {
        componentSize = (signalSize / 2) + 1;
    }

    _SignalSize = signalSize;
    _ComponentSize = componentSize;

    return componentSize;
}

size_t FftInterfaceBase::Initialize(size_t signalSize) {
    return Initialize(signalSize, 0);
}

sample* FftInterfaceBase::Allocate(size_t size) {
    sample* buffer = (sample*)malloc(sizeof(sample) * size);
    memset(buffer, 0, size);
    return buffer;
}

void FftInterfaceBase::Deallocate(sample* data) {
    delete data;
}
