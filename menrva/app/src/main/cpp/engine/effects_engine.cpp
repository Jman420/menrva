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

MenrvaEffectsEngine::MenrvaEffectsEngine(LoggerBase* logger, FftInterfaceBase* fftEngine)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_UNINITIALIZED;
    _MenrvaEffects[0] = new BassBoost(_Logger);
    _MenrvaEffects[1] = new StereoWidener(_Logger);
    _MenrvaEffects[2] = new Equalizer(_Logger);

    _WorkingInputBuffer = new AudioBuffer(fftEngine, DSP_FRAME_SIZE);
    _WorkingOutputBuffer = new AudioBuffer(fftEngine, DSP_FRAME_SIZE);
}

void MenrvaEffectsEngine::ResetEffects() {
    _Logger->WriteLog("Resetting Effects...", LOG_SENDER, __func__);
    _EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_INITIALIZING;

    for (EffectBase* effect : _MenrvaEffects) {
        _Logger->WriteLog("Resetting Effect : %s", LOG_SENDER, __func__, effect->NAME.c_str());
        effect->ResetConfig();
    }
    _Logger->WriteLog("Successfully Clear Effects!", LOG_SENDER, __func__);
}

int MenrvaEffectsEngine::SetBufferConfig(effect_config_t config) {
    // TODO : Implement Logic to Configure Effects
    return 0;
}

int MenrvaEffectsEngine::Process(AudioInputBuffer* inputBuffer, AudioOutputBuffer* outputBuffer) {
    _Logger->WriteLog("Processing AudioBuffer length : %d", LOG_SENDER, __func__, inputBuffer->GetLength());

    // TODO : Load Input Buffer into Frames of Expected Length (1024) in WorkingInputBuffer

    for (EffectBase* effect : _MenrvaEffects) {
        if (effect->Enabled) {
            _Logger->WriteLog("Processing Effect : %s", LOG_SENDER, __func__, effect->NAME.c_str());
            effect->Process(_WorkingInputBuffer, _WorkingOutputBuffer);
        }
        else {
            _Logger->WriteLog("Skipping Effect : %s.  Effect Disabled.", LOG_SENDER, __func__);
        }
    }

    // TODO : Move Processed Frames from WorkingOutputBuffer into Output Buffer

    _Logger->WriteLog("Successfully Processed AudioBuffer length : %d.  Output AudioBuffer length : %d", LOG_SENDER, __func__, inputBuffer->GetLength());
    return 0;
}

void MenrvaEffectsEngine::SetEffectEnabled(unsigned int effectIndex, bool enabled) {
    _Logger->WriteLog("Setting Enabled Flag on Effect Index : %d to : %d", LOG_SENDER, __func__, effectIndex, enabled);
    if (effectIndex >= EFFECTS_LENGTH) {
        _Logger->WriteLog("Skipping Setting Enabled Flag on Effect Index : %d.  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->Enabled = enabled;
    _Logger->WriteLog("Successfully set Enabled Flag on Effect Index : %d to : %d", LOG_SENDER, __func__, effectIndex, enabled);
}

void MenrvaEffectsEngine::ConfigureEffectSetting(unsigned int effectIndex, char* settingName, void* value) {
    _Logger->WriteLog("Setting Effect Configuration : %s on Effect Index : %d", LOG_SENDER, __func__, settingName, effectIndex);
    if (effectIndex >= sizeof(_MenrvaEffects)) {
        _Logger->WriteLog("Skipping Setting Effect Configuration : %s on Effect Index : %d.  Index out of bounds.", LOG_SENDER, __func__, LogLevel::WARN, settingName, effectIndex);
        return;
    }

    EffectBase* effect = _MenrvaEffects[effectIndex];
    effect->ConfigureSetting(settingName, value);
    _Logger->WriteLog("Successfully set Effect Configuration : %s on Effect Index : %d.", LOG_SENDER, __func__, settingName, effectIndex);
}
