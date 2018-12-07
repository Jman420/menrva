// Author : Jman420

#include "menrva_module_interface.h"

const std::string MenrvaModuleInterface::LOG_TAG = "Menrva-ModuleInterface - ";

const effect_interface_s MenrvaModuleInterface::engineInterface =
{
    MenrvaEngineInterface::Process,
    MenrvaEngineInterface::Command,
    MenrvaEngineInterface::GetDescriptorFromModule,
    NULL
};

int MenrvaModuleInterface::CreateModule(const effect_uuid_t* uuid, int32_t sessionId __unused,
                                        int32_t ioId __unused, effect_handle_t* pHandle) {
    std::string logPrefix = LOG_TAG + "CreateModule()";
    LogMngr::LogMngr::WriteLog("Creating Menrva Module...", logPrefix);

    if (pHandle == NULL) {
        LogMngr::LogMngr::WriteLog("Invalid Effect Handle Pointer provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == NULL) {
        LogMngr::WriteLog("Invalid Effect UUID provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaEngineInterface::effectDescriptor.uuid, sizeof(*uuid)) != 0) {
        std::string uuid = MenrvaEngineInterface::engineUUID;
        std::string msg = "Incorrect Effect UUID provided. Does not match Menrva UUID : " + uuid;
        LogMngr::WriteLog(msg, logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }

    LogMngr::WriteLog("Creating Menrva Context...", logPrefix);
    menrva_module_context* context = new menrva_module_context;
    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_UNINITIALIZED;
    LogMngr::WriteLog("Initializing Menrva Context...", logPrefix);
    InitModule(context);

    *pHandle = (effect_handle_t)context;
    LogMngr::WriteLog("Successfully Created Menrva Module!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::InitModule(menrva_module_context* context) {
    std::string logPrefix = LOG_TAG + "InitModule()";
    LogMngr::WriteLog("Creating Menrva Effects Engine & Interface...", logPrefix);
    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING;
    context->effectsEngine = new MenrvaEffectsEngine;
    context->itfe = &engineInterface;

    // TODO : Configure any necessary parameters
    //LogMngr::WriteLog("Setting up Menrva Effects Engine Parameters...", logPrefix);

    context->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_READY;
    LogMngr::WriteLog("Successfully Initialized Menrva Context!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::ReleaseModule(effect_handle_t moduleHandle) {
    std::string logPrefix = LOG_TAG + "ReleaseModule()";
    LogMngr::WriteLog("Releasing Menrva Module...", logPrefix);
    menrva_module_context* module = (menrva_module_context*)moduleHandle;

    if (module == NULL) {
        LogMngr::WriteLog("Invalid Module Provided.  Provided Module is not a Menrva Module.", logPrefix);
        return -EINVAL;
    }

    LogMngr::WriteLog("Deleting Effects Engine & Module Pointers...", logPrefix);
    module->moduleStatus = MenrvaModuleStatus::MENRVA_MODULE_RELEASING;
    delete module->effectsEngine;
    delete module;

    LogMngr::WriteLog("Successfully Released Menrva Module!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::GetDescriptorFromUUID(const effect_uuid_t* uuid,
                                                 effect_descriptor_t* pDescriptor) {
    std::string logPrefix = LOG_TAG + "GetDescriptorFromUUID()";
    LogMngr::WriteLog("Getting Descriptor by UUID...", logPrefix);
    if (pDescriptor == NULL) {
        LogMngr::WriteLog("Invalid Descriptor Pointer provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == NULL) {
        LogMngr::WriteLog("Invalid Effect UUID provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaEngineInterface::effectDescriptor.uuid, sizeof(*uuid)) != 0) {
        std::string uuid = MenrvaEngineInterface::engineUUID;
        std::string msg = "Incorrect Effect UUID provided. Does not match Menrva UUID : " + uuid;
        LogMngr::WriteLog(msg, logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }

    *pDescriptor = MenrvaEngineInterface::effectDescriptor;
    LogMngr::WriteLog("Returning Effect Descriptor pointer!", logPrefix);
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
