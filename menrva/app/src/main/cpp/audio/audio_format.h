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

#ifndef MENRVA_AUDIO_FORMAT_H
#define MENRVA_AUDIO_FORMAT_H

#include "sample.h"

enum AudioFormat {
    PCM_16 = 0x1u,
    PCM_32 = 0x3u,
    PCM_Float = 0x5u,
    Sample,
};

const sample PCM16_MAX_VALUE = 32767.0f,
             PCM16_MIN_VALUE = -32767.0f,
             PCM32_MAX_VALUE = 2147483647.0f,
             PCM32_MIN_VALUE = -2147483647.0f,
             PCM_FLOAT_MAX_VALUE = 1.0f,
             PCM_FLOAT_MIN_VALUE = -1.0f;

#endif //MENRVA_AUDIO_FORMAT_H