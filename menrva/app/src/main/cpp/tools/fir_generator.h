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
#include <math.h>
#include "../abstracts/fft_interface_base.h"

class FIR_Generator {
public:
    FIR_Generator(FFTInterfaceBase* fftEngine);

    /*
     * Create() - Returns a Finite Impulse Response of size 'interpolationSize' based on the provided
     * Frequency Samples & Amplitudes.
     * interpolationSize - Size of resulting FIR Filter (number of elements in the filter; 2048 or 4096)
     * frequencySamples - Array of Frequency values to interpolate
     * amplitudeSamples - Array of Amplitude values to interpolate
     * sampleSize - Length of Frequency & Amplitude Arrays
     */
    float* Create(unsigned int filterSize, float* frequencySamples, float* amplitudeSamples, unsigned int sampleSize);

private:
    static const std::string LOG_TAG;
    static constexpr float PI2 = (float)M_PI * 2.0f;

    FFTInterfaceBase* _FFTEngine;
};

#endif //MENRVA_FIR_GENERATOR_H
