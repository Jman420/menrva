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

#include <cerrno>
#include <algorithm>
#include "effects_engine.h"
#include "../config.h"
#include "../tools/buffer.cpp"
#include "../effects/bass_boost.h"
#include "../effects/stereo_widener.h"
#include "../effects/equalizer.h"

MenrvaEffectsEngine::MenrvaEffectsEngine(LoggerBase* logger, FftInterfaceBase* fftEngine, ServiceLocator* serviceLocator)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _ServiceLocator = serviceLocator;
    _FftEngine = fftEngine;
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_UNCONFIGURED;
}

MenrvaEffectsEngine::~MenrvaEffectsEngine() {
    _Logger->WriteLog("Disposing of Buffers & Audio Effects...", LOG_SENDER, __func__);
    for (int channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        delete _InputAudioFrame[channelCounter];
        delete _OutputAudioFrame[channelCounter];

        delete _MenrvaEffects[channelCounter];
    }
    delete _InputAudioFrame;
    delete _OutputAudioFrame;
    delete _MenrvaEffects;

    delete _FftEngine;
    _ChannelLength = 0;
    _Logger->WriteLog("Successfully disposed of Menrva Engine!", LOG_SENDER, __func__);
}

int MenrvaEffectsEngine::SetBufferConfig(effect_config_t& bufferConfig) {
    _Logger->WriteLog("Setting up Buffer Configs...", LOG_SENDER, __func__);
    _ChannelLength = bufferConfig.inputCfg.channels;

    _Logger->WriteLog("Allocating Buffers for (%d) Channels...", LOG_SENDER, __func__, _ChannelLength);
    size_t totalBuffersSize = sizeof(AudioBuffer) * _ChannelLength;
    _InputAudioFrame = (AudioBuffer*)malloc(totalBuffersSize);
    _OutputAudioFrame = (AudioBuffer*)malloc(totalBuffersSize);

    _Logger->WriteLog("Allocating Audio Effects for (%d) Channels...", LOG_SENDER, __func__, _ChannelLength);
    size_t totalEffectsSize = sizeof(EffectsBundle) * _ChannelLength;
    _MenrvaEffects = (EffectsBundle*)malloc(totalEffectsSize);

    _Logger->WriteLog("Instantiating Buffers & Audio Effects for (%d) Channels...", LOG_SENDER, __func__, _ChannelLength);
    for (int channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _InputAudioFrame[channelCounter] = *new AudioBuffer(_FftEngine, MENRVA_DSP_FRAME_LENGTH);
        _OutputAudioFrame[channelCounter] = *new AudioBuffer(_FftEngine, MENRVA_DSP_FRAME_LENGTH);

        EffectsBundle& effectsBundle = *new EffectsBundle();
        _MenrvaEffects[channelCounter] = effectsBundle;
        effectsBundle.BassBoost = new BassBoost(_Logger, _ServiceLocator);
        effectsBundle.Equalizer = new Equalizer(_Logger, _ServiceLocator);
        effectsBundle.StereoWidener = new StereoWidener(_Logger, _ServiceLocator);
    }

    _Logger->WriteLog("Successfully setup Buffer Configs!", LOG_SENDER, __func__);
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_DISABLED;
    return 0;
}

int MenrvaEffectsEngine::Process(AudioInputBuffer& inputBuffer, AudioOutputBuffer& outputBuffer) {
    _Logger->WriteLog("Processing Input Audio Buffer of length (%d) and channels (%d)...", LOG_SENDER, __func__, inputBuffer.GetSampleLength(), _ChannelLength);
    if (_EngineStatus == MenrvaEngineStatus::MENRVA_ENGINE_UNCONFIGURED) {
        _Logger->WriteLog("Unable to process Input Audio Buffer of length (%d) and channels (%d).  Engine not configured!", LOG_SENDER, __func__, LogLevel::ERROR, inputBuffer.GetSampleLength(), _ChannelLength);
        return -EINVAL;
    }
    if (_EngineStatus == MenrvaEngineStatus::MENRVA_ENGINE_DISABLED) {
        _Logger->WriteLog("Skipping processing Input Audio Buffer of length (%d) and channels (%d).  Engine is Disabled!", LOG_SENDER, __func__);
        return -ENODATA;
    }

    size_t inputFrameIndex = 0,
           outputBufferIndex = 0,
           inputFrameLength = _InputAudioFrame->GetLength(),
           lastFrameIndex = inputFrameLength - 1;
    _Logger->WriteLog("Processing Audio Frames of size (%d)...", LOG_SENDER, __func__, inputFrameLength);
    for (size_t sampleCounter = 0; sampleCounter < inputBuffer.GetSampleLength(); sampleCounter++) {
        inputFrameIndex = sampleCounter % inputFrameLength;
        _Logger->WriteLog("Loading Input Buffer Index (%d) into Audio Frame Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, sampleCounter, inputFrameIndex);

        for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
            AudioBuffer& channelFrameBuffer = _InputAudioFrame[channelCounter];
            sample value = inputBuffer(channelCounter, sampleCounter);
            channelFrameBuffer[sampleCounter] = value;
            _Logger->WriteLog("Successfully loaded Input Sample Value (%f) from Input Buffer Index (%d) into Audio Frame Index (%d).", LOG_SENDER, __func__, LogLevel::VERBOSE, value, sampleCounter, inputFrameIndex);
        }

        if (inputFrameIndex == lastFrameIndex) {
            _Logger->WriteLog("Processing Full Audio Frame ending on Input Buffer Index (%d)...", LOG_SENDER, __func__, sampleCounter);
            ProcessInputAudioFrame();
            outputBufferIndex = ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
            _Logger->WriteLog("Successfully processed Full Audio Frame ending on Input Buffer Index (%d).", LOG_SENDER, __func__, sampleCounter);
        }
    }

    if (inputFrameIndex != lastFrameIndex) {
        _Logger->WriteLog("Padding Incomplete Audio Frame with zeros from Index (%d)...", LOG_SENDER, __func__, inputFrameIndex);
        for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
            _InputAudioFrame[channelCounter].ResetData(inputFrameIndex);
        }

        _Logger->WriteLog("Processing Final Incomplete Audio Frame...", LOG_SENDER, __func__);
        ProcessInputAudioFrame();
        ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
        _Logger->WriteLog("Successfully processed Final Incomplete Audio Frame!", LOG_SENDER, __func__);
    }

    _Logger->WriteLog("Successfully processed Input Audio Buffer of length (%d)!", LOG_SENDER, __func__, inputBuffer.GetSampleLength());
    return (_EngineStatus == MenrvaEngineStatus::MENRVA_ENGINE_DISABLED) ? -ENODATA : 0;
}

void MenrvaEffectsEngine::ResetBuffers(effect_config_t &bufferConfig) {
    _Logger->WriteLog("Resetting Effects Buffers...", LOG_SENDER, __func__);
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectsBundle& effect = _MenrvaEffects[channelCounter];
        _Logger->WriteLog("Resetting Buffers for Effect : %s", LOG_SENDER, __func__, effect.NAME.c_str());
        effect.ResetBuffers(bufferConfig, MENRVA_DSP_FRAME_LENGTH);
    }
    _Logger->WriteLog("Successfully Reset Effects Buffers!", LOG_SENDER, __func__);
}

void MenrvaEffectsEngine::SetEffectEnabled(uint8_t effectIndex, bool enabled) {
    _Logger->WriteLog("Setting Enabled Flag on Effect Index (%d) to (%d)...", LOG_SENDER, __func__, effectIndex, enabled);
    if (effectIndex >= EFFECTS_LENGTH) {
        _Logger->WriteLog("Skipping Setting Enabled Flag on Effect Index (%d).  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->Enabled = enabled;
    _Logger->WriteLog("Successfully set Enabled Flag on Effect Index (%d) to (%d)!", LOG_SENDER, __func__, effectIndex, enabled);
}

void MenrvaEffectsEngine::ConfigureEffectSetting(uint8_t effectIndex, char* settingName,
                                                 void* value) {
    _Logger->WriteLog("Setting Effect Configuration : %s on Effect Index (%d)...", LOG_SENDER, __func__, settingName, effectIndex);
    if (effectIndex >= sizeof(_MenrvaEffects)) {
        _Logger->WriteLog("Skipping Setting Effect Configuration (%s) on Effect Index (%d).  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, settingName, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->ConfigureSetting(settingName, value);
    _Logger->WriteLog("Successfully set Effect Configuration (%s) on Effect Index (%d)!", LOG_SENDER, __func__, settingName, effectIndex);
}

void MenrvaEffectsEngine::ProcessInputAudioFrame() {
    _Logger->WriteLog("Processing Input Audio Frame...", LOG_SENDER, __func__);
    for (EffectBase* effect : _MenrvaEffects) {
        if (effect->Enabled) {
            _Logger->WriteLog("Processing Effect : %s", LOG_SENDER, __func__, LogLevel::VERBOSE, effect->NAME.c_str());
            // TODO : Ensure that Effect's Processing will aggregate (need to output to the next effect's input buffer; probably need to re-think managing enabled effects in a separate vector collection, all effects except the last in collection will overwrite Input Buffer and only last effect will overwrite Output Buffer)
            effect->Process(*_InputAudioFrame, *_OutputAudioFrame);
        }
        else {
            _Logger->WriteLog("Skipping Effect : %s.  Effect Disabled.", LOG_SENDER, __func__, LogLevel::VERBOSE);
        }
    }
    _Logger->WriteLog("Successfully processed Input Audio Frame!", LOG_SENDER, __func__);
}

size_t MenrvaEffectsEngine::ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer& outputBuffer) {
    _Logger->WriteLog("Processing Output Audio Frame...", LOG_SENDER, __func__);
    AudioBuffer& outputFrame = *_OutputAudioFrame;
    size_t outputFrameLength = std::min(outputFrame.GetLength(),
                                        outputBuffer.GetSampleLength() - startOutputIndex);

    for (size_t outputCounter = 0; outputCounter < outputFrameLength; outputCounter++) {
        sample value = outputFrame[outputCounter];
        size_t outputBufferIndex = outputCounter + startOutputIndex;
        _Logger->WriteLog("Processing Output Audio Frame Index (%d) with Value (%f) into Output Buffer Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter, value, outputBufferIndex);
        outputBuffer.SetValue(outputBufferIndex, value);
        _Logger->WriteLog("Successfully processed Output Audio Frame Index (%d) into Output Buffer Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter, outputBufferIndex);
    }

    _Logger->WriteLog("Successfully processed Output Audio Frame!", LOG_SENDER, __func__);
    return startOutputIndex + outputFrameLength;
}
