//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_EQUALIZER_H
#define MENRVA_EQUALIZER_H

#include "menrva_effect.h"

class Equalizer : public MenrvaEffect {
    void process(audio_buffer_t *in, audio_buffer_t *out);
};

#endif //MENRVA_EQUALIZER_H
