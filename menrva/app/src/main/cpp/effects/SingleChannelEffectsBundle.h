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

#ifndef MENRVA_EFFECTS_BUNDLE_H
#define MENRVA_EFFECTS_BUNDLE_H

#include "BassBoost.h"
#include "Equalizer.h"

enum class SingleChannelEffectIndexes
        : uint8_t {
    BASS_BOOST = 0,
    EQUALIZER,

    // NOTE : Unused Enum State to provide Enum Length; must be Final Enum Entry
    Length
};

class SingleChannelEffectsBundle {
public:
    static const uint8_t LENGTH = static_cast<const uint8_t>(SingleChannelEffectIndexes::Length);

    SingleChannelEffectsBundle();
    ~SingleChannelEffectsBundle();

    void ResetBuffers(sample sampleRate, size_t audioFrameLength);

    BassBoost* GetBassBoost();
    Equalizer* GetEqualizer();

    SingleChannelEffectBase* operator[](uint8_t index) const;

private:
    BassBoost* _BassBoost;
    Equalizer* _Equalizer;
    SingleChannelEffectBase** _Effects;
};

#endif //MENRVA_EFFECTS_BUNDLE_H
