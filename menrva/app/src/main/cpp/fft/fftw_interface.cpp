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

PlanCache* FFTW_Interface::_PlansCache = new PlanCache();

FFTW_Interface::FFTW_Interface(unsigned int signalSize, unsigned int componentSize) :
        FFTInterfaceBase(signalSize, componentSize) {}

int FFTW_Interface::Initialize(unsigned int signalSize, unsigned int componentSize) {
    if (componentSize < 1) {
        componentSize = signalSize / 2 + 1;
    }

    std::string plansKey = std::to_string(signalSize) + "x" + std::to_string(componentSize);
    std::map<std::string, fftw_plan_pair>::iterator cachedPlansIterator = _PlansCache->find(plansKey);
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

    _Plans.Real2ComplexPlan = fftwf_plan_guru_split_dft_r2c(1, &dim, 0, 0, outputSignal, freqReal, freqImag, FFTW_MEASURE);
    _Plans.Complex2RealPlan = fftwf_plan_guru_split_dft_c2r(1, &dim, 0, 0, freqReal, freqImag, outputSignal, FFTW_MEASURE);
    _PlansCache->insert( { plansKey, _Plans } );
    FFTInterfaceBase::Initialize(signalSize, componentSize);

    Deallocate(outputSignal);
    Deallocate(freqReal);
    Deallocate(freqImag);
    return componentSize;
}

float* FFTW_Interface::Allocate(size_t size) {
    return fftwf_alloc_real(size);
}

void FFTW_Interface::Deallocate(float* data) {
    fftwf_free(data);
}

void FFTW_Interface::SignalToComponents(float* signal, float* realComponents, float* imagComponents) {
    fftwf_execute_split_dft_r2c(_Plans.Real2ComplexPlan, signal, realComponents, imagComponents);
}

void FFTW_Interface::ComponentsToSignal(float* signal, float* realComponents, float* imagComponents) {
    fftwf_execute_split_dft_c2r(_Plans.Complex2RealPlan, realComponents, imagComponents, signal);
}
