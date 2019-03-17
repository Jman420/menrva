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

BassBoost::BassBoost(LoggerBase* logger, FirGenerator* firGenerator, Convolver* convolver)
        : SingleChannelEffectBase(EFFECT_NAME),
          LoggingBase(logger, __PRETTY_FUNCTION__) {
    _FirGenerator = firGenerator;
    _Convolver = convolver;
}

BassBoost::~BassBoost() {
    delete _FirGenerator;
    delete _Convolver;
}

void BassBoost::Process(AudioBuffer& input, AudioBuffer& output) {
    if (!Enabled) {
        return;
    }

    _Convolver->Process(input, output);
}

void BassBoost::ResetBuffers(sample sampleRate, size_t audioFrameLength) {
    // BEGIN DEBUG
    sample centerFreq = 60.0;
    sample freqTransition = 80.0;
    sample strength = 6.0;

    size_t filterSize = 4097;
    size_t sampleSize = 4;
    sample frequencySamples[] = {0, ((centerFreq * 2.0F) / sampleRate), ((centerFreq * 2.0F + freqTransition) / sampleRate), 1.0F};
    sample amplitudeSamples[] = {(pow(10.0F, strength / 20.0F)), (pow(10.0F, strength / 20.0F)), 1.0F, 1.0F};

    AudioBuffer* impulseFilter = _FirGenerator->Calculate(filterSize, frequencySamples, amplitudeSamples, sampleSize);
    _Convolver->Initialize(audioFrameLength, *impulseFilter, true);
    Enabled = true;
    // END DEBUG

    _Convolver->ResetBuffers();
}

void BassBoost::ConfigureSetting(char* settingName, void* value) {
    // TODO : Implement Logic for Configuring the BassBoost Effect
}
