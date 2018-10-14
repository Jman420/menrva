// Author : Jman420

#ifndef MENRVA_MENRVA_EFFECTS_ENGINE_H
#define MENRVA_MENRVA_EFFECTS_ENGINE_H

#include "aosp/hardware/audio_effect.h"

#include "effects/menrva_effect.h"
#include "effects/equalizer.h"
#include "effects/bass_boost.h"
#include "effects/stereo_widener.h"

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
    audio_buffer_t *workingBuffer;

    MenrvaEffectsEngine();
    void ResetEffects();
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
