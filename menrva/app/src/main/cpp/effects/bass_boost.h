// Author : Jman420

#ifndef MENRVA_BASS_BOOST_H
#define MENRVA_BASS_BOOST_H

#include "ieffect.h"

class BassBoost : public IEffect {
public:
    BassBoost();
    void Process(audio_buffer_t* in, audio_buffer_t* out);
    void ResetConfig();
    void ConfigureSetting(char* settingName, void* value);

private:

};

#endif //MENRVA_BASS_BOOST_H
