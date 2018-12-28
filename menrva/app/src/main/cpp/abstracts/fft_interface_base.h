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

#ifndef MENRVA_FFTENGINEBASE_H
#define MENRVA_FFTENGINEBASE_H

#include <cstddef>
#include "../audio/sample.h"
#include "../audio/audio_buffer.h"
#include "../audio/audio_components_buffer.h"

class FFTInterfaceBase {
public:
    FFTInterfaceBase(unsigned int signalSize = 0, unsigned int componentSize = 0);
    virtual int Initialize(unsigned int signalSize, unsigned int componentSize = 0);
    virtual void SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) = 0;
    virtual void ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) = 0;
    virtual sample* Allocate(size_t size) = 0;
    virtual void Deallocate(sample* data) = 0;
    int GetSignalSize();
    int GetComponentSize();

protected:
    int _SignalSize,
        _ComponentSize;
};

#endif //MENRVA_FFTENGINEBASE_H
