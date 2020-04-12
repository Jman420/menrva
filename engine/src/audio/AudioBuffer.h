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

#ifndef MENRVA_AUDIO_BUFFER_H
#define MENRVA_AUDIO_BUFFER_H

#include "Sample.h"
#include "../tools/Buffer.h"
#include "../fourier_transform/FftInterfaceBase.h"

class AudioBuffer
        : public Buffer<sample> {
public:
    AudioBuffer();
    AudioBuffer(FftInterfaceBase* fftEngine, size_t length);
    AudioBuffer(sample* data, size_t length);
    ~AudioBuffer() override;

    void SetData(sample* data, size_t length) override;
    void CreateData(FftInterfaceBase* fftEngine, size_t length);

private:
    FftInterfaceBase* _FftEngine;
    bool _DisposeData;

    void DisposeData();
};

#endif //MENRVA_AUDIO_BUFFER_H
