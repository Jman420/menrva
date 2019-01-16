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

#include "fft_interface_base.h"

FftInterfaceBase::FftInterfaceBase(LoggerBase* logger)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _SignalSize = 0;
    _ComponentSize = 0;
}

size_t FftInterfaceBase::GetSignalSize() {
    return _SignalSize;
}

size_t FftInterfaceBase::GetComponentSize() {
    return _ComponentSize;
}

size_t FftInterfaceBase::Initialize(size_t signalSize, size_t componentSize) {
    _Logger->WriteLog("Setting FFT Signal & Component Sizes...", LOG_SENDER, __func__);
    if (signalSize > 0 && componentSize < 1) {
        componentSize = signalSize / 2 + 1;
        _Logger->WriteLog("Calculated Component Size (%d) from Signal Size (%d).", LOG_SENDER, __func__, componentSize, signalSize);
    }

    _SignalSize = signalSize;
    _ComponentSize = componentSize;

    _Logger->WriteLog("Successfully set FFT Signal Size (%d) and Component Size (%d)!", LOG_SENDER, __func__, signalSize, componentSize);
    return componentSize;
}

size_t FftInterfaceBase::Initialize(size_t signalSize) {
    return Initialize(signalSize, 0);
}
