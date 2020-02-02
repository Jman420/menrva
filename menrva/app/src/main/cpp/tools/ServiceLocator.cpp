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

#include "../Config.h"
#include "ServiceLocator.h"
#include "AndroidLogger.h"
#include "../fft/KfrInterface.h"
/**#include "../fft/FftwInterface.h"
#include "../fft/KissFftInterface.h"/**/

LoggerBase* ServiceLocator::_Logger = new AndroidLogger();

ServiceLocator::ServiceLocator() {
    _FftEngineType = FftEngineType::KFR;
}

LoggerBase* ServiceLocator::GetLogger() {
    return _Logger;
}

FftInterfaceBase* ServiceLocator::GetFftEngine() {
    return new KfrInterface(GetLogger());

    /**switch (_FftEngineType) {
        case FftEngineType::KFR:
            return new KfrInterface(GetLogger());

        case FftEngineType::FFTW:
            return new FftwInterface(GetLogger());

        case FftEngineType::KISS_FFT:
            return new KissFftInterface(GetLogger());
    }

    throw std::runtime_error("Invalid FFT Engine Type.");/**/
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
