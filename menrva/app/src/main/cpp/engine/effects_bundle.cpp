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

#include "effects_bundle.h"

EffectsBundle::EffectsBundle() {
    ServiceLocator serviceLocator = *new ServiceLocator();
    _BassBoost = new BassBoost(serviceLocator.GetLogger(), serviceLocator.GetFirGenerator(), serviceLocator.GetConvolver());
    _Equalizer = new Equalizer(serviceLocator.GetLogger());
    _StereoWidener = new StereoWidener(serviceLocator.GetLogger());

    _Effects = new EffectBase*[LENGTH];
    _Effects[EffectIndexes::BASS_BOOST] = _BassBoost;
    _Effects[EffectIndexes::EQUALIZER] = _Equalizer;
    _Effects[EffectIndexes::STEREO_WIDENER] = _StereoWidener;
}

EffectsBundle::~EffectsBundle() {
    delete _BassBoost;
    delete _Equalizer;
    delete _StereoWidener;

    delete[] _Effects;
}

BassBoost* EffectsBundle::GetBassBoost() {
    return _BassBoost;
}

Equalizer* EffectsBundle::GetEqualizer() {
    return _Equalizer;
}

StereoWidener* EffectsBundle::GetStereoWidener() {
    return _StereoWidener;
}

EffectBase* EffectsBundle::operator[](uint8_t index) const {
    if (index > LENGTH) {
        throw std::runtime_error("Index out of bounds.");
    }

    return _Effects[index];
}
