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

#ifndef MENRVA_AUDIO_COMPONENTS_BUFFER_H
#define MENRVA_AUDIO_COMPONENTS_BUFFER_H

#include "Sample.h"
#include "IAudioBuffer.h"
#include "../fourier_transform/FftInterfaceBase.h"

class AudioComponentsBuffer {
public:
    explicit AudioComponentsBuffer(IAudioBuffer* realBuffer, IAudioBuffer* imaginaryBuffer);
    ~AudioComponentsBuffer();

    void CreateData(FftInterfaceBase* fftEngine, size_t length);

    size_t GetLength();
    sample* GetRealData();
    sample* GetImaginaryData();
    IAudioBuffer* GetRealBuffer();
    IAudioBuffer* GetImaginaryBuffer();

    void SetRealValue(size_t index, sample value);
    void SetImaginaryValue(size_t index, sample value);

    void ResetData();

private:
    size_t _Length;
    IAudioBuffer* _RealBuffer;
    IAudioBuffer* _ImaginaryBuffer;
};

#endif //MENRVA_AUDIO_COMPONENTS_BUFFER_H
