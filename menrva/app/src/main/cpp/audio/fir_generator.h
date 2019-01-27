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
#include "../abstracts/logging_base.h"
#include "../abstracts/fft_interface_base.h"
#include "sample.h"
#include "audio_buffer.h"

class FirGenerator : public LoggingBase {
public:
    FirGenerator(LoggerBase* logger, FftInterfaceBase* fftEngine);

    /*
     * Calculate() - Returns a Finite Impulse Response of size 'filterSize' based on the provided
     *   Frequency Samples & Amplitudes.
     * filterSize - Size of resulting FIR Filter (number of elements in the filter; 2048 or 4096 or other power of 2)
     * frequencySamples - Array of Frequency values to interpolate
     * amplitudeSamples - Array of Amplitude values to interpolate
     * sampleSize - Length of Frequency & Amplitude Arrays
     */
    AudioBuffer* Calculate(size_t filterLength, sample* frequencySamples, sample* amplitudeSamples, size_t sampleLength);

private:
    static const std::string LOG_SENDER;
    static constexpr sample PI = (sample)M_PI,
                            PI2 = (sample)M_PI * (sample)2.0,
                            ONE = (sample)1.0,
                            ONE_HALF = (sample)0.5,
                            HAMMING_054 = (sample)0.54,
                            HAMMING_046 = (sample)0.46;

    FftInterfaceBase* _FftEngine;
};

#endif //MENRVA_FIR_GENERATOR_H
