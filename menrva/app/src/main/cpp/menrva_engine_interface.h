//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_MENRVA_ENGINE_INTERFACE_H
#define MENRVA_MENRVA_ENGINE_INTERFACE_H

#include "aosp/hardware/audio_effect.h"

class MenrvaEngineInterface {
public:
    static int Process(effect_handle_t self, audio_buffer_t *in, audio_buffer_t *out);
    static int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData,
                uint32_t *replySize, void *pReplyData);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t *pDescriptor);

    static const effect_descriptor_t effectDescriptor;

private:
    MenrvaEngineInterface();  // This is a static class
};

#endif //MENRVA_MENRVA_ENGINE_INTERFACE_H
