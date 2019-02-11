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

#include "audio_buffer.h"
#include "sample.h"

class AudioComponentsBuffer {
public:
    AudioComponentsBuffer();
    AudioComponentsBuffer(FftInterfaceBase* fftEngine, size_t length);
    ~AudioComponentsBuffer();

    void CreateData(FftInterfaceBase* fftEngine, size_t length);

    size_t GetLength();
    sample* GetRealData();
    sample* GetImagData();
    AudioBuffer* GetRealBuffer();
    AudioBuffer* GetImagBuffer();

    void ResetData();

private:
    size_t _Length;
    AudioBuffer* _RealBuffer;
    AudioBuffer* _ImagBuffer;
};

#endif //MENRVA_AUDIO_COMPONENTS_BUFFER_H
