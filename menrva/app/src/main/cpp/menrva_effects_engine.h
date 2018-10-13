//
// Created by jgiannone on 10/12/2018.
//

#ifndef MENRVA_MENRVA_EFFECT_H
#define MENRVA_MENRVA_EFFECT_H

#include "aosp/hardware/audio_effect.h"

class MenrvaEffectsEngine {
public:
    MenrvaEffectsEngine();
    ~MenrvaEffectsEngine();
    void reset();
    int process(audio_buffer_t *in, audio_buffer_t *out);
};

#endif //MENRVA_MENRVA_EFFECT_H
