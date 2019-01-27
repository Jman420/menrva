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

#include "../config.h"
#include "service_locator.h"
#include "android_logger.h"

#ifdef MENRVA_USE_FFTW
    #include "../fft/fftw_interface.h"
#elif defined(MENRVA_USE_KISSFFT)
    #include "../fft/kissfft_interface.h"
#elif defined(MENRVA_USE_KFR)
    #include "../fft/kfr_interface.h"
#endif

LoggerBase* ServiceLocator::_Logger = new AndroidLogger();

LoggerBase* ServiceLocator::GetLogger() {
    return _Logger;
}

FftInterfaceBase* ServiceLocator::GetFftEngine() {
    FftInterfaceBase* returnValue = nullptr;

#ifdef MENRVA_USE_FFTW
    returnValue = new FftwInterface(GetLogger());
#elif defined(MENRVA_USE_KISSFFT)
    returnValue = new KissFftInterface(GetLogger());
#elif defined(MENRVA_USE_KFR)
    returnValue = new KfrInterface(GetLogger());
#endif

    return returnValue;
}

FirGenerator* ServiceLocator::GetFirGenerator() {
    return new FirGenerator(GetLogger(), GetFftEngine());
}

ConvolutionOperationsBase* ServiceLocator::GetConvolutionOperations() {
    return new ConvolutionOperations(GetLogger());
}

Convolver* ServiceLocator::GetConvolver() {
    return new Convolver(GetLogger(), GetFftEngine(), GetConvolutionOperations());
}
