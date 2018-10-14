//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_STEREO_WIDENER_H
#define MENRVA_STEREO_WIDENER_H

#include "menrva_effect.h"

class StereoWidener : public MenrvaEffect {
    void process(audio_buffer_t *in, audio_buffer_t *out);
};

#endif //MENRVA_STEREO_WIDENER_H
