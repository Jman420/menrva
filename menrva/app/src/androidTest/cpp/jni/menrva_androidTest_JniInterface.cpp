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

#include <jni.h>
#include "tools/test_helper.h"
#include "../../../main/cpp/audio/sample.h"
#include "../../../main/cpp/tools/service_locator.h"
#include "../../../main/cpp/ir/fir_generator.h"
#include "../../../main/cpp/effects/bass_boost.h"
#include "../../../main/cpp/tools/wave_generator.h"
#include "../../../main/cpp/module_interface.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug1FirGenerator(JNIEnv* env, jobject instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug2ConvolverOneFrame(JNIEnv* env, jobject instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(params.MenrvaAudioFrameLength, firFilter);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0,
           frequency = 1.0,
           offset = 0.0;
    AudioBuffer& inputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.MenrvaAudioFrameLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    convolver.Process(inputBuffer, outputBuffer);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug3ConvolverFullFilter(JNIEnv* env, jobject instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(params.MenrvaAudioFrameLength, firFilter);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0,
           frequency = 1.0,
           offset = 0.0;
    size_t audioWaveLength = params.MenrvaAudioFrameLength * convolver.GetFilterSegmentsLength();
    AudioBuffer& sineInputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, audioWaveLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    AudioBuffer inputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    for (int frameCounter = 0; frameCounter < convolver.GetFilterSegmentsLength(); frameCounter++) {
        memcpy(inputBuffer.GetData(), &sineInputBuffer[frameCounter * params.MenrvaAudioFrameLength], sizeof(sample) * params.MenrvaAudioFrameLength);
        convolver.Process(inputBuffer, outputBuffer);

        int debug = 0;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug4BassBoost(JNIEnv* env, jobject instance) {
    test_params params;

    ServiceLocator serviceLocator;
    BassBoost jmanBassBoost(serviceLocator.GetLogger(), serviceLocator.GetFirGenerator(), serviceLocator.GetConvolver());
    jmanBassBoost.ResetBuffers(params.SampleRate, params.MenrvaAudioFrameLength);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0,
           frequency = 1.0,
           offset = 0.0;
    AudioBuffer& inputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.MenrvaAudioFrameLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    jmanBassBoost.Process(inputBuffer, outputBuffer);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug5FullPipeline(JNIEnv* env, jobject instance) {
    test_params params;
    effect_config_t menrvaEffectConfig;
    menrvaEffectConfig.inputCfg.samplingRate = static_cast<uint32_t>(params.SampleRate);
    menrvaEffectConfig.inputCfg.format = AudioFormat::PCM_16;
    menrvaEffectConfig.inputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;

    menrvaEffectConfig.outputCfg.samplingRate = menrvaEffectConfig.inputCfg.samplingRate;
    menrvaEffectConfig.outputCfg.format = menrvaEffectConfig.inputCfg.format;
    menrvaEffectConfig.outputCfg.channels = menrvaEffectConfig.inputCfg.channels;
    menrvaEffectConfig.outputCfg.accessMode = EFFECT_BUFFER_ACCESS_WRITE;

    ServiceLocator serviceLocator;
    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0,
           frequency = 1.0,
           offset = 0.0;
    AudioBuffer& inputSineBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.AndroidAudioFrameLength);

    audio_buffer_t inputBuffer = *new audio_buffer_t();
    inputBuffer.frameCount = params.AndroidAudioFrameLength;
    inputBuffer.s16 = new int16_t[params.AndroidAudioFrameLength];
    uint32_t channelLength = audio_channel_count_from_out_mask(menrvaEffectConfig.inputCfg.channels);
    AudioOutputBuffer outputConverter = *new AudioOutputBuffer(serviceLocator.GetLogger(), AudioFormat::PCM_16);
    outputConverter.SetData(inputSineBuffer.GetData(), channelLength, params.AndroidAudioFrameLength);
    for (uint32_t channelCounter = 0; channelCounter < channelLength; channelCounter++){
        for (size_t sampleCounter = 0; sampleCounter < params.AndroidAudioFrameLength; sampleCounter++) {
            size_t interleavedIndex = channelCounter + (sampleCounter * channelLength);
            inputBuffer.s16[interleavedIndex] = *static_cast<int16_t*>(outputConverter(channelCounter, sampleCounter));
        }
    }

    effect_handle_t menrvaEffectHandle = nullptr;
    MenrvaModuleInterface::CreateModule(&MenrvaModuleInterface::EffectDescriptor.uuid, 0, 0, &menrvaEffectHandle);
    MenrvaModuleContext menrvaEngineContext = *(MenrvaModuleContext*)menrvaEffectHandle;
    uint32_t intSize = sizeof(int);
    int setConfigCmdReply = *new int(),
        enableCmdReply = *new int();
    menrvaEngineContext.itfe->command(menrvaEffectHandle, EFFECT_CMD_SET_CONFIG, sizeof(effect_config_t), &menrvaEffectConfig, &intSize, &setConfigCmdReply);
    menrvaEngineContext.itfe->command(menrvaEffectHandle, EFFECT_CMD_ENABLE, 0, nullptr, &intSize, &enableCmdReply);

    audio_buffer_t outputBuffer = *new audio_buffer_t();
    outputBuffer.frameCount = params.AndroidAudioFrameLength;
    outputBuffer.s16 = new int16_t[params.AndroidAudioFrameLength];
    menrvaEngineContext.itfe->process(menrvaEffectHandle, &inputBuffer, &outputBuffer);

    int debug = 0;
}
