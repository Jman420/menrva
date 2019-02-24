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
    _Logger->WriteLog("Disposing of Buffers...", LOG_SENDER, __func__);
    delete[] _InputAudioFrame;
    delete[] _OutputAudioFrame;

    _Logger->WriteLog("Disposing of Effects...", LOG_SENDER, __func__);
    delete[] _MenrvaEffects;

    _Logger->WriteLog("Disposing of FFT Engine...", LOG_SENDER, __func__);
    delete _FftEngine;
    _ChannelLength = 0;
    _Logger->WriteLog("Successfully disposed of Menrva Engine!", LOG_SENDER, __func__);
}

int MenrvaEffectsEngine::SetBufferConfig(uint32_t channelLength, sample sampleRate) {
    _Logger->WriteLog("Setting up Buffer Configs...", LOG_SENDER, __func__);
    _ChannelLength = channelLength;

    _Logger->WriteLog("Instantiating Buffers for (%d) Channels...", LOG_SENDER, __func__, _ChannelLength);
    _InputAudioFrame = new AudioBuffer[_ChannelLength];
    _OutputAudioFrame = new AudioBuffer[_ChannelLength];
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _InputAudioFrame[channelCounter].CreateData(_FftEngine, MENRVA_DSP_FRAME_LENGTH);
        _OutputAudioFrame[channelCounter].CreateData(_FftEngine, MENRVA_DSP_FRAME_LENGTH);
    }

    _Logger->WriteLog("Instantiating Audio Effects for (%d) Channels...", LOG_SENDER, __func__, _ChannelLength);
    _MenrvaEffects = new EffectsBundle[_ChannelLength];
    for (int channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        for (int effectCounter = 0; effectCounter < EffectsBundle::LENGTH; effectCounter++) {
            _MenrvaEffects[channelCounter][effectCounter]->ResetBuffers(sampleRate, MENRVA_DSP_FRAME_LENGTH);
        }
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

    const size_t inputFrameLength = _InputAudioFrame->GetLength(),
                 lastFrameIndex = inputFrameLength - 1;
    size_t inputFrameIndex = 0,
           outputBufferIndex = 0,
           inputBufferLength = inputBuffer.GetSampleLength();
    _Logger->WriteLog("Processing Input Buffer of length (%d) into Audio Frames of length (%d)...", LOG_SENDER, __func__, inputBufferLength, inputFrameLength);
    for (size_t sampleCounter = 0; sampleCounter < inputBufferLength; sampleCounter++) {
        inputFrameIndex = sampleCounter % inputFrameLength;
        _Logger->WriteLog("Loading Input Buffer Index (%d) into Audio Frame Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, sampleCounter, inputFrameIndex);

        for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
            sample value = inputBuffer(channelCounter, sampleCounter);
            _InputAudioFrame[channelCounter][inputFrameIndex] = value;
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

void MenrvaEffectsEngine::ResetBuffers(sample sampleRate) {
    _Logger->WriteLog("Resetting Effects Buffers...", LOG_SENDER, __func__);
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectsBundle& effectsBundle = _MenrvaEffects[channelCounter];
        _Logger->WriteLog("Resetting Buffers for Channel (%d).", LOG_SENDER, __func__, channelCounter);

        for (int8_t effectCounter = 0; effectCounter < EffectsBundle::LENGTH; effectCounter++) {
            EffectBase& effect = *effectsBundle[effectCounter];
            effect.ResetBuffers(sampleRate, MENRVA_DSP_FRAME_LENGTH);
        }
    }
    _Logger->WriteLog("Successfully Reset Effects Buffers!", LOG_SENDER, __func__);
}

void MenrvaEffectsEngine::SetEffectEnabled(uint8_t effectIndex, bool enabled) {
    _Logger->WriteLog("Setting Enabled Flag on Effect Index (%d) to (%d)...", LOG_SENDER, __func__, effectIndex, enabled);
    if (effectIndex >= EffectsBundle::LENGTH) {
        _Logger->WriteLog("Skipping Setting Enabled Flag on Effect Index (%d).  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, effectIndex);
        return;
    }

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectBase& effect = *_MenrvaEffects[channelCounter][effectIndex];
        effect.Enabled = enabled;
        _Logger->WriteLog("Successfully set Enabled Flag for Effect (%s) for Channel (%d) to (%d).", LOG_SENDER, __func__, effect.NAME.c_str(), channelCounter, enabled);
    }
    _Logger->WriteLog("Successfully set Enabled Flag on Effect Index (%d) to (%d)!", LOG_SENDER, __func__, effectIndex, enabled);
}

void MenrvaEffectsEngine::ConfigureEffectSetting(uint8_t effectIndex, char* settingName, void* value) {
    _Logger->WriteLog("Setting Effect Configuration : %s on Effect Index (%d)...", LOG_SENDER, __func__, settingName, effectIndex);
    if (effectIndex >= EffectsBundle::LENGTH) {
        _Logger->WriteLog("Skipping Setting Effect Configuration (%s) on Effect Index (%d).  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, settingName, effectIndex);
        return;
    }

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectBase& effect = *_MenrvaEffects[channelCounter][effectIndex];
        effect.ConfigureSetting(settingName, value);
        _Logger->WriteLog("Successfully set Effect Configuration (%s) for Effect (%s) for Channel (%d).", LOG_SENDER, __func__, settingName, effect.NAME.c_str(), channelCounter);
    }
    _Logger->WriteLog("Successfully set Effect Configuration (%s) on Effect Index (%d)!", LOG_SENDER, __func__, settingName, effectIndex);
}

void MenrvaEffectsEngine::ProcessInputAudioFrame() {
    _Logger->WriteLog("Processing Input Audio Frame...", LOG_SENDER, __func__);
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _Logger->WriteLog("Processing Input Audio Frame for Channel (%d).", LOG_SENDER, __func__, channelCounter);
        EffectsBundle& effectsBundle = _MenrvaEffects[channelCounter];

        for (uint8_t effectCounter = 0; effectCounter < EffectsBundle::LENGTH; effectCounter++) {
            EffectBase& effect = *effectsBundle[effectCounter];
            if (effect.Enabled) {
                _Logger->WriteLog("Processing Effect (%s) for Channel (%d)...", LOG_SENDER, __func__, effect.NAME.c_str(), channelCounter);
                effect.Process(*_InputAudioFrame, *_OutputAudioFrame);
            }
            else {
                _Logger->WriteLog("Skipping Effect (%s) for Channel (%d).  Effect Disabled.", LOG_SENDER, __func__, effect.NAME.c_str(), channelCounter);
            }
        }
    }
    _Logger->WriteLog("Successfully processed Input Audio Frame!", LOG_SENDER, __func__);
}

size_t MenrvaEffectsEngine::ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer& outputBuffer) {
    _Logger->WriteLog("Processing Output Audio Frame...", LOG_SENDER, __func__);
    AudioBuffer& outputFrame = *_OutputAudioFrame;
    size_t outputFrameLength = std::min(outputFrame.GetLength(),
                                        outputBuffer.GetSampleLength() - startOutputIndex);

    _Logger->WriteLog("Output Frame Length : %u", LOG_SENDER, __func__, LogLevel::VERBOSE, outputFrameLength);
    _Logger->WriteLog("Start Output Index : %u", LOG_SENDER, __func__, LogLevel::VERBOSE, startOutputIndex);

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _Logger->WriteLog("Processing Output Audio Frame of length (%u) for Channel (%u)...", LOG_SENDER, __func__, outputFrameLength, channelCounter);
        outputFrame = _OutputAudioFrame[channelCounter];

        for (size_t outputCounter = 0; outputCounter < outputFrameLength; outputCounter++) {
            sample value = outputFrame[outputCounter];
            size_t outputBufferIndex = outputCounter + startOutputIndex;
            _Logger->WriteLog("Processing Output Audio Frame Index (%d) with Value (%f) into Output Buffer Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter, value, outputBufferIndex);
            outputBuffer.SetValue(channelCounter, outputBufferIndex, value);
            _Logger->WriteLog("Successfully processed Output Audio Frame Index (%d) into Output Buffer Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter, outputBufferIndex);
        }
    }

    _Logger->WriteLog("Successfully processed Output Audio Frame!", LOG_SENDER, __func__);
    return startOutputIndex + outputFrameLength;
}
