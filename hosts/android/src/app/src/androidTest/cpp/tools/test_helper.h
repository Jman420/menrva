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

#ifndef DSP_COMPARE_TEST_HELPER_H
#define DSP_COMPARE_TEST_HELPER_H

#include <vector>
#include <cmath>
#include <menrvaEngine/audio/Sample.h>

struct test_params {
    sample SampleRate = 44100.0,
           FirCenterFreq = 60.0,
           FirFreqTransition = 80.0,
           FirStrength = 6.0;
    size_t AndroidAudioFrameLength = 1152,
           MenrvaAudioFrameLength = 1024,
           FirInterpolationLength = 4096,
           FirSamplesLength = 4;
    sample FirFrequencySamples[4] = { 0, (FirCenterFreq * 2.0f) / SampleRate, (FirCenterFreq * 2.0f + FirFreqTransition) / SampleRate, 1.0 },
           FirAmplitudeSamples[4] = { pow(10.0f, FirStrength / 20.0f), pow(10.0f, FirStrength / 20.0f), 1.0, 1.0 };
};

#endif //DSP_COMPARE_TEST_HELPER_H
