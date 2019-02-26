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

#include "wave_generator.h"
#include "../ir/wave_generator_constants.h"

WaveGenerator::WaveGenerator(FftInterfaceBase* fftEngine) {
    _FftEngine = fftEngine;
}

WaveGenerator::~WaveGenerator() {
    delete _FftEngine;
}

AudioBuffer* WaveGenerator::CalculateSineWave(sample amplitude, sample frequency, sample offset, size_t length) {
    AudioBuffer& sineWaveBuffer = *new AudioBuffer(_FftEngine, length);
    for (int sampleCounter = 0; sampleCounter < length; sampleCounter++) {
        sineWaveBuffer[sampleCounter] = static_cast<sample>(amplitude * sin((M_PI * 2.0 / length) * frequency * sampleCounter + offset));
    }

    return &sineWaveBuffer;
}
