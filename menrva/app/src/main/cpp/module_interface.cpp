/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2018 Justin Giannone (aka Jman420)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cerrno>
#include "module_interface.h"
#include "engine/engine_interface.h"
#include "tools/service_locator.h"

const std::string MenrvaModuleInterface::LOG_TAG = "Menrva-ModuleInterface - ";
ServiceLocator* MenrvaModuleInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaModuleInterface::_Logger = _ServiceLocator->GetLogger();

const effect_interface_s MenrvaModuleInterface::EngineInterface =
{
    MenrvaEngineInterface::Process,
    MenrvaEngineInterface::Command,
    MenrvaEngineInterface::GetDescriptorFromModule,
    NULL
};

int MenrvaModuleInterface::CreateModule(const effect_uuid_t* uuid, int32_t sessionId __unused,
                                        int32_t ioId __unused, effect_handle_t* pHandle) {
    std::string logPrefix = LOG_TAG + "CreateModule()";
    _Logger->WriteLog("Creating Menrva Module...", logPrefix);

    if (pHandle == NULL) {
        _Logger->WriteLog("Invalid Effect Handle Pointer provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == NULL) {
        _Logger->WriteLog("Invalid Effect UUID provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaEngineInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        std::string engineUuid = MenrvaEngineInterface::EngineUUID;
        std::string msg = "Incorrect Effect UUID provided. Does not match Menrva UUID : " + engineUuid;
        _Logger->WriteLog(msg, logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Creating Menrva Context...", logPrefix);
    menrva_module_context* context = new menrva_module_context;
    context->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_UNINITIALIZED;
    _Logger->WriteLog("Initializing Menrva Context...", logPrefix);
    InitModule(context);

    *pHandle = (effect_handle_t)context;
    _Logger->WriteLog("Successfully Created Menrva Module!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::InitModule(menrva_module_context* context) {
    std::string logPrefix = LOG_TAG + "InitModule()";
    _Logger->WriteLog("Creating Menrva Effects Engine & Interface...", logPrefix);
    context->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING;
    context->EffectsEngine = new MenrvaEffectsEngine();
    context->itfe = &EngineInterface;
    context->InputBuffer = new AudioBuffer(_ServiceLocator->GetFftEngine());
    context->OutputBuffer = new AudioBuffer(_ServiceLocator->GetFftEngine());

    // TODO : Configure any necessary parameters
    //_Logger->WriteLog("Setting up Menrva Effects Engine Parameters...", logPrefix);

    context->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_READY;
    _Logger->WriteLog("Successfully Initialized Menrva Context!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::ReleaseModule(effect_handle_t moduleHandle) {
    std::string logPrefix = LOG_TAG + "ReleaseModule()";
    _Logger->WriteLog("Releasing Menrva Module...", logPrefix);
    menrva_module_context* module = (menrva_module_context*)moduleHandle;

    if (module == NULL) {
        _Logger->WriteLog("Invalid Module Provided.  Provided Module is not a Menrva Module.", logPrefix);
        return -EINVAL;
    }

    _Logger->WriteLog("Deleting Effects Engine & Module Pointers...", logPrefix);
    module->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_RELEASING;
    delete module->EffectsEngine;
    delete module->InputBuffer;
    delete module->OutputBuffer;
    delete module->config;
    delete module;

    _Logger->WriteLog("Successfully Released Menrva Module!", logPrefix);
    return 0;
}

int MenrvaModuleInterface::GetDescriptorFromUUID(const effect_uuid_t* uuid,
                                                 effect_descriptor_t* pDescriptor) {
    std::string logPrefix = LOG_TAG + "GetDescriptorFromUUID()";
    _Logger->WriteLog("Getting Descriptor by UUID...", logPrefix);
    if (pDescriptor == NULL) {
        _Logger->WriteLog("Invalid Descriptor Pointer provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == NULL) {
        _Logger->WriteLog("Invalid Effect UUID provided.", logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaEngineInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        std::string engineUuid = MenrvaEngineInterface::EngineUUID;
        std::string msg = "Incorrect Effect UUID provided. Does not match Menrva UUID : " + engineUuid;
        _Logger->WriteLog(msg, logPrefix, LogLevel::ERROR);
        return -EINVAL;
    }

    *pDescriptor = MenrvaEngineInterface::EffectDescriptor;
    _Logger->WriteLog("Returning Effect Descriptor pointer!", logPrefix);
    return 0;
}

MenrvaModuleInterface::MenrvaModuleInterface() {}

// Required Exported Member for Android Audio Framework Entry Point
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        .tag = AUDIO_EFFECT_LIBRARY_TAG,
        .version = EFFECT_LIBRARY_API_VERSION,
        .name = MenrvaEngineInterface::EffectDescriptor.name,
        .implementor = MenrvaEngineInterface::EffectDescriptor.implementor,
        .create_effect = MenrvaModuleInterface::CreateModule,
        .release_effect = MenrvaModuleInterface::ReleaseModule,
        .get_descriptor = MenrvaModuleInterface::GetDescriptorFromUUID,
    };
}
