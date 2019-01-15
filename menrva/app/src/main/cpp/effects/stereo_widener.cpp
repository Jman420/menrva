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

#include "stereo_widener.h"

const std::string StereoWidener::EFFECT_NAME = "StereoWidener";

StereoWidener::StereoWidener(LoggerBase* logger, ServiceLocator* serviceLocator)
        : EffectBase(EFFECT_NAME),
          LoggingBase(logger, __PRETTY_FUNCTION__) {

}

void StereoWidener::Process(AudioBuffer* input, AudioBuffer* output) {
    // TODO : Implement Stereo Widener Effect
}

void StereoWidener::ResetConfig(effect_config_t* bufferConfig) {
    // TODO : Implement Default Configuration for StereoWidener Effect
}

void StereoWidener::ConfigureSetting(char* settingName, void* value) {
    // TODO : Implement Logic for Configuring the StereoWidener Effect
}
