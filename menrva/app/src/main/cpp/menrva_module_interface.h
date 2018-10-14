/*
 * Author : Jman420
 * Description : Represents the public interface for interacting with the Menrva Audio Effects
 *   Module
 */

#ifndef MENRVA_MENRVA_MODULE_INTERFACE_H
#define MENRVA_MENRVA_MODULE_INTERFACE_H

#include "aosp/hardware/audio_effect.h"

#include "menrva_effects_engine.h"

struct menrva_module_context {
    MenrvaEffectsEngine *effectsEngine;
    const struct effect_interface_s *control_interface;
    effect_config_t *config;

    int moduleState;
};

class MenrvaModuleInterface {
public:
    static const effect_interface_s control_interface;
    static int CreateModule(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId,
                            effect_handle_t *pHandle);
    static int InitModule(menrva_module_context *context);
    static int ReleaseModule(effect_handle_t moduleHandle);
    static int GetDescriptorFromUUID(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor);

private:
    MenrvaModuleInterface();  // This is a static class
};

#endif //MENRVA_MENRVA_MODULE_INTERFACE_H
