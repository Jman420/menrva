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

#ifndef MENRVA_FFTW_INTERFACE_H
#define MENRVA_FFTW_INTERFACE_H

#include <map>
#include <fftw3.h>
#include "../abstracts/fft_interface_base.h"

struct fftw_plan_pair {
    fftwf_plan Real2ComplexPlan,
               Complex2RealPlan;
};
typedef std::map<std::string, fftw_plan_pair> PlanCache;

class FFTW_Interface : public FFTInterfaceBase {
public:
    FFTW_Interface(unsigned int signalSize, unsigned int componentSize);
    int Initialize(unsigned int signalSize, unsigned int componentSize = 0) override;
    void SignalToComponents(float* signal, float* realComponents, float* imagComponents) override;
    void ComponentsToSignal(float* signal, float* realComponents, float* imagComponents) override;
    float* Allocate(size_t size) override;
    void Deallocate(float* data) override;

protected:
    static PlanCache* _PlansCache;

    fftw_plan_pair _Plans;
};

#endif //MENRVA_FFTW_INTERFACE_H
