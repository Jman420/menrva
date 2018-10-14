//
// Created by jgiannone on 10/12/2018.
//

#include "menrva_effects_engine.h"

void MenrvaEffectsEngine::Reset() {}

int MenrvaEffectsEngine::Process(audio_buffer_t *in, audio_buffer_t *out) {
    for (MenrvaEffect *effect : menrvaEffects) {
        if (effect->enabled) {
            effect->process(in, out);
        }
    }

    return 0;
}

void MenrvaEffectsEngine::SetEffectEnabled(int effectIndex, bool enabled) {
    if (effectIndex >= sizeof(menrvaEffects)) {
        return;
    }

    MenrvaEffect *effect = menrvaEffects[effectIndex];
    effect->enabled = enabled;
}

void MenrvaEffectsEngine::ConfigureEffectSetting(int effectIndex, char *settingName, void *value) {
    // TODO : Implement logic to configure effects
}
