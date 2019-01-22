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

#include "kissfft_interface.h"
#include "kissfft_functions.h"

KissFftPlanCache* KissFftInterface::_PlansCache = new KissFftPlanCache();

KissFftInterface::KissFftInterface(LoggerBase* logger) : FftInterfaceBase(logger) { }

size_t KissFftInterface::Initialize(size_t signalSize, size_t componentSize) {
    _Logger->WriteLog("Initializing KissFFT Interface...", LOG_SENDER, __func__);
    componentSize = FftInterfaceBase::Initialize(signalSize, componentSize);
    if (signalSize < 1 && componentSize < 1) {
        return componentSize;
    }

    _Logger->WriteLog("Calculating FFT Plans Cache Key for Signal Size (%d) and Component Size (%d)...", LOG_SENDER, __func__, signalSize, componentSize);
    std::string plansKey = std::to_string(signalSize) + "x" + std::to_string(componentSize);
    const char* plansKeyC = plansKey.c_str();

    _Logger->WriteLog("Checking FFT Plans Cache for Key (%s)...", LOG_SENDER, __func__, plansKeyC);
    KissFftPlanCache::iterator cachedPlansIterator = _PlansCache->find(plansKey);
    if (cachedPlansIterator != _PlansCache->end()) {
        _Logger->WriteLog("Successfully found Cached FFT Plans for Initialization!", LOG_SENDER, __func__);
        _Plans = cachedPlansIterator->second;
        return componentSize;
    }

    _Logger->WriteLog("Calculating and Caching FFT Plans for Cache Key (%s)...", LOG_SENDER, __func__, plansKeyC);
    _Plans.RealToComplexPlan = KissFftCreatePlan(signalSize, 0, 0, 0);
    _Plans.ComplexToRealPlan = KissFftCreatePlan(componentSize, 1, 0, 0);
    _PlansCache->insert( { plansKey, _Plans } );
    _Logger->WriteLog("Successfully Calculated and Cached FFT Plans for Cache Key (%s).", LOG_SENDER, __func__, plansKeyC);

    _Logger->WriteLog("Allocating KissFFT Components Buffer...", LOG_SENDER, __func__);
    _ComplexValues = (kiss_fft_cpx*)calloc(componentSize, sizeof(kiss_fft_cpx));

    _Logger->WriteLog("Successfully initialized KissFFT Interface!", LOG_SENDER, __func__);
    return componentSize;
}

void KissFftInterface::SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) {
    KissFftRealToComplex(_Plans.RealToComplexPlan, signal->GetData(), _ComplexValues);

    sample* realComponents = components->GetRealBuffer()->GetData();
    sample* imagComponents = components->GetImagBuffer()->GetData();
    for (int componentCounter = 0; componentCounter < components->GetLength(); componentCounter++) {
        kiss_fft_cpx* kissFftComponents = &_ComplexValues[componentCounter];
        realComponents[componentCounter] = kissFftComponents->r;
        imagComponents[componentCounter] = kissFftComponents->i;
    }
}

void KissFftInterface::ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) {
    sample* realComponents = components->GetRealBuffer()->GetData();
    sample* imagComponents = components->GetImagBuffer()->GetData();
    for (int componentCounter = 0; componentCounter < components->GetLength(); componentCounter++) {
        kiss_fft_cpx* kissFftComponents = &_ComplexValues[componentCounter];
        kissFftComponents->r = realComponents[componentCounter];
        kissFftComponents->i = imagComponents[componentCounter];
    }

    KissFftComplexToReal(_Plans.ComplexToRealPlan, _ComplexValues, signal->GetData());
}

sample* KissFftInterface::Allocate(size_t size) {
    sample* buffer = (sample*)malloc(size);
    memset(buffer, 0, size);
    return buffer;
}

void KissFftInterface::Deallocate(sample* data) {
    free(data);
}