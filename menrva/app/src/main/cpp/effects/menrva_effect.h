//
// Created by jgiannone on 10/12/2018.
//

#ifndef MENRVA_MENRVA_EFFECT_H
#define MENRVA_MENRVA_EFFECT_H

#include "../aosp/system/audio_effect.h"

class MenrvaEffect {
public:
    bool enabled = false;
    virtual void process(audio_buffer_t *in, audio_buffer_t *out) = 0;
};

#endif //MENRVA_MENRVA_EFFECT_H
