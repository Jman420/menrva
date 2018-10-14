// Author : Jman420

#ifndef MENRVA_BASS_BOOST_H
#define MENRVA_BASS_BOOST_H

#include "menrva_effect.h"

class BassBoost : public MenrvaEffect {
    void Process(audio_buffer_t *in, audio_buffer_t *out);
    void ResetConfig();
    void ConfigureSetting(char *settingName, void *value);
};

#endif //MENRVA_BASS_BOOST_H
