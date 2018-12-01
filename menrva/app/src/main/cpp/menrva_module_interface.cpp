// Author : Jman420

#include "menrva_module_interface.h"

#define LOG_TAG "MenrvaModuleInterface"
#define LOG_NDEBUG 0

const effect_interface_s MenrvaModuleInterface::engineInterface =
{
    MenrvaEngineInterface::Process,
    MenrvaEngineInterface::Command,
    MenrvaEngineInterface::GetDescriptorFromModule,
    NULL
};

int MenrvaModuleInterface::CreateModule(const effect_uuid_t *uuid, int32_t sessionId __unused,
                                        int32_t ioId __unused, effect_handle_t *pHandle) {
    if (pHandle == NULL || uuid == NULL) {
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaEngineInterface::effectDescriptor.uuid, sizeof(*uuid)) != 0) {
        return -EINVAL;
    }

    ALOGV("Creating module context...");
    menrva_module_context *context = new menrva_module_context;
    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_UNINITIALIZED;
    ALOGV("Initializing context...");
    InitModule(context);

    *pHandle = (effect_handle_t)context;
    ALOGV("Successfully Created Module!");
    return 0;
}

int MenrvaModuleInterface::InitModule(menrva_module_context *context) {
    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING;
    context->effectsEngine = new MenrvaEffectsEngine;
    context->itfe = &engineInterface;

    // TODO : Configure any necessary parameters

    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_READY;
    return 0;
}

int MenrvaModuleInterface::ReleaseModule(effect_handle_t moduleHandle) {
    menrva_module_context* module = (menrva_module_context*)moduleHandle;

    if (module == NULL) {
        return -EINVAL;
    }

    module->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_RELEASING;
    delete module->effectsEngine;
    delete module;

    return 0;
}

int MenrvaModuleInterface::GetDescriptorFromUUID(const effect_uuid_t *uuid,
                                                 effect_descriptor_t *pDescriptor) {
    ALOGV("Getting Descriptor by UUID...");
    if (pDescriptor == NULL || uuid == NULL) {
        ALOGV("Invalid Descriptor Pointer or UUID!");
        return -EINVAL;
    }
    else if (memcmp(uuid, &MenrvaEngineInterface::effectDescriptor.uuid, sizeof(*uuid)) != 0) {
        ALOGV("Incorrect UUID provided!");
        return -EINVAL;
    }

    ALOGV("Returning Effect Descriptor pointer...");
    *pDescriptor = MenrvaEngineInterface::effectDescriptor;
    return 0;
}

MenrvaModuleInterface::MenrvaModuleInterface() {}

// Required Exported Member for Android Audio Framework Entry Point
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        .tag = AUDIO_EFFECT_LIBRARY_TAG,
        .version = EFFECT_LIBRARY_API_VERSION,
        .name = MenrvaEngineInterface::effectDescriptor.name,
        .implementor = MenrvaEngineInterface::effectDescriptor.implementor,
        .create_effect = MenrvaModuleInterface::CreateModule,
        .release_effect = MenrvaModuleInterface::ReleaseModule,
        .get_descriptor = MenrvaModuleInterface::GetDescriptorFromUUID,
    };
}
