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

#ifndef MENRVA_WAVE_GENERATOR_CONSTANTS_H
#define MENRVA_WAVE_GENERATOR_CONSTANTS_H

#include <cmath>
#include "../audio/sample.h"

class WaveGeneratorConstants {
public:
    static constexpr sample PI = (sample)M_PI,
                            PI2 = (sample)(M_PI * 2.0),
                            ONE = (sample)1.0,
                            ONE_HALF = (sample)0.5,
                            HAMMING_054 = (sample)0.54,
                            HAMMING_046 = (sample)0.46;
};

#endif //MENRVA_WAVE_GENERATOR_CONSTANTS_H
