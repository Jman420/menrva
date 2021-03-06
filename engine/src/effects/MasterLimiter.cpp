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

#include "MasterLimiter.h"

const std::string MasterLimiter::EFFECT_NAME = "MasterLimiter";

MasterLimiter::MasterLimiter(LogWriterBase* logger)
        : MultiChannelEffectBase(EFFECT_NAME),
          LogProducer(logger, __PRETTY_FUNCTION__) {}

void MasterLimiter::Process(AudioBuffer* inputBuffers, AudioBuffer* outputBuffers, uint32_t channelLength) {
    // TODO : Implement Master Limiter (floor & ceiling limiters; pan control; gain control)
    
    // BEGIN DEBUG
    for (int channelCounter = 0; channelCounter < channelLength; channelCounter++) {
        AudioBuffer& channelInput = inputBuffers[channelCounter];
        AudioBuffer& channelOutput = outputBuffers[channelCounter];

        for (int sampleCounter = 0; sampleCounter < channelInput.GetLength(); sampleCounter++) {
            channelOutput[sampleCounter] = channelInput[sampleCounter];
        }
    }
    // END DEBUG
}

void MasterLimiter::ConfigureSetting(char* settingName, void* value) {
    // TODO : Implement Logic for Configuring the MasterLimiter Effect
}
