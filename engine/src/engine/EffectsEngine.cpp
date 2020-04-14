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
#include "EffectsEngine.h"
#include "../EngineConfig.h"
#include "../tools/StringOperations.h"

MenrvaEffectsEngine::MenrvaEffectsEngine(ILogWriter* logger, FftInterfaceBase* fftEngine, ServiceLocator* serviceLocator)
        : LogProducer(logger, __PRETTY_FUNCTION__) {
    _ServiceLocator = serviceLocator;
    _FftEngine = fftEngine;
    _EngineStatus = MenrvaEngineStatus::UNCONFIGURED;
}

MenrvaEffectsEngine::~MenrvaEffectsEngine() {
    _Logger->WriteLog("Disposing of Buffers...", LOG_SENDER, __func__);
    delete[] _InputAudioFrame;
    delete[] _OutputAudioFrame;

    _Logger->WriteLog("Disposing of Effects...", LOG_SENDER, __func__);
    delete _SingleChannelEffects;
    delete _MultiChannelEffects;

    _Logger->WriteLog("Disposing of FFT Engine...", LOG_SENDER, __func__);
    delete _FftEngine;
    _ChannelLength = 0;
    _Logger->WriteLog("Successfully disposed of Menrva Engine!", LOG_SENDER, __func__);
}

void MenrvaEffectsEngine::SetBufferConfig(uint32_t channelLength, sample sampleRate, size_t frameLength) {
    _Logger->WriteLog("Setting up Buffer Configs...", LOG_SENDER, __func__);
    _ChannelLength = channelLength;

    _Logger->WriteLog(StringOperations::FormatString("Instantiating Buffers for (%d) Channels...", _ChannelLength),
                      LOG_SENDER, __func__);
    _InputAudioFrame = new AudioBuffer[_ChannelLength];
    _OutputAudioFrame = new AudioBuffer[_ChannelLength];
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _InputAudioFrame[channelCounter].CreateData(_FftEngine, frameLength);
        _OutputAudioFrame[channelCounter].CreateData(_FftEngine, frameLength);
    }

    _Logger->WriteLog(StringOperations::FormatString("Instantiating Single Channel Audio Effects for (%d) Channels...", _ChannelLength),
                      LOG_SENDER, __func__);
    _SingleChannelEffects = new SingleChannelEffectsBundle[_ChannelLength];
    for (int channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _SingleChannelEffects[channelCounter].ResetBuffers(sampleRate, frameLength);
    }

    _Logger->WriteLog("Instantiating Multi Channel Audio Effects...", LOG_SENDER, __func__);
    _MultiChannelEffects = new MultiChannelEffectsBundle(*_ServiceLocator);
    _MultiChannelEffects->ResetBuffers(sampleRate, frameLength);

    _Logger->WriteLog("Successfully setup Buffer Configs!", LOG_SENDER, __func__);
    _EngineStatus = MenrvaEngineStatus::DISABLED;
}

int MenrvaEffectsEngine::Process(AudioInputBuffer& inputBuffer, AudioOutputBuffer& outputBuffer) {
    _Logger->WriteLog(StringOperations::FormatString("Processing Input Audio Buffer of length (%d) and channels (%d)...", inputBuffer.GetSampleLength(), _ChannelLength),
                      LOG_SENDER, __func__);
    if (_EngineStatus == MenrvaEngineStatus::UNCONFIGURED) {
        _Logger->WriteLog(StringOperations::FormatString("Unable to process Input Audio Buffer of length (%d) and channels (%d).  Engine not configured!", inputBuffer.GetSampleLength(), _ChannelLength),
                          LOG_SENDER, __func__, LogLevel::Error);
        return -EINVAL;
    }
    if (_EngineStatus == MenrvaEngineStatus::DISABLED) {
        _Logger->WriteLog(StringOperations::FormatString("Skipping processing Input Audio Buffer of length (%d) and channels (%d).  Engine is Disabled!", inputBuffer.GetSampleLength(), _ChannelLength),
                          LOG_SENDER, __func__);
        return -ENODATA;
    }

    const size_t inputFrameLength = _InputAudioFrame->GetLength();
    const size_t lastFrameIndex = inputFrameLength - 1;
    size_t inputFrameIndex = 0;
    size_t outputBufferIndex = 0;
    size_t inputBufferLength = inputBuffer.GetSampleLength();
    _Logger->WriteLog(StringOperations::FormatString("Processing Input Buffer of length (%d) into Audio Frames of length (%d)...", inputBufferLength, inputFrameLength),
                      LOG_SENDER, __func__);
    for (size_t sampleCounter = 0; sampleCounter < inputBufferLength; sampleCounter++) {
        inputFrameIndex = sampleCounter % inputFrameLength;
        _Logger->WriteLog(StringOperations::FormatString("Loading Input Buffer Index (%d) into Audio Frame Index (%d)...", sampleCounter, inputFrameIndex),
                          LOG_SENDER, __func__, LogLevel::Verbose);

        for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
            sample value = inputBuffer(channelCounter, sampleCounter);
            _InputAudioFrame[channelCounter][inputFrameIndex] = value;
            _Logger->WriteLog(StringOperations::FormatString("Successfully loaded Input Sample Value (%f) from Input Buffer Index (%d) into Audio Frame Index (%d).", value, sampleCounter, inputFrameIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
        }

        if (inputFrameIndex == lastFrameIndex) {
            _Logger->WriteLog(StringOperations::FormatString("Processing Full Audio Frame ending on Input Buffer Index (%d)...", sampleCounter),
                              LOG_SENDER, __func__);
            ProcessInputAudioFrame();
            outputBufferIndex = ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
            _Logger->WriteLog(StringOperations::FormatString("Successfully processed Full Audio Frame ending on Input Buffer Index (%d).", sampleCounter),
                              LOG_SENDER, __func__);
        }
    }

    inputFrameIndex++;
    if (inputFrameIndex < lastFrameIndex) {
        _Logger->WriteLog(StringOperations::FormatString("Padding Incomplete Audio Frame with zeros from Index (%d)...", inputFrameIndex),
                          LOG_SENDER, __func__);
        for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
            _InputAudioFrame[channelCounter].ResetData(inputFrameIndex);
        }

        _Logger->WriteLog("Processing Final Incomplete Audio Frame...", LOG_SENDER, __func__);
        ProcessInputAudioFrame();
        ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
        _Logger->WriteLog("Successfully processed Final Incomplete Audio Frame!", LOG_SENDER, __func__);
    }

    _Logger->WriteLog(StringOperations::FormatString("Successfully processed Input Audio Buffer of length (%d)!", inputBuffer.GetSampleLength()),
                      LOG_SENDER, __func__);
    return (_EngineStatus == MenrvaEngineStatus::DISABLED) ? -ENODATA : 0;
}

void MenrvaEffectsEngine::ResetBuffers(sample sampleRate) {
    _Logger->WriteLog("Resetting Effects Buffers...", LOG_SENDER, __func__);
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        SingleChannelEffectsBundle& effectsBundle = _SingleChannelEffects[channelCounter];
        _Logger->WriteLog(StringOperations::FormatString("Resetting Buffers for Channel (%d).", channelCounter),
                          LOG_SENDER, __func__);

        for (int8_t effectCounter = 0; effectCounter < SingleChannelEffectsBundle::LENGTH; effectCounter++) {
            EffectBase& effect = *effectsBundle[effectCounter];
            effect.ResetBuffers(sampleRate, MENRVA_DSP_FRAME_LENGTH);
        }
    }
    _Logger->WriteLog("Successfully Reset Effects Buffers!", LOG_SENDER, __func__);
}

void MenrvaEffectsEngine::SetEffectEnabled(uint8_t effectIndex, bool enabled) {
    _Logger->WriteLog(StringOperations::FormatString("Setting Enabled Flag on Effect Index (%d) to (%d)...", effectIndex, enabled),
                      LOG_SENDER, __func__);
    if (effectIndex >= SingleChannelEffectsBundle::LENGTH) {
        _Logger->WriteLog(StringOperations::FormatString("Skipping Setting Enabled Flag on Effect Index (%d).  Index out of bounds.", effectIndex),
                          LOG_SENDER, __func__, LogLevel::Warn);
        return;
    }

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectBase& effect = *_SingleChannelEffects[channelCounter][effectIndex];
        effect.Enabled = enabled;
        _Logger->WriteLog(StringOperations::FormatString("Successfully set Enabled Flag for Effect (%s) for Channel (%d) to (%d).", effect.NAME.c_str(), channelCounter, enabled),
                          LOG_SENDER, __func__);
    }
    _Logger->WriteLog(StringOperations::FormatString("Successfully set Enabled Flag on Effect Index (%d) to (%d)!", effectIndex, enabled),
                      LOG_SENDER, __func__);
}

void MenrvaEffectsEngine::ConfigureEffectSetting(uint8_t effectIndex, char* settingName, void* value) {
    _Logger->WriteLog(StringOperations::FormatString("Setting Effect Configuration : %s on Effect Index (%d)...", settingName, effectIndex),
                      LOG_SENDER, __func__);
    if (effectIndex >= SingleChannelEffectsBundle::LENGTH) {
        _Logger->WriteLog(StringOperations::FormatString("Skipping Setting Effect Configuration (%s) on Effect Index (%d).  Index out of bounds.", settingName, effectIndex),
                          LOG_SENDER, __func__, LogLevel::Warn);
        return;
    }

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        EffectBase& effect = *_SingleChannelEffects[channelCounter][effectIndex];
        effect.ConfigureSetting(settingName, value);
        _Logger->WriteLog(StringOperations::FormatString("Successfully set Effect Configuration (%s) for Effect (%s) for Channel (%d).", settingName, effect.NAME.c_str(), channelCounter),
                          LOG_SENDER, __func__);
    }
    _Logger->WriteLog(StringOperations::FormatString("Successfully set Effect Configuration (%s) on Effect Index (%d)!", settingName, effectIndex),
                      LOG_SENDER, __func__);
}

SingleChannelEffectsBundle* MenrvaEffectsEngine::GetSingleChannelEffectsBundle() {
    return _SingleChannelEffects;
}

MultiChannelEffectsBundle* MenrvaEffectsEngine::GetMultiChannelEffectsBundle() {
    return _MultiChannelEffects;
}

void MenrvaEffectsEngine::ProcessInputAudioFrame() {
    _Logger->WriteLog("Processing Input Audio Frame...", LOG_SENDER, __func__);

    /**_Logger->WriteLog(StringOperations::FormatString("Processing Single Channel Effects for (%d) Channels...", _ChannelLength),
                      LOG_SENDER, __func__);
    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _Logger->WriteLog(StringOperations::FormatString("Processing Input Audio Frame for Channel (%d).", channelCounter),
                          LOG_SENDER, __func__);
        SingleChannelEffectsBundle& effectsBundle = _SingleChannelEffects[channelCounter];

        for (uint8_t effectCounter = 0; effectCounter < SingleChannelEffectsBundle::LENGTH; effectCounter++) {
            SingleChannelEffectBase& effect = *effectsBundle[effectCounter];
            if (effect.Enabled) {
                _Logger->WriteLog(StringOperations::FormatString("Processing Effect (%s) for Channel (%d)...", effect.NAME.c_str(), channelCounter),
                                  LOG_SENDER, __func__);
                effect.Process(_InputAudioFrame[channelCounter], _InputAudioFrame[channelCounter]);
            }
            else {
                _Logger->WriteLog(StringOperations::FormatString("Skipping Effect (%s) for Channel (%d).  Effect Disabled.", effect.NAME.c_str(), channelCounter),
                                  LOG_SENDER, __func__);
            }
        }
    }

    _Logger->WriteLog("Processing Multi Channel Effects...", LOG_SENDER, __func__);
    MultiChannelEffectsBundle& multiChannelEffectsBundle = *_MultiChannelEffects;
    for (uint8_t effectCounter = 0; effectCounter < MultiChannelEffectsBundle::LENGTH - 1; effectCounter++) {
        MultiChannelEffectBase& effect = *multiChannelEffectsBundle[effectCounter];
        if (effect.Enabled) {
            _Logger->WriteLog(StringOperations::FormatString("Processing Effect (%s) for (%d) Channels...", effect.NAME.c_str(), _ChannelLength),
                              LOG_SENDER, __func__);
            effect.Process(_InputAudioFrame, _InputAudioFrame, _ChannelLength);
        }
        else {
            _Logger->WriteLog(StringOperations::FormatString("Skipping Effect (%s).  Effect Disabled.", effect.NAME.c_str()),
                              LOG_SENDER, __func__);
        }
    }
    /**/

    MultiChannelEffectsBundle& multiChannelEffectsBundle = *_MultiChannelEffects;

    _Logger->WriteLog("Processing Master Limiter Effect...", LOG_SENDER, __func__);
    MasterLimiter& masterLimiter = *multiChannelEffectsBundle.GetMasterLimiter();
    masterLimiter.Process(_InputAudioFrame, _OutputAudioFrame, _ChannelLength);

    _Logger->WriteLog("Successfully processed Input Audio Frame!", LOG_SENDER, __func__);
}

size_t MenrvaEffectsEngine::ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer& outputBuffer) {
    _Logger->WriteLog("Processing Output Audio Frame...", LOG_SENDER, __func__);
    AudioBuffer& outputFrame = *_OutputAudioFrame;
    size_t outputFrameLength = std::min(outputFrame.GetLength(),
                                        outputBuffer.GetSampleLength() - startOutputIndex);

    _Logger->WriteLog(StringOperations::FormatString("Output Frame Length : %u", outputFrameLength),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    _Logger->WriteLog(StringOperations::FormatString("Start Output Index : %u", startOutputIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);

    for (uint32_t channelCounter = 0; channelCounter < _ChannelLength; channelCounter++) {
        _Logger->WriteLog(StringOperations::FormatString("Processing Output Audio Frame of length (%u) for Channel (%u)...", outputFrameLength, channelCounter),
                          LOG_SENDER, __func__);
        outputFrame = _OutputAudioFrame[channelCounter];

        for (size_t outputCounter = 0; outputCounter < outputFrameLength; outputCounter++) {
            sample value = outputFrame[outputCounter];
            size_t outputBufferIndex = outputCounter + startOutputIndex;
            _Logger->WriteLog(StringOperations::FormatString("Processing Output Audio Frame Index (%d) with Value (%f) into Output Buffer Index (%d)...", outputCounter, value, outputBufferIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            outputBuffer.SetValue(channelCounter, outputBufferIndex, value);
            _Logger->WriteLog(StringOperations::FormatString("Successfully processed Output Audio Frame Index (%d) into Output Buffer Index (%d)!", outputCounter, outputBufferIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
        }
    }

    _Logger->WriteLog("Successfully processed Output Audio Frame!", LOG_SENDER, __func__);
    return startOutputIndex + outputFrameLength;
}
