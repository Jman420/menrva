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

#ifndef MENRVA_FFT_INTERFACE_BASE_H
#define MENRVA_FFT_INTERFACE_BASE_H

#include <cstddef>
#include "../audio/sample.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_components_buffer.h"

class FftInterfaceBase {
public:
    explicit FftInterfaceBase();

    size_t Initialize(size_t signalSize);
    size_t GetSignalSize();
    size_t GetComponentSize();

    virtual size_t Initialize(size_t signalSize, size_t componentSize);
    virtual void SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) = 0;
    virtual void ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) = 0;
    virtual sample* Allocate(size_t size);
    virtual void Deallocate(sample* data);

protected:
    size_t _SignalSize,
           _ComponentSize;
};

#endif //MENRVA_FFT_INTERFACE_BASE_H
