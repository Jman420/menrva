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
#include "../Config.h"

typedef float* (fftwAllocateFunc)(size_t);
typedef void (fftwFreeFunc)(void*);

#ifdef MENRVA_DOUBLE_PRECISION
    typedef fftw_plan FftwPlan;
    typedef fftw_plan (fftwPlanFunc)(int, const fftw_iodim*, int, const fftw_iodim*, double*, double*, double*, unsigned int);
    typedef void (fftwExecuteFunc)(const fftwf_plan, double*, double*, double*);

    static fftwPlanFunc* Fftw3PlanReal2Complex = fftw_plan_guru_split_dft_r2c;
    static fftwExecuteFunc* Fftw3ExecuteReal2Complex = fftw_execute_split_dft_r2c;
    static fftwPlanFunc* Fftw3PlanComplex2Real = fftw_plan_guru_split_dft_c2r;
    static fftwExecuteFunc* Fftw3ExecuteComplex2Real = fftw_execute_split_dft_c2r;

    static fftwAllocate* Fftw3Allocate = fftw_alloc_real;
    static fftwFreeFunc* Fftw3Free = fftw_free;

#else
    typedef fftwf_plan FftwPlan;
    typedef fftwf_plan (fftwPlanFunc)(int, const fftw_iodim*, int, const fftw_iodim*, float*, float*, float*, unsigned int);
    typedef void (fftwExecuteFunc)(const fftwf_plan, float*, float*, float*);

    static fftwPlanFunc* Fftw3PlanReal2Complex = fftwf_plan_guru_split_dft_r2c;
    static fftwExecuteFunc* Fftw3ExecuteReal2Complex = fftwf_execute_split_dft_r2c;
    static fftwPlanFunc* Fftw3PlanComplex2Real = fftwf_plan_guru_split_dft_c2r;
    static fftwExecuteFunc* Fftw3ExecuteComplex2Real = fftwf_execute_split_dft_c2r;

    static fftwAllocateFunc* Fftw3Allocate = fftwf_alloc_real;
    static fftwFreeFunc* Fftw3Free = fftwf_free;

#endif

struct fftw_plan_pair {
    FftwPlan Real2ComplexPlan,
             Complex2RealPlan;
};

#endif //MENRVA_FFTW_FUNCTIONS_H
