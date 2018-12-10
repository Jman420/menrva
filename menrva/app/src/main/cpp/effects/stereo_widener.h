// Author : Jman420

#ifndef MENRVA_STEREO_WIDENER_H
#define MENRVA_STEREO_WIDENER_H

#include "ieffect.h"

class StereoWidener : public IEffect {
    void Process(audio_buffer_t* in, audio_buffer_t* out);
    void ResetConfig();
    void ConfigureSetting(char* settingName, void* value);
};

#endif //MENRVA_STEREO_WIDENER_H
