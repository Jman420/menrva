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

#include <menrvaEngine/host/ServiceLocator.h>
#include <menrvaEngine/ir/WaveGenerator.h>
#include "engine_debugging.h"
#include "test_helper.h"
#include "../../../main/cpp/aosp/aosp_audio_effect_defs.h"
#include "../../../main/cpp/AndroidInterface.h"

void EngineDebugging::ProcessPipeline(uint32_t channelMask) {
    test_params params;
    effect_config_t menrvaEffectConfig;
    menrvaEffectConfig.inputCfg.samplingRate = static_cast<uint32_t>(params.SampleRate);
    menrvaEffectConfig.inputCfg.format = AudioFormat::PCM_16;
    menrvaEffectConfig.inputCfg.channels = channelMask;

    menrvaEffectConfig.outputCfg.samplingRate = menrvaEffectConfig.inputCfg.samplingRate;
    menrvaEffectConfig.outputCfg.format = menrvaEffectConfig.inputCfg.format;
    menrvaEffectConfig.outputCfg.channels = menrvaEffectConfig.inputCfg.channels;
    menrvaEffectConfig.outputCfg.accessMode = EFFECT_BUFFER_ACCESS_WRITE;

    uint32_t channelLength = audio_channel_count_from_out_mask(menrvaEffectConfig.inputCfg.channels);

    ServiceLocator serviceLocator;
    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0;
    sample frequency = 1.0;
    sample offset = 0.0;
    AudioBuffer& inputSineBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.AndroidAudioFrameLength);

    audio_buffer_t inputBuffer;
    inputBuffer.frameCount = params.AndroidAudioFrameLength;
    inputBuffer.s16 = new int16_t[channelLength * params.AndroidAudioFrameLength];
    AudioOutputBuffer outputConverter = *new AudioOutputBuffer(serviceLocator.GetLogger(), AudioFormat::PCM_16);
    outputConverter.SetData(inputBuffer.s16, channelLength, params.AndroidAudioFrameLength);
    for (uint32_t channelCounter = 0; channelCounter < channelLength; channelCounter++){
        for (size_t sampleCounter = 0; sampleCounter < params.AndroidAudioFrameLength; sampleCounter++) {
            outputConverter.SetValue(channelCounter, sampleCounter, inputSineBuffer[sampleCounter]);
        }
    }

    effect_handle_t menrvaEffectHandle = nullptr;
    AndroidInterface::CreateModule(&AndroidInterface::EffectDescriptor.uuid, 0, 0, &menrvaEffectHandle);
    AndroidModuleInterface menrvaModuleInterface = *(AndroidModuleInterface*)menrvaEffectHandle;
    uint32_t intSize = sizeof(int);
    int setConfigCmdReply = *new int();
    int enableCmdReply = *new int();
    menrvaModuleInterface.effectInterface->command(menrvaEffectHandle, EFFECT_CMD_SET_CONFIG, sizeof(effect_config_t), &menrvaEffectConfig, &intSize, &setConfigCmdReply);
    menrvaModuleInterface.effectInterface->command(menrvaEffectHandle, EFFECT_CMD_ENABLE, 0, nullptr, &intSize, &enableCmdReply);

    audio_buffer_t outputBuffer;
    outputBuffer.frameCount = params.AndroidAudioFrameLength;
    outputBuffer.s16 = new int16_t[params.AndroidAudioFrameLength];
    menrvaModuleInterface.effectInterface->process(menrvaEffectHandle, &inputBuffer, &outputBuffer);
}
