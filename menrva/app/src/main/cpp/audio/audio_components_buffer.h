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

#ifndef MENRVA_AUDIOCOMPONENTSBUFFER_H
#define MENRVA_AUDIOCOMPONENTSBUFFER_H

#include <cstddef>
#include "audio_buffer.h"

class AudioComponentsBuffer {
public:
    AudioComponentsBuffer(FFTInterfaceBase* fftEngine, size_t size);
    ~AudioComponentsBuffer();

    void clear();
    void resize(size_t size);
    void resetData();
    bool cloneFrom(AudioComponentsBuffer* source);

    size_t getSize();
    sample* getRealData();
    sample* getImagData();
    AudioBuffer* getRealBuffer();
    AudioBuffer* getImagBuffer();

private:
    size_t _Size;
    AudioBuffer* _RealBuffer;
    AudioBuffer* _ImagBuffer;
};

#endif //MENRVA_AUDIOCOMPONENTSBUFFER_H
