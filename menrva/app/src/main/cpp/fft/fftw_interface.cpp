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

FftwPlanCache* FftwInterface::_PlansCache = new FftwPlanCache();

FftwInterface::FftwInterface(LoggerBase* logger) :
        FftInterfaceBase(logger) {}

size_t FftwInterface::Initialize(size_t signalSize, size_t componentSize) {
    _Logger->WriteLog("Initializing FFTW Interface...", LOG_SENDER, __func__);
    componentSize = FftInterfaceBase::Initialize(signalSize, componentSize);
    if (signalSize < 1 && componentSize < 1) {
        return componentSize;
    }

    _Logger->WriteLog("Calculating FFT Plans Cache Key for Signal Size (%d) and Component Size (%d)...", LOG_SENDER, __func__, signalSize, componentSize);
    std::string plansKey = std::to_string(signalSize) + "x" + std::to_string(componentSize);
    const char* plansKeyC = plansKey.c_str();

    _Logger->WriteLog("Checking FFT Plans Cache for Key (%s)...", LOG_SENDER, __func__, plansKeyC);
    FftwPlanCache::iterator cachedPlansIterator = _PlansCache->find(plansKey);
    if (cachedPlansIterator != _PlansCache->end()) {
        _Logger->WriteLog("Successfully found Cached FFT Plans for Initialization!", LOG_SENDER, __func__);
        _Plans = cachedPlansIterator->second;
        return componentSize;
    }

    _Logger->WriteLog("Calculating and Caching FFT Plans for Cache Key (%s)...", LOG_SENDER, __func__, plansKeyC);
    fftw_iodim dim;
    dim.n = static_cast<int>(signalSize);
    dim.is = dim.os = 1;

    sample* outputSignal = Allocate(signalSize);
    sample* freqReal = Allocate(componentSize);
    sample* freqImag = Allocate(componentSize);

    _Plans.Real2ComplexPlan = Fftw3PlanReal2Complex(1, &dim, 0, nullptr, outputSignal, freqReal, freqImag, FFTW_MEASURE);
    _Plans.Complex2RealPlan = Fftw3PlanComplex2Real(1, &dim, 0, nullptr, freqReal, freqImag, outputSignal, FFTW_MEASURE);
    _PlansCache->insert( { plansKey, _Plans } );
    _Logger->WriteLog("Successfully Calculated and Cached FFT Plans for Cache Key (%s).", LOG_SENDER, __func__, plansKeyC);

    Deallocate(outputSignal);
    Deallocate(freqReal);
    Deallocate(freqImag);

    _Logger->WriteLog("Successfully initialized FFTW Interface!", LOG_SENDER, __func__);
    return componentSize;
}

float* FftwInterface::Allocate(size_t size) {
    return Fftw3Allocate(size);
}

void FftwInterface::Deallocate(sample* data) {
    Fftw3Free(data);
}

void FftwInterface::SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) {
    Fftw3ExecuteReal2Complex(_Plans.Real2ComplexPlan, signal->GetData(), components->GetRealData(),
                             components->GetImagData());
}

void FftwInterface::ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) {
    Fftw3ExecuteComplex2Real(_Plans.Complex2RealPlan, components->GetRealData(),
                             components->GetImagData(), signal->GetData());
}
