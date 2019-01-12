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

/* MENRVA_DOUBLE_PRECISION - Uncomment the following define to switch Menrva to use double instead
 * of float types for all calculations.
 *
 * NOTE : At the time of writing the Android AudioFlinger Service expects float32 as the largest
 * type it handles.  See the 'aosp/README.md' file for AOSP Source Files worth referencing, or the
 * included structures copied from those files in 'aosp/aosp_audio_effect_defs.h'.  The relevant
 * structure in this case is 'audio_buffer_t'.
 */
//#define MENRVA_DOUBLE_PRECISION

/* DSP_FRAME_SIZE - Defines the number of audio samples in a single audio frame for processing.
 *
 * Recommended value : 1024
 */
#define DSP_FRAME_SIZE 1024

#endif //MENRVA_CONFIG_H
