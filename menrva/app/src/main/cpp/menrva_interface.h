//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_MENRVA_INTERFACE_H
#define MENRVA_MENRVA_INTERFACE_H

#include "aosp/hardware/audio_effect.h"
#include "aosp/system/audio_effect.h"

#include "menrva_effects_engine.h"

// Effect Engine Module Definition
struct menrva_engine_context {
    MenrvaEffectsEngine *effectsEngine;
    const struct effect_interface_s *control_interface;
    effect_config_t *config;

    int moduleState;
};

// TODO : Draft Module State Constants

// TODO : Draft Module Command Constants

#endif //MENRVA_MENRVA_INTERFACE_H
