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

    _Plans.Real2ComplexPlan = Fftw3PlanReal2Complex(1, &dim, 0, 0, outputSignal, freqReal, freqImag, FFTW_MEASURE);
    _Plans.Complex2RealPlan = Fftw3PlanComplex2Real(1, &dim, 0, 0, freqReal, freqImag, outputSignal, FFTW_MEASURE);
    _PlansCache->insert( { plansKey, _Plans } );
    FFTInterfaceBase::Initialize(signalSize, componentSize);

    Deallocate(outputSignal);
    Deallocate(freqReal);
    Deallocate(freqImag);
    return componentSize;
}

float* FftwInterface::Allocate(size_t size) {
    return Fftw3Allocate(size);
}

void FftwInterface::Deallocate(sample* data) {
    Fftw3Free(data);
}

void FftwInterface::SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) {
    // TODO : Validate signal & component lengths

    Fftw3ExecuteReal2Complex(_Plans.Real2ComplexPlan, signal->GetData(), components->GetRealData(),
                             components->GetImagData());
}

void FftwInterface::ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) {
    // TODO : Validate signal & component lengths

    Fftw3ExecuteComplex2Real(_Plans.Complex2RealPlan, components->GetRealData(),
                             components->GetImagData(), signal->GetData());
}
