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
#include <cmath>
#include "../../../main/cpp/audio/sample.h"
#include "../../../main/cpp/tools/service_locator.h"
#include "../../../main/cpp/ir/fir_generator.h"
#include "../../../main/cpp/effects/bass_boost.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug1FirGenerator(JNIEnv* env, jobject instance) {
    sample centerFreq = 60.0,
           sampleRate = 44100.0,
           freqTransition = 80.0,
           strength = 6.0;
    size_t interpolationSize = 4096,
           sampleSize = 4;

    sample frequencySamples[] = { 0, (sample)((centerFreq * 2.0) / sampleRate), (sample)((centerFreq * 2.0 + freqTransition) / sampleRate), 1.0 },
           amplitudeSamples[] = { (sample)(pow(10.0, strength / 20.0)), (sample)(pow(10.0, strength / 20.0)), 1.0, 1.0 };

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer* firFilter = firGenerator.Calculate(interpolationSize, frequencySamples, amplitudeSamples, sampleSize);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug2ConvolverOneFrame(JNIEnv* env, jobject instance) {
    sample centerFreq = 60.0,
           sampleRate = 44100.0,
           freqTransition = 80.0,
           strength = 6.0;
    size_t interpolationSize = 4096,
           sampleSize = 4,
           audioFrameLength = 1024;

    sample frequencySamples[] = { 0, (sample)((centerFreq * 2.0) / sampleRate), (sample)((centerFreq * 2.0 + freqTransition) / sampleRate), 1.0 },
           amplitudeSamples[] = { (sample)(pow(10.0, strength / 20.0)), (sample)(pow(10.0, strength / 20.0)), 1.0, 1.0 };

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer& firFilter = *firGenerator.Calculate(interpolationSize, frequencySamples, amplitudeSamples, sampleSize);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(audioFrameLength, firFilter);

    sample angle = 0.0,
           amplitude = 1.0,
           offset = 0.0;
    AudioBuffer inputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    for (int sampleCounter = 0; sampleCounter < audioFrameLength; sampleCounter++)
    {
        inputBuffer[sampleCounter] = amplitude * sin(angle) + offset;
        angle += (2 * M_PI) / audioFrameLength;
    }

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    convolver.Process(inputBuffer, outputBuffer);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug3ConvolverFullFilter(JNIEnv* env, jobject instance) {
    sample centerFreq = 60.0,
            sampleRate = 44100.0,
            freqTransition = 80.0,
            strength = 6.0;
    size_t interpolationSize = 4096,
            sampleSize = 4,
            audioFrameLength = 1024;

    sample frequencySamples[] = { 0, (sample)((centerFreq * 2.0) / sampleRate), (sample)((centerFreq * 2.0 + freqTransition) / sampleRate), 1.0 },
           amplitudeSamples[] = { (sample)(pow(10.0, strength / 20.0)), (sample)(pow(10.0, strength / 20.0)), 1.0, 1.0 };

    ServiceLocator serviceLocator;
    FirGenerator firGenerator(serviceLocator.GetLogger(), serviceLocator.GetFftEngine());
    AudioBuffer& firFilter = *firGenerator.Calculate(interpolationSize, frequencySamples, amplitudeSamples, sampleSize);
    Convolver convolver(serviceLocator.GetLogger(), serviceLocator.GetFftEngine(), serviceLocator.GetConvolutionOperations());
    convolver.Initialize(audioFrameLength, firFilter);

    sample angle = 0.0,
           amplitude = 1.0,
           offset = 0.0;
    AudioBuffer inputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    size_t audioWaveLength = audioFrameLength * convolver.GetFilterSegmentsLength();
    for (int sampleCounter = 0; sampleCounter < audioWaveLength; sampleCounter++)
    {
        inputBuffer[sampleCounter] = amplitude * sin(angle) + offset;
        angle += (2 * M_PI) / audioFrameLength;
    }

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    convolver.Process(inputBuffer, outputBuffer);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug4BassBoost(JNIEnv* env, jobject instance) {
    sample sampleRate = 44100.0;
    size_t audioFrameLength = 1024;

    ServiceLocator serviceLocator;
    BassBoost jmanBassBoost(serviceLocator.GetLogger(), serviceLocator.GetFirGenerator(), serviceLocator.GetConvolver());
    jmanBassBoost.ResetBuffers(sampleRate, audioFrameLength);

    sample angle = 0.0,
            amplitude = 1.0,
            offset = 0.0;
    AudioBuffer inputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    for (int sampleCounter = 0; sampleCounter < audioFrameLength; sampleCounter++)
    {
        inputBuffer[sampleCounter] = amplitude * sin(angle) + offset;
        angle += (2 * M_PI) / audioFrameLength;
    }

    AudioBuffer outputBuffer(serviceLocator.GetFftEngine(), audioFrameLength);
    jmanBassBoost.Process(inputBuffer, outputBuffer);

    int debug = 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_monkeystable_menrva_EngineDebugging_debug5EnginePipeline(JNIEnv* env, jobject instance) {
    // TODO : Initialize Params to Create Module
    // TODO : Create Menrva Module
    // TODO : Generate Sine Wave
    // TODO : Break Sine Wave into Segments of 1150
    // TODO : Pass Segments to Menrva Engine for Processing

    int debug = 0;
}
