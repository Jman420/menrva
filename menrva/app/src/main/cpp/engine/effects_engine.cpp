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

MenrvaEffectsEngine::MenrvaEffectsEngine() {
    engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_UNINITIALIZED;
    ResetEffects();
}

void MenrvaEffectsEngine::ResetEffects() {
    engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_INITIALIZING;

    workingBuffer = (audio_buffer_t*)malloc(sizeof(audio_buffer_t));
    for (EffectBase* effect : menrvaEffects) {
        effect->ResetConfig();
    }
}

int MenrvaEffectsEngine::Process(audio_buffer_t* in, audio_buffer_t* out) {

    for (EffectBase* effect : menrvaEffects) {
        if (effect->Enabled) {
            effect->Process(in, out);
        }
    }

    // TODO : Write final processed buffer to out buffer

    return 0;
}

void MenrvaEffectsEngine::SetEffectEnabled(int effectIndex, bool enabled) {
    if (effectIndex >= sizeof(menrvaEffects)) {
        return;
    }

    EffectBase* effect = menrvaEffects[effectIndex];
    effect->Enabled = enabled;
}

void MenrvaEffectsEngine::ConfigureEffectSetting(int effectIndex, char* settingName, void* value) {
    if (effectIndex >= sizeof(menrvaEffects)) {
        return;
    }

    EffectBase* effect = menrvaEffects[effectIndex];
    effect->ConfigureSetting(settingName, value);
}
