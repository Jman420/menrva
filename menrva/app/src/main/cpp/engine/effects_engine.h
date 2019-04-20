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

#ifndef MENRVA_EFFECTS_ENGINE_H
#define MENRVA_EFFECTS_ENGINE_H

#include "../abstracts/LoggingBase.h"
#include "../audio/AudioInputBuffer.h"
#include "../audio/AudioOutputBuffer.h"
#include "../tools/service_locator.h"
#include "../effects/single_channel_effects_bundle.h"
#include "../effects/multi_channel_effects_bundle.h"

enum MenrvaEngineStatus {
    MENRVA_ENGINE_UNCONFIGURED,
    MENRVA_ENGINE_DISABLED,
    MENRVA_ENGINE_ENABLED,
};

class MenrvaEffectsEngine : public LoggingBase {
public:
    MenrvaEngineStatus _EngineStatus;

    MenrvaEffectsEngine(LoggerBase* logger, FftInterfaceBase* fftEngine, ServiceLocator* serviceLocator);
    ~MenrvaEffectsEngine();

    int SetBufferConfig(uint32_t channelLength, sample sampleRate, size_t frameLength);
    int Process(AudioInputBuffer& inputBuffer, AudioOutputBuffer& outputBuffer);
    void ResetBuffers(sample sampleRate);
    void SetEffectEnabled(uint8_t effectIndex, bool enabled);
    void ConfigureEffectSetting(uint8_t effectIndex, char* settingName, void* value);
    SingleChannelEffectsBundle* GetSingleChannelEffectsBundle();
    MultiChannelEffectsBundle* GetMultiChannelEffectsBundle();

private:
    ServiceLocator* _ServiceLocator;
    FftInterfaceBase* _FftEngine;

    uint32_t _ChannelLength;
    SingleChannelEffectsBundle* _SingleChannelEffects;
    MultiChannelEffectsBundle* _MultiChannelEffects;
    AudioBuffer* _InputAudioFrame;
    AudioBuffer* _OutputAudioFrame;

    void ProcessInputAudioFrame();
    size_t ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer& outputBuffer);
};

#endif //MENRVA_EFFECTS_ENGINE_H
