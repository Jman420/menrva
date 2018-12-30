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

#include <cstring>
#include <algorithm>
#include <cstdlib>
#include "convolution_operations.h"

void ConvolutionOperations::ResetAndClone(AudioBuffer* source, AudioBuffer* destination) {
    assert(destination->GetLength() >= source->GetLength());

    size_t cloneLength = destination->GetLength();
    size_t sizeDiff = cloneLength - source->GetLength();
    size_t sampleTypeSize = sizeof(sample);
    sample* destinationData = destination->GetData();
    size_t cloneSize = sampleTypeSize * cloneLength;

    memcpy(destinationData, source, cloneSize);
    memset(destinationData + cloneSize, 0, sampleTypeSize * sizeDiff);
}

void ConvolutionOperations::Sum(AudioBuffer& bufferA, AudioBuffer& bufferB, AudioBuffer& output) {
    assert(output.GetLength() <= bufferA.GetLength());
    assert(bufferA.GetLength() == bufferB.GetLength());

    for (int sampleCounter = 0; sampleCounter < output.GetLength(); sampleCounter++) {
        output[sampleCounter] = bufferA[sampleCounter] + bufferB[sampleCounter];
    }
}

void ConvolutionOperations::ComplexMultiplyAccumulate(AudioComponentsBuffer* bufferA,
                                                     AudioComponentsBuffer* bufferB,
                                                     AudioComponentsBuffer* output) {
    assert(output->GetLength() <= bufferA->GetLength());
    assert(bufferA->GetLength() == bufferB->GetLength());

    AudioBuffer bufferAReal = *bufferA->GetRealBuffer(),
                bufferAImag = *bufferA->GetImagBuffer(),
                bufferBReal = *bufferB->GetRealBuffer(),
                bufferBImag = *bufferB->GetImagBuffer(),
                outputReal = *output->GetRealBuffer(),
                outputImag = *output->GetImagBuffer();

    for (int sampleCounter = 0; sampleCounter < output->GetLength(); sampleCounter++) {
        outputReal[sampleCounter] += bufferAReal[sampleCounter] * bufferBReal[sampleCounter] - bufferAImag[sampleCounter] * bufferBImag[sampleCounter];
        outputImag[sampleCounter] += bufferAReal[sampleCounter] * bufferBImag[sampleCounter] + bufferAImag[sampleCounter] * bufferBReal[sampleCounter];
    }
}
