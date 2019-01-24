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

#include "bass_boost.h"

const std::string BassBoost::EFFECT_NAME = "BassBoost";

BassBoost::BassBoost(LoggerBase* logger, ServiceLocator* serviceLocator)
        : EffectBase(EFFECT_NAME),
          LoggingBase(logger, __PRETTY_FUNCTION__) {
    _FirGenerator = serviceLocator->GetFirGenerator();
    _Convolver = serviceLocator->GetConvolver();
}

BassBoost::~BassBoost() {
    delete _FirGenerator;
    delete _Convolver;
}

void BassBoost::Process(AudioBuffer* input, AudioBuffer* output) {
    if (!Enabled) {
        return;
    }

    _Convolver->Process(input, output);
}

void BassBoost::ResetConfig(effect_config_t* bufferConfig) {
    // BEGIN DEBUG
    sample sampleRate = bufferConfig->inputCfg.samplingRate,
           centerFreq = 60.0,
           freqTransition = 80.0,
           strength = 6.0;

    size_t filterSize = 4096,
           sampleSize = 4;
    sample frequencySamples[] = { 0, (sample)((centerFreq * 2.0) / sampleRate), (sample)((centerFreq * 2.0 + freqTransition) / sampleRate), 1.0 },
           amplitudeSamples[] = { (sample)(pow(10.0, strength / 20.0)), (sample)(pow(10.0, strength / 20.0)), 1.0, 1.0 };

    AudioBuffer* impulseFilter = _FirGenerator->Calculate(filterSize, frequencySamples, amplitudeSamples, sampleSize);
    _Convolver->Initialize(MENRVA_DSP_FRAME_LENGTH, impulseFilter);
    Enabled = true;
    // END DEBUG
}

void BassBoost::ConfigureSetting(char* settingName, void* value) {
    // TODO : Implement Logic for Configuring the BassBoost Effect
}
