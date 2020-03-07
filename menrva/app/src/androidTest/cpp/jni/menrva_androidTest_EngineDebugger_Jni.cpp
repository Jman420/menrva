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
#include "../tools/test_helper.h"
#include "../tools/engine_debugging.h"
#include "../../../main/cpp/audio/Sample.h"
#include "../../../main/cpp/hosts/ServiceLocator.h"
#include "../../../main/cpp/ir/FirGenerator.h"
#include "../../../main/cpp/effects/BassBoost.h"
#include "../../../main/cpp/ir/WaveGenerator.h"
#include "../../../main/cpp/hosts/android/AndroidInterface.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug1FirGenerator(JNIEnv* __unused env, jobject __unused instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug2ConvolverOneFrame(JNIEnv* __unused env, jobject __unused instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(params.MenrvaAudioFrameLength, firFilter);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0;
    sample frequency = 1.0;
    sample offset = 0.0;
    AudioBuffer& inputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.MenrvaAudioFrameLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    convolver.Process(inputBuffer, outputBuffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug3ConvolverFullFilter(JNIEnv* __unused env, jobject __unused instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(params.MenrvaAudioFrameLength, firFilter);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0;
    sample frequency = 1.0;
    sample offset = 0.0;
    size_t audioWaveLength = params.MenrvaAudioFrameLength * convolver.GetFilterSegmentsLength();
    AudioBuffer& sineInputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, audioWaveLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    AudioBuffer inputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    for (int frameCounter = 0; frameCounter < convolver.GetFilterSegmentsLength(); frameCounter++) {
        memcpy(inputBuffer.GetData(), &sineInputBuffer[frameCounter * params.MenrvaAudioFrameLength], sizeof(sample) * params.MenrvaAudioFrameLength);
        convolver.Process(inputBuffer, outputBuffer);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug4BassBoost(JNIEnv* __unused env, jobject __unused instance) {
    test_params params;

    ServiceLocator serviceLocator;
    BassBoost jmanBassBoost(serviceLocator.GetLogger(), serviceLocator.GetFirGenerator(), serviceLocator.GetConvolver());
    jmanBassBoost.ResetBuffers(params.SampleRate, params.MenrvaAudioFrameLength);

    WaveGenerator waveGenerator(serviceLocator.GetFftEngine());
    sample amplitude = 1.0;
    sample frequency = 1.0;
    sample offset = 0.0;
    AudioBuffer& inputBuffer = *waveGenerator.CalculateSineWave(amplitude, frequency, offset, params.MenrvaAudioFrameLength);

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), params.MenrvaAudioFrameLength);
    jmanBassBoost.Process(inputBuffer, outputBuffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug5ProcessPipelineMono(JNIEnv* __unused env, jobject __unused instance) {
    EngineDebugging::ProcessPipeline(AUDIO_CHANNEL_OUT_MONO);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug6ProcessPipelineStereo(JNIEnv* __unused env, jobject __unused instance) {
    EngineDebugging::ProcessPipeline(AUDIO_CHANNEL_OUT_STEREO);
}
