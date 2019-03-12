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

#ifndef MENRVA_MULTI_CHANNEL_EFFECTS_BUNDLE_H
#define MENRVA_MULTI_CHANNEL_EFFECTS_BUNDLE_H

#include <cstdint>
#include "stereo_widener.h"
#include "master_limiter.h"
#include "../tools/service_locator.h"

enum class MultiChannelEffectIndexes : uint8_t {
    STEREO_WIDENER = 0,
    MASTER_LIMITER,  // Must be the last Effect Entry

    // NOTE : Unused Enum State to provide Enum Length; must be Final Enum Entry
    Length
};

class MultiChannelEffectsBundle {
public:
    static const uint8_t LENGTH = static_cast<const uint8_t>(MultiChannelEffectIndexes::Length);

    explicit MultiChannelEffectsBundle(ServiceLocator& serviceLocator);
    ~MultiChannelEffectsBundle();

    void ResetBuffers(sample sampleRate, size_t audioFrameLength);
    StereoWidener* GetStereoWidener();
    MasterLimiter* GetMasterLimiter();

    MultiChannelEffectBase* operator[](uint8_t index) const;

private:
    StereoWidener* _StereoWidener;
    MasterLimiter* _MasterLimiter;
    MultiChannelEffectBase** _Effects;
};

#endif //MENRVA_MULTI_CHANNEL_EFFECTS_BUNDLE_H
