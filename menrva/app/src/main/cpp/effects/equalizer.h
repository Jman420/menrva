// Author : Jman420

#ifndef MENRVA_EQUALIZER_H
#define MENRVA_EQUALIZER_H

#include "ieffect.h"

class Equalizer : public IEffect {
    void Process(audio_buffer_t* in, audio_buffer_t* out);
    void ResetConfig();
    void ConfigureSetting(char* settingName, void* value);
};

#endif //MENRVA_EQUALIZER_H
