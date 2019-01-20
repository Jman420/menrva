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

#ifndef MENRVA_EFFECTS_ENGINE_H
#define MENRVA_EFFECTS_ENGINE_H

#include "../aosp/aosp_audio_effect_defs.h"
#include "../abstracts/effect_base.h"
#include "../effects/equalizer.h"
#include "../effects/bass_boost.h"
#include "../effects/stereo_widener.h"
#include "../abstracts/logger_base.h"
#include "../abstracts/logging_base.h"
#include "../audio/audio_input_buffer.h"
#include "../audio/audio_output_buffer.h"

enum MenrvaEngineStatus {
    MENRVA_ENGINE_UNINITIALIZED,
    MENRVA_ENGINE_INITIALIZING,
    MENRVA_ENGINE_UNCONFIGURED,
    MENRVA_ENGINE_READY,
    MENRVA_ENGINE_DISABLED,
    MENRVA_ENGINE_ENABLED,
};

class MenrvaEffectsEngine : public LoggingBase {
public:
    MenrvaEngineStatus _EngineStatus;

    MenrvaEffectsEngine(LoggerBase* logger, FftInterfaceBase* fftEngine, ServiceLocator* serviceLocator);
    ~MenrvaEffectsEngine();

    void ResetEffects(effect_config_t* bufferConfig);
    int SetBufferConfig(effect_config_t* bufferConfig);
    int Process(AudioInputBuffer* inputBuffer, AudioOutputBuffer* outputBuffer);
    void SetEffectEnabled(uint8_t effectIndex, bool enabled);
    void ConfigureEffectSetting(uint8_t effectIndex, char* settingName, void* value);

private:
    const static int EFFECTS_LENGTH = 3;

    EffectBase* _MenrvaEffects[EFFECTS_LENGTH];
    AudioBuffer* _InputAudioFrame;
    AudioBuffer* _OutputAudioFrame;

    void ProcessInputAudioFrame();
    size_t ProcessOutputAudioFrame(size_t startOutputIndex, AudioOutputBuffer* outputBuffer);
};

#endif //MENRVA_EFFECTS_ENGINE_H
