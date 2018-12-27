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

#include <string>
#include "fftw_interface.h"
#include "../abstracts/fft_interface_base.h"

PlanCache* FftwInterface::_PlansCache = new PlanCache();

FftwInterface::FftwInterface(unsigned int signalSize, unsigned int componentSize) :
        FFTInterfaceBase(signalSize, componentSize) {}

int FftwInterface::Initialize(unsigned int signalSize, unsigned int componentSize) {
    if (signalSize == 0 && componentSize == 0) {
        FFTInterfaceBase::Initialize(signalSize, componentSize);
        return 0;
    }

    if (componentSize < 1) {
        componentSize = signalSize / 2 + 1;
    }

    std::string plansKey = std::to_string(signalSize) + "x" + std::to_string(componentSize);
    PlanCache::iterator cachedPlansIterator = _PlansCache->find(plansKey);
    if (cachedPlansIterator != _PlansCache->end()) {
        _Plans = cachedPlansIterator->second;
        return componentSize;
    }

    fftw_iodim dim;
    dim.n = signalSize;
    dim.is = dim.os = 1;

    float* outputSignal = Allocate(signalSize);
    float* freqReal = Allocate(componentSize);
    float* freqImag = Allocate(componentSize);

    _Plans.Real2ComplexPlan = Fftw3PlanReal2ComplexFunc(1, &dim, 0, 0, outputSignal, freqReal, freqImag, FFTW_MEASURE);
    _Plans.Complex2RealPlan = Fftw3PlanComplex2RealFunc(1, &dim, 0, 0, freqReal, freqImag, outputSignal, FFTW_MEASURE);
    _PlansCache->insert( { plansKey, _Plans } );
    FFTInterfaceBase::Initialize(signalSize, componentSize);

    Deallocate(outputSignal);
    Deallocate(freqReal);
    Deallocate(freqImag);
    return componentSize;
}

float* FftwInterface::Allocate(size_t size) {
    return fftwf_alloc_real(size);
}

void FftwInterface::Deallocate(sample* data) {
    fftwf_free(data);
}

void FftwInterface::SignalToComponents(sample* signal, sample* realComponents, sample* imagComponents) {
    fftwf_execute_split_dft_r2c(_Plans.Real2ComplexPlan, signal, realComponents, imagComponents);
}

void FftwInterface::ComponentsToSignal(sample* signal, sample* realComponents, sample* imagComponents) {
    fftwf_execute_split_dft_c2r(_Plans.Complex2RealPlan, realComponents, imagComponents, signal);
}
