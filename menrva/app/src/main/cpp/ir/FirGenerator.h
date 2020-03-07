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

#ifndef MENRVA_FIR_GENERATOR_H
#define MENRVA_FIR_GENERATOR_H

#include <string>
#include <cmath>
#include "../log/LoggingBase.h"
#include "../fft/FftInterfaceBase.h"
#include "../audio/Sample.h"
#include "../audio/AudioBuffer.h"

class FirGenerator
        : public LoggingBase {
public:
    FirGenerator(LoggerBase* logger, FftInterfaceBase* fftEngine);
    ~FirGenerator();

    /* Calculate() - Returns a Finite Impulse Response of size 'filterLength' based on the provided Frequency Samples & Amplitudes.
     * filterLength - Length of resulting FIR Filter (number of elements in the filter; 2048 or 4096 or other power of 2)
     * frequencySamples - Array of Frequency values to interpolate
     * amplitudeSamples - Array of Amplitude values to interpolate
     * sampleSize - Length of Frequency & Amplitude Arrays
     */
    AudioBuffer* Calculate(size_t filterLength, sample* frequencySamples, sample* amplitudeSamples, size_t sampleLength);

private:
    FftInterfaceBase* _FftEngine;
};

#endif //MENRVA_FIR_GENERATOR_H
