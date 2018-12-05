// Author : Jman420

#ifndef MENRVA_MENRVA_EFFECT_H
#define MENRVA_MENRVA_EFFECT_H

#include "../aosp/system/audio_effect.h"

class MenrvaEffect {
public:
    bool Enabled = false;
    virtual void Process(audio_buffer_t* in, audio_buffer_t* out) = 0;
    virtual void ResetConfig() = 0;
    virtual void ConfigureSetting(char* settingName, void* value) = 0;
};

#endif //MENRVA_MENRVA_EFFECT_H
