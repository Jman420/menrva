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

#ifndef MENRVA_CONFIG_H
#define MENRVA_CONFIG_H

/* DSP_FRAME_LENGTH - Defines the number of audio samples in a single audio frame for processing.
 *
 * Recommended value : 1024
 */
#define DSP_FRAME_LENGTH 1024

/* USE_FFTW - Indicates to use FFTW in the current build
 * USE_KISSFFT - Indicates to use KissFFT in the current build
 * USE_KFR - Indicates to use KFR in the current build
 *
 * NOTE : Only one of the above Directives should be defined at once.
 */
#define USE_FFTW
//#define USE_KISSFFT
//#define USE_KFR

/* MENRVA_DOUBLE_PRECISION - Uncomment the following define to switch Menrva to use double instead
 * of float types for all calculations.
 *
 * NOTE : At the time of writing the Android AudioFlinger Service expects float32 as the largest
 * type it handles.  See the 'aosp/README.md' file for AOSP Source Files worth referencing, or the
 * included structures copied from those files in 'aosp/aosp_audio_effect_defs.h'.  The relevant
 * structure in this case is 'audio_buffer_t'.
 */
//#define MENRVA_DOUBLE_PRECISION



/*
 * Code below this section is validation for the above Configuration to prevent undefined behavior.
 * ----------------------------DO NOT MODIFY CODE BELOW THIS LINE---------------------------------
 */

#if !defined(USE_FFTW) && !defined(USE_KISSFFT) && !defined(USE_KFR)
    #error "No FFT Engine Implementation Declared.  One of the following Preprocessory Directives must be defined : USE_FFTW, USE_KISSFFT or USE_KFR."
#endif

#ifdef USE_FFTW
    #if (defined(USE_KISSFFT) || defined(USE_KFR)) && !defined(FFT_ENGINE_ERROR)
        #define FFT_ENGINE_ERROR 1
    #endif
#endif

#ifdef USE_KISSFFT
    #if (defined(USE_FFTW) || defined(USE_KFR)) && !defined(FFT_ENGINE_ERROR)
        #define FFT_ENGINE_ERROR 1
    #endif
#endif

#ifdef USE_KFR
    #if (defined(USE_FFTW) || defined(USE_KISSFFT)) && !defined(FFT_ENGINE_ERROR)
        #define FFT_ENGINE_ERROR 1
    #endif
#endif

#ifdef FFT_ENGINE_ERROR
    #error "Multiple FFT Engine Implementations Declared.  Only one of the following Preprocessor Directives can be defined : USE_FFTW, USE_KISSFFT or USE_KFR."
#endif

#endif //MENRVA_CONFIG_H
