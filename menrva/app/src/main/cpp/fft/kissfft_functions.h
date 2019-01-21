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

#ifndef MENRVA_KISSFFT_FUNCTIONS_H
#define MENRVA_KISSFFT_FUNCTIONS_H

#include <kiss_fftr.h>
#include "../config.h"

typedef kiss_fftr_cfg KissFftPlan;
typedef kiss_fftr_cfg (kissFftPlanFunc)(int, int, void*, size_t*);
static kissFftPlanFunc* KissFftCreatePlan = kiss_fftr_alloc;

#ifdef MENRVA_DOUBLE_PRECISION
    typedef void (kissFftExecuteForward)(kiss_fftr_cfg, const double*, kiss_fft_cpx*);
    typedef void (kissFftExecuteInverse)(kiss_fftr_cfg, const kiss_fft_cpx*, double*);

    static kissFftExecuteForward* KissFftRealToComplex = kiss_fftr;
    static kissFftExecuteInverse* KissFftComplexToReal = kiss_fftri;

#else
    typedef void (kissFftExecuteForward)(kiss_fftr_cfg, const float*, kiss_fft_cpx*);
    typedef void (kissFftExecuteInverse)(kiss_fftr_cfg, const kiss_fft_cpx*, float*);

    static kissFftExecuteForward* KissFftRealToComplex = kiss_fftr;
    static kissFftExecuteInverse* KissFftComplexToReal = kiss_fftri;

#endif

struct kissfft_plan_pair {
    KissFftPlan RealToComplexPlan,
                ComplexToRealPlan;
};

#endif //MENRVA_KISSFFT_FUNCTIONS_H
