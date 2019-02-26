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

#include "multi_channel_effects_bundle.h"

MultiChannelEffectsBundle::MultiChannelEffectsBundle(ServiceLocator& serviceLocator) {
    _StereoWidener = new StereoWidener(serviceLocator.GetLogger());
    _MasterLimiter = new MasterLimiter(serviceLocator.GetLogger());

    _Effects = new MultiChannelEffectBase*[LENGTH];
    _Effects[static_cast<uint8_t>(MultiChannelEffectIndexes::STEREO_WIDENER)] = _StereoWidener;
    _Effects[static_cast<uint8_t>(MultiChannelEffectIndexes::MASTER_LIMITER)] = _MasterLimiter;
}

MultiChannelEffectsBundle::~MultiChannelEffectsBundle() {
    delete[] _Effects;
}

StereoWidener* MultiChannelEffectsBundle::GetStereoWidener() {
    return _StereoWidener;
}

MasterLimiter* MultiChannelEffectsBundle::GetMasterLimiter() {
    return _MasterLimiter;
}

MultiChannelEffectBase* MultiChannelEffectsBundle::operator[](uint8_t index) const {
    if (index > LENGTH) {
        throw std::runtime_error("Index out of bounds.");
    }

    return _Effects[index];
}
