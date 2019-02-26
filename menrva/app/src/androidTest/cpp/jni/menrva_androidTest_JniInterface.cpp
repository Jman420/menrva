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
#include "../../../main/cpp/audio/sample.h"
#include "../../../main/cpp/tools/service_locator.h"
#include "../../../main/cpp/ir/fir_generator.h"
#include "../../../main/cpp/effects/bass_boost.h"
#include "../../../main/cpp/tools/wave_generator.h"
#include "../../../main/cpp/module_interface.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug1FirGenerator(JNIEnv* env, jobject instance) {
    test_params params;

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer firFilter = *firGenerator.Calculate(params.FirInterpolationLength, params.FirFrequencySamples, params.FirAmplitudeSamples, params.FirSamplesLength);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug2ConvolverOneFrame(JNIEnv* env, jobject instance) {
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
Java_com_monkeystable_menrva_EngineDebugger_debug3ConvolverFullFilter(JNIEnv* env, jobject instance) {
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
Java_com_monkeystable_menrva_EngineDebugger_debug4BassBoost(JNIEnv* env, jobject instance) {
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
Java_com_monkeystable_menrva_EngineDebugger_debug5ProcessPipelineMono(JNIEnv* env, jobject instance) {
    EngineDebugging::ProcessPipeline(AUDIO_CHANNEL_OUT_MONO);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugger_debug6ProcessPipelineStereo(JNIEnv* env, jobject instance) {
    EngineDebugging::ProcessPipeline(AUDIO_CHANNEL_OUT_STEREO);
}
