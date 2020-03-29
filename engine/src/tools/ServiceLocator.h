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

#ifndef MENRVA_SERVICE_LOCATOR_H
#define MENRVA_SERVICE_LOCATOR_H

#include "../log/LogWriterBase.h"
#include "../fft/FftInterfaceBase.h"
#include "../ir/FirGenerator.h"
#include "../convolver/ConvolutionOperationsBase.h"
#include "../convolver/Convolver.h"

class ServiceLocator {
public:
    virtual LogWriterBase* GetLogger() = 0;
    virtual FftInterfaceBase* GetFftEngine();
    virtual FirGenerator* GetFirGenerator();
    virtual ConvolutionOperationsBase* GetConvolutionOperations();
    virtual Convolver* GetConvolver();
};

#endif //MENRVA_SERVICE_LOCATOR_H