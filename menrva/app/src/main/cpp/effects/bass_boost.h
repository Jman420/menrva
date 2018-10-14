//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_BASS_BOOST_H
#define MENRVA_BASS_BOOST_H

#include "menrva_effect.h"

class BassBoost : public MenrvaEffect {
    void process(audio_buffer_t *in, audio_buffer_t *out);
};

#endif //MENRVA_BASS_BOOST_H
