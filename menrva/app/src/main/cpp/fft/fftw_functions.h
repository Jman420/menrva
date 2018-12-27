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

#ifndef MENRVA_FFTW_FUNCTIONS_H
#define MENRVA_FFTW_FUNCTIONS_H

#include <fftw3.h>
#include "../audio/sample.h"

#ifdef MENRVA_DOUBLE_PRECISION
typedef fftw_plan FftwPlan;
typedef fftw_plan (fftwPlanFunc)(int, const fftw_iodim*, int, const fftw_iodim*, double*, double*, double*, unsigned int);
static fftwPlanFunc* Fftw3PlanReal2ComplexFunc = fftw_plan_guru_split_dft_r2c;
static fftwPlanFunc* Fftw3PlanComplex2RealFunc = fftw_plan_guru_split_dft_c2r;
#else
typedef fftwf_plan FftwPlan;
typedef fftwf_plan (fftwPlanFunc)(int, const fftw_iodim*, int, const fftw_iodim*, float*, float*, float*, unsigned int);
static fftwPlanFunc* Fftw3PlanReal2ComplexFunc = fftwf_plan_guru_split_dft_r2c;
static fftwPlanFunc* Fftw3PlanComplex2RealFunc = fftwf_plan_guru_split_dft_c2r;
#endif

struct fftw_plan_pair {
    FftwPlan Real2ComplexPlan,
             Complex2RealPlan;
};

#endif //MENRVA_FFTW_FUNCTIONS_H
