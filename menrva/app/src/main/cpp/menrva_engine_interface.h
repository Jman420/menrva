// Author : Jman420

#ifndef MENRVA_MENRVA_ENGINE_INTERFACE_H
#define MENRVA_MENRVA_ENGINE_INTERFACE_H

#include "aosp/hardware/audio_effect.h"

// Represents the public interface for interacting with the Menrva Audio Effects Engine
class MenrvaEngineInterface {
public:
    // Represents the Descriptor for the Menrva Effects Engine
    static const effect_descriptor_t effectDescriptor;

    static int Process(effect_handle_t self, audio_buffer_t *in, audio_buffer_t *out);
    static int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData,
                uint32_t *replySize, void *pReplyData);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t *pDescriptor);

private:
    // Private Constructor to prevent instantiation of Static Class
    MenrvaEngineInterface();
};

#endif //MENRVA_MENRVA_ENGINE_INTERFACE_H
