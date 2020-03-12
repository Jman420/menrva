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

#ifndef MENRVA_WAVE_GENERATOR_H
#define MENRVA_WAVE_GENERATOR_H

#include "../audio/Sample.h"
#include "../audio/AudioBuffer.h"

class WaveGenerator {
public:
    explicit WaveGenerator(FftInterfaceBase* fftEngine);
    ~WaveGenerator();

    AudioBuffer* CalculateSineWave(sample amplitude, sample frequency, sample offset, size_t length);

private:
    FftInterfaceBase* _FftEngine;
};

#endif //MENRVA_WAVE_GENERATOR_H
