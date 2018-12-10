// Author : Jman420

#ifndef MENRVA_MENRVA_MODULE_INTERFACE_H
#define MENRVA_MENRVA_MODULE_INTERFACE_H

#include <string>

#include "boost/di.hpp"

#include "aosp/aosp_audio_effect_defs.h"

#include "effects_engine.h"

enum MenrvaModuleStatus {
    MENRVA_MODULE_UNINITIALIZED,
    MENRVA_MODULE_INITIALIZING,
    MENRVA_MODULE_READY,
    MENRVA_MODULE_RELEASING,
};

// Expected structure passed as effect_handle_t; Represents an instance of a MenrvaModule
struct menrva_module_context {
    __unused const effect_interface_s* itfe;
    effect_config_t* config;

    MenrvaEffectsEngine* effectsEngine;
    MenrvaModuleStatus moduleStatus;
};

// Represents the public interface for interacting with the Menrva Audio Effects Module
class MenrvaModuleInterface {
public:
    // Represents the public interface for interacting with the Menrva Audio Effects Engine
    static const effect_interface_s engineInterface;

    static int CreateModule(const effect_uuid_t* uuid, int32_t sessionId, int32_t ioId,
                            effect_handle_t* pHandle);
    static int InitModule(menrva_module_context* context);
    static int ReleaseModule(effect_handle_t moduleHandle);
    static int GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor);

private:
    // Boost.DI
    static const auto INJECTOR;

    static const std::string LOG_TAG;
    // TODO : Convert Logger to an Instantiated Class & Add a Private Static Member initialized from DI

    // Private Constructor to prevent instantiation of Static Class
    MenrvaModuleInterface();
};

#endif //MENRVA_MENRVA_MODULE_INTERFACE_H
