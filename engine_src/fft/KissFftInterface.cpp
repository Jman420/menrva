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

#include "KissFftInterface.h"
#include "KissFftFunctions.h"

KissFftPlanCache* KissFftInterface::_PlansCache = new KissFftPlanCache();

KissFftInterface::KissFftInterface(LogWriterBase* logger)
    : LogProducer(logger, __PRETTY_FUNCTION__) {
    _Initialized = false;
}

KissFftInterface::~KissFftInterface() {
    Dispose();
}

size_t KissFftInterface::Initialize(size_t signalSize, size_t componentSize) {
    _Logger->WriteLog("Initializing KissFFT Interface...", LOG_SENDER, __func__);
    Dispose();

    componentSize = FftInterfaceBase::Initialize(signalSize, componentSize);
    if (signalSize < 1 && componentSize < 1) {
        std::string msg = "Invalid Signal and Component Sizes provided!";
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error(msg);
    }

    _Logger->WriteLog("Calculating FFT Plans Cache Key for Signal Size (%d) and Component Size (%d)...", LOG_SENDER, __func__, signalSize, componentSize);
    std::string plansKey = std::to_string(signalSize) + "x" + std::to_string(componentSize);
    const char* plansKeyC = plansKey.c_str();

    _Logger->WriteLog("Checking FFT Plans Cache for Key (%s)...", LOG_SENDER, __func__, plansKeyC);
    auto cachedPlansIterator = _PlansCache->find(plansKey);
    if (cachedPlansIterator != _PlansCache->end()) {
        _Logger->WriteLog("Successfully found Cached FFT Plans for Initialization!", LOG_SENDER, __func__);
        _Plans = cachedPlansIterator->second;
    }
    else {
        _Logger->WriteLog("Calculating and Caching FFT Plans for Cache Key (%s)...", LOG_SENDER, __func__, plansKeyC);
        _Plans.RealToComplexPlan = KissFftCreatePlan(signalSize, 0, nullptr, 0);
        _Plans.ComplexToRealPlan = KissFftCreatePlan(signalSize, 1, nullptr, 0);
        _PlansCache->insert({plansKey, _Plans});
        _Logger->WriteLog("Successfully Calculated and Cached FFT Plans for Cache Key (%s).", LOG_SENDER, __func__, plansKeyC);
    }
    
    _Logger->WriteLog("Allocating KissFFT Components Buffer...", LOG_SENDER, __func__);
    _ComponentsBuffer = new kiss_fft_cpx[componentSize];

    _Initialized = true;
    _Logger->WriteLog("Successfully initialized KissFFT Interface!", LOG_SENDER, __func__);
    return componentSize;
}

void KissFftInterface::SignalToComponents(AudioBuffer& signal, AudioComponentsBuffer& components) {
    KissFftRealToComplex(_Plans.RealToComplexPlan, signal.GetData(), _ComponentsBuffer);

    sample* realComponents = components.GetRealData();
    sample* imagComponents = components.GetImagData();
    for (int componentCounter = 0; componentCounter < components.GetLength(); componentCounter++) {
        realComponents[componentCounter] = _ComponentsBuffer[componentCounter].r;
        imagComponents[componentCounter] = _ComponentsBuffer[componentCounter].i;
    }
}

void KissFftInterface::ComponentsToSignal(AudioComponentsBuffer& components, AudioBuffer& signal) {
    sample* realComponents = components.GetRealData();
    sample* imagComponents = components.GetImagData();
    for (int componentCounter = 0; componentCounter < components.GetLength(); componentCounter++) {
        _ComponentsBuffer[componentCounter].r = realComponents[componentCounter];
        _ComponentsBuffer[componentCounter].i = imagComponents[componentCounter];
    }

    KissFftComplexToReal(_Plans.ComplexToRealPlan, _ComponentsBuffer, signal.GetData());
}

void KissFftInterface::Dispose() {
    if (!_Initialized) {
        return;
    }

    delete[] _ComponentsBuffer;
    _Initialized = false;
}
