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

#include "kfr_interface.h"

using namespace kfr;

KfrInterface::KfrInterface(LoggerBase* logger)
    : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _Initialized = false;
}

KfrInterface::~KfrInterface() {
    Dispose();
}

size_t KfrInterface::Initialize(size_t signalSize, size_t componentSize) {
    _Logger->WriteLog("Initializing KFR Interface...", LOG_SENDER, __func__);
    Dispose();

    componentSize = FftInterfaceBase::Initialize(signalSize, componentSize);
    if (signalSize < 1 && componentSize < 1) {
        std::string msg = "Invalid Signal and Component Sizes provided!";
        _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
        throw std::runtime_error(msg);
    }

    _Logger->WriteLog("Getting KFR Plan for Signal Size (%d)...", LOG_SENDER, __func__, signalSize);
    _Plan = dft_cache::instance().getreal(ctype<sample>, signalSize);

    _Logger->WriteLog("Initializing KFR Buffers for Component Size (%d)...", LOG_SENDER, __func__, componentSize);
    _ComponentsBuffer = new univector<complex<sample>>(componentSize);
    _TempBuffer = new univector<u8>((u8)_Plan->temp_size);

    _Initialized = true;
    _Logger->WriteLog("Successfully initialized KFR Interface!", LOG_SENDER, __func__);
    return componentSize;
}

void KfrInterface::SignalToComponents(AudioBuffer& signal, AudioComponentsBuffer& components) {
    complex<sample>* componentsData = _ComponentsBuffer->data();
    _Plan->execute(componentsData, signal.GetData(), _TempBuffer->data());

    sample* realComponents = components.GetRealBuffer()->GetData();
    sample* imagComponents = components.GetImagBuffer()->GetData();
    for (int componentCounter = 0; componentCounter < components.GetLength(); componentCounter++) {
        realComponents[componentCounter] = componentsData[componentCounter].re;
        imagComponents[componentCounter] = componentsData[componentCounter].im;
    }
}

void KfrInterface::ComponentsToSignal(AudioComponentsBuffer& components, AudioBuffer& signal) {
    complex<sample>* componentsData = _ComponentsBuffer->data();
    sample* realComponents = components.GetRealBuffer()->GetData();
    sample* imagComponents = components.GetImagBuffer()->GetData();
    for (int componentCounter = 0; componentCounter < components.GetLength(); componentCounter++) {
        componentsData[componentCounter].re = realComponents[componentCounter];
        componentsData[componentCounter].im = imagComponents[componentCounter];
    }

    _Plan->execute(signal.GetData(), componentsData, _TempBuffer->data());
}

void KfrInterface::Dispose() {
    if (!_Initialized) {
        return;
    }

    delete _ComponentsBuffer;
    delete _TempBuffer;
    _Initialized = false;
}
