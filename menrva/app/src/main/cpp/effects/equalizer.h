// Author : Jman420

#ifndef MENRVA_EQUALIZER_H
#define MENRVA_EQUALIZER_H

#include "../abstracts/effect_base.h"

class Equalizer : public EffectBase {
    void Process(audio_buffer_t* in, audio_buffer_t* out);
    void ResetConfig();
    void ConfigureSetting(char* settingName, void* value);
};

#endif //MENRVA_EQUALIZER_H
