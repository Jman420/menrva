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

#include "single_channel_effects_bundle.h"

SingleChannelEffectsBundle::SingleChannelEffectsBundle() {
    ServiceLocator serviceLocator = *new ServiceLocator();
    _BassBoost = new BassBoost(serviceLocator.GetLogger(), serviceLocator.GetFirGenerator(), serviceLocator.GetConvolver());
    _Equalizer = new Equalizer(serviceLocator.GetLogger());

    _Effects = new SingleChannelEffectBase*[LENGTH];
    _Effects[static_cast<uint8_t>(SingleChannelEffectIndexes::BASS_BOOST)] = _BassBoost;
    _Effects[static_cast<uint8_t>(SingleChannelEffectIndexes::EQUALIZER)] = _Equalizer;
}

SingleChannelEffectsBundle::~SingleChannelEffectsBundle() {
    delete[] _Effects;
}

void SingleChannelEffectsBundle::ResetBuffers(sample sampleRate, size_t audioFrameLength) {
    for (int effectCounter = 0; effectCounter < LENGTH; effectCounter++) {
        EffectBase& effect = *_Effects[effectCounter];
        effect.ResetBuffers(sampleRate, audioFrameLength);
    }
}

BassBoost* SingleChannelEffectsBundle::GetBassBoost() {
    return _BassBoost;
}

Equalizer* SingleChannelEffectsBundle::GetEqualizer() {
    return _Equalizer;
}

SingleChannelEffectBase* SingleChannelEffectsBundle::operator[](uint8_t index) const {
    if (index > LENGTH) {
        throw std::runtime_error("Index out of bounds.");
    }

    return _Effects[index];
}
