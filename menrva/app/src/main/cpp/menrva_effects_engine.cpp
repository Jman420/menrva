// Author : Jman420

#include <cstdlib>

#include "menrva_effects_engine.h"

MenrvaEffectsEngine::MenrvaEffectsEngine() {
    engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_UNINITIALIZED;
    ResetEffects();
}

void MenrvaEffectsEngine::ResetEffects() {
    engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_INITIALIZING;

    workingBuffer = (audio_buffer_t*)malloc(sizeof(audio_buffer_t));
    for (MenrvaEffect* effect : menrvaEffects) {
        effect->ResetConfig();
    }
}

int MenrvaEffectsEngine::Process(audio_buffer_t* in, audio_buffer_t* out) {

    for (MenrvaEffect* effect : menrvaEffects) {
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

    MenrvaEffect* effect = menrvaEffects[effectIndex];
    effect->Enabled = enabled;
}

void MenrvaEffectsEngine::ConfigureEffectSetting(int effectIndex, char* settingName, void* value) {
    if (effectIndex >= sizeof(menrvaEffects)) {
        return;
    }

    MenrvaEffect* effect = menrvaEffects[effectIndex];
    effect->ConfigureSetting(settingName, value);
}
