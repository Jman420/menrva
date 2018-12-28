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

#ifndef MENRVA_MENRVA_EFFECTS_ENGINE_H
#define MENRVA_MENRVA_EFFECTS_ENGINE_H

#include "../aosp/aosp_audio_effect_defs.h"
#include "../abstracts/effect_base.h"
#include "../effects/equalizer.h"
#include "../effects/bass_boost.h"
#include "../effects/stereo_widener.h"

enum MenrvaEngineStatus {
    MENRVA_ENGINE_UNINITIALIZED,
    MENRVA_ENGINE_INITIALIZING,
    MENRVA_ENGINE_UNCONFIGURED,
    MENRVA_ENGINE_READY,
    MENRVA_ENGINE_DISABLED,
    MENRVA_ENGINE_ENABLED,
};

class MenrvaEffectsEngine {
public:
    MenrvaEngineStatus engineStatus;
    audio_buffer_t* workingBuffer;

    MenrvaEffectsEngine();
    void ResetEffects();
    int Process(AudioBuffer* in, AudioBuffer* out);
    void SetEffectEnabled(int effectIndex, bool enabled);
    void ConfigureEffectSetting(int effectIndex, char settingName[], void* value);

private:
    EffectBase* menrvaEffects[3] = {
            new Equalizer(),
            new BassBoost(),
            new StereoWidener(),
    };
};

#endif //MENRVA_MENRVA_EFFECTS_ENGINE_H