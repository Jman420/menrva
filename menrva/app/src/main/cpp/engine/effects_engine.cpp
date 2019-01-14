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

#include <cstdlib>
#include "effects_engine.h"
#include "../config.h"
#include "../tools/buffer.cpp"

MenrvaEffectsEngine::MenrvaEffectsEngine(LoggerBase* logger, FftInterfaceBase* fftEngine)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_UNINITIALIZED;
    _MenrvaEffects[0] = new BassBoost(_Logger);
    _MenrvaEffects[1] = new StereoWidener(_Logger);
    _MenrvaEffects[2] = new Equalizer(_Logger);

    _InputAudioFrame = new AudioBuffer(_Logger, fftEngine, DSP_FRAME_LENGTH);
    _OutputAudioFrame = new AudioBuffer(_Logger, fftEngine, DSP_FRAME_LENGTH);
}

void MenrvaEffectsEngine::ResetEffects(effect_config_t* bufferConfig) {
    _Logger->WriteLog("Resetting Effects...", LOG_SENDER, __func__);
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_INITIALIZING;

    for (EffectBase* effect : _MenrvaEffects) {
        _Logger->WriteLog("Resetting Effect : %s", LOG_SENDER, __func__, effect->NAME.c_str());
        effect->ResetConfig(bufferConfig);
    }
    _Logger->WriteLog("Successfully Reset Effects!", LOG_SENDER, __func__);
}

int MenrvaEffectsEngine::SetBufferConfig(effect_config_t* bufferConfig) {
    // TODO : Implement Logic to Configure Effects
    return 0;
}

int MenrvaEffectsEngine::Process(AudioInputBuffer* inputBuffer, AudioOutputBuffer* outputBuffer) {
    _Logger->WriteLog("Processing Input Audio Buffer of length (%d)...", LOG_SENDER, __func__, inputBuffer->GetLength());
    AudioBuffer inputFrame = *_InputAudioFrame;
    AudioInputBuffer input = *inputBuffer;
    size_t inputFrameIndex = 0,
           outputBufferIndex = 0,
           inputFrameLength = inputFrame.GetLength(),
           lastFrameIndex = inputFrameLength - 1;

    _Logger->WriteLog("Processing Audio Frames of size (%d)...", LOG_SENDER, __func__, inputFrameIndex);
    inputFrame[inputFrameIndex] = input[inputFrameIndex];
    for (size_t sampleCounter = 1; sampleCounter < inputBuffer->GetLength(); sampleCounter++) {
        inputFrameIndex = sampleCounter % inputFrameLength;
        _Logger->WriteLog("Loading Input Buffer Index (%d) into Audio Frame Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, sampleCounter, inputFrameIndex);

        sample value = input[sampleCounter];
        inputFrame[inputFrameIndex] = value;
        _Logger->WriteLog("Successfully loaded Input Sample Value (%f) from Input Buffer Index (%d) into Audio Frame Index (%d).", LOG_SENDER, __func__, LogLevel::VERBOSE, value, sampleCounter, inputFrameIndex);

        if (inputFrameIndex == lastFrameIndex) {
            _Logger->WriteLog("Processing Full Audio Frame ending on Input Buffer Index (%d)...", LOG_SENDER, __func__, sampleCounter);
            ProcessInputAudioFrame();
            outputBufferIndex = ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
            _Logger->WriteLog("Successfully processed Full Audio Frame ending on Input Buffer Index (%d).", LOG_SENDER, __func__, sampleCounter);
        }
    }

    if (inputFrameIndex != lastFrameIndex) {
        _Logger->WriteLog("Padding Incomplete Audio Frame with zeros from Index (%d)...", LOG_SENDER, __func__, inputFrameIndex);
        inputFrame.ResetData(inputFrameIndex);

        _Logger->WriteLog("Processing Final Incomplete Audio Frame...", LOG_SENDER, __func__);
        ProcessInputAudioFrame();
        ProcessOutputAudioFrame(outputBufferIndex, outputBuffer);
        _Logger->WriteLog("Successfully processed Final Incomplete Audio Frame!", LOG_SENDER, __func__);
    }

    _Logger->WriteLog("Successfully processed Input Audio Buffer of length (%d)!", LOG_SENDER, __func__, inputBuffer->GetLength());
    return 0;
}

void MenrvaEffectsEngine::SetEffectEnabled(uint8_t effectIndex, bool enabled) {
    _Logger->WriteLog("Setting Enabled Flag on Effect Index : %d to : %d", LOG_SENDER, __func__, effectIndex, enabled);
    if (effectIndex >= EFFECTS_LENGTH) {
        _Logger->WriteLog("Skipping Setting Enabled Flag on Effect Index : %d.  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->Enabled = enabled;
    _Logger->WriteLog("Successfully set Enabled Flag on Effect Index : %d to : %d", LOG_SENDER, __func__, effectIndex, enabled);
}

void MenrvaEffectsEngine::ConfigureEffectSetting(uint8_t effectIndex, char* settingName,
                                                 void* value) {
    _Logger->WriteLog("Setting Effect Configuration : %s on Effect Index : %d", LOG_SENDER, __func__, settingName, effectIndex);
    if (effectIndex >= sizeof(_MenrvaEffects)) {
        _Logger->WriteLog("Skipping Setting Effect Configuration : %s on Effect Index : %d.  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, settingName, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->ConfigureSetting(settingName, value);
    _Logger->WriteLog("Successfully set Effect Configuration : %s on Effect Index : %d.", LOG_SENDER, __func__, settingName, effectIndex);
}

void MenrvaEffectsEngine::ProcessInputAudioFrame() {
    _Logger->WriteLog("Processing Input Audio Frame...", LOG_SENDER, __func__);
    for (EffectBase* effect : _MenrvaEffects) {
        if (effect->Enabled) {
            _Logger->WriteLog("Processing Effect : %s", LOG_SENDER, __func__, effect->NAME.c_str(), LogLevel::VERBOSE);
            // TODO : Ensure that Effect's Processing will aggregate (need to output to the next effect's input buffer; probably need to re-think managing enabled effects in a separate vector collection, all effects except the last in collection will overwrite Input Buffer and only last effect will overwrite Output Buffer)
            effect->Process(_InputAudioFrame, _OutputAudioFrame);
        }
        else {
            _Logger->WriteLog("Skipping Effect : %s.  Effect Disabled.", LOG_SENDER, __func__, LogLevel::VERBOSE);
        }
    }
    _Logger->WriteLog("Successfully processed Input Audio Frame!", LOG_SENDER, __func__);
}

size_t MenrvaEffectsEngine::ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer* outputBuffer) {
    _Logger->WriteLog("Processing Output Audio Frame...", LOG_SENDER, __func__);
    AudioBuffer outputFrame = *_OutputAudioFrame;
    for (size_t outputCounter = 0; outputCounter < outputBuffer->GetLength(); outputCounter++) {
        sample value = outputFrame[outputCounter];
        _Logger->WriteLog("Processing Output Audio Frame Index (%d) with Value (%f)...", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter, value);
        outputBuffer->SetValue(outputCounter + startOutputIndex, value);
        _Logger->WriteLog("Successfully processed Output Audio Frame Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, outputCounter);
    }

    _Logger->WriteLog("Successfully processed Output Audio Frame!", LOG_SENDER, __func__);
    return startOutputIndex + DSP_FRAME_LENGTH;
}
