//
// Created by jgiannone on 10/12/2018.
//

#ifndef MENRVA_MENRVA_EFFECTS_ENGINE_H
#define MENRVA_MENRVA_EFFECTS_ENGINE_H

#include "aosp/hardware/audio_effect.h"

#include "effects/menrva_effect.h"
#include "effects/equalizer.h"
#include "effects/bass_boost.h"
#include "effects/stereo_widener.h"

class MenrvaEffectsEngine {
public:
    void Reset();
    int Process(audio_buffer_t *in, audio_buffer_t *out);
    void SetEffectEnabled(int effectIndex, bool enabled);
    void ConfigureEffectSetting(int effectIndex, char settingName[], void *value);

private:
    MenrvaEffect *menrvaEffects[3] = {
            new Equalizer(),
            new BassBoost(),
            new StereoWidener(),
    };
};

#endif //MENRVA_MENRVA_EFFECTS_ENGINE_H
