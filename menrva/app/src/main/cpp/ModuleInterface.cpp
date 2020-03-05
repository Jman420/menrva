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
#include "ModuleInterface.h"
#include "engine/EngineInterface.h"
#include "tools/ServiceLocator.h"
#include "tools/StringOperations.h"

const std::string MenrvaModuleInterface::LOG_SENDER = "ModuleInterface";
ServiceLocator* MenrvaModuleInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaModuleInterface::_Logger = _ServiceLocator->GetLogger();

const effect_descriptor_t MenrvaModuleInterface::EffectDescriptor = {
        // UUID of to the OpenSL ES interface implemented by this effect (EFFECT_TYPE_nullptr)
        .type = { 0xec7178ec, 0xe5e1, 0x4432, 0xa3f4, { 0x46, 0x57, 0xe6, 0x79, 0x52, 0x10 } },
        // UUID for this particular implementation (http://www.itu.int/ITU-T/asn1/uuid.html)
        .uuid = { 0xa91fdfe4, 0xd09e, 0x11e8, 0xa8d5, { 0xf2, 0x80, 0x1f, 0x1b, 0x9f, 0xd1 } },
        // Version of the effect control API implemented
        .apiVersion = EFFECT_CONTROL_API_VERSION,
        // effect engine capabilities/requirements flags (see below)
        .flags = EFFECT_FLAG_TYPE_INSERT | EFFECT_FLAG_INSERT_FIRST,
        // CPU load indication
        .cpuLoad = 10,
        // Data Memory usage
        .memoryUsage = 1,
        // human readable effect name
        .name = "MenrvaEngine",
        // human readable effect implementor name
        .implementor = "Jman420"
};

const effect_interface_s MenrvaModuleInterface::EngineInterface =
{
    MenrvaEngineInterface::Process,
    MenrvaEngineInterface::Command,
    MenrvaModuleInterface::GetDescriptorFromModule,
    nullptr
};

const char* MenrvaModuleInterface::EffectTypeUUID = "ec7178ec-e5e1-4432-a3f4-4657e6795210";
const char* MenrvaModuleInterface::EngineUUID = "a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1";

int MenrvaModuleInterface::CreateModule(const effect_uuid_t* uuid, int32_t sessionId __unused, int32_t ioId __unused, effect_handle_t* pHandle) {
    _Logger->WriteLog("Creating Menrva Module...", LOG_SENDER, __func__);
    
    if (pHandle == nullptr) {
        _Logger->WriteLog("Invalid Effect Handle Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaModuleInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog(StringOperations::FormatString("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", MenrvaModuleInterface::EngineUUID),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Creating Menrva Context...", LOG_SENDER, __func__);
    auto context = new MenrvaModuleContext();
    context->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_UNINITIALIZED;
    InitModule(*context);

    *pHandle = (effect_handle_t)context;
    _Logger->WriteLog("Successfully Created Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

void MenrvaModuleInterface::InitModule(MenrvaModuleContext& context) {
    _Logger->WriteLog("Initializing Menrva Effects Engine & Interface...", LOG_SENDER, __func__);

    if (context.ModuleStatus > MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {
        _Logger->WriteLog("Menrva Effects Engine & Interface already Initialized!", LOG_SENDER, __func__);
        return;
    }

    context.ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING;
    context.EffectsEngine = new MenrvaEffectsEngine(_Logger, _ServiceLocator->GetFftEngine(), _ServiceLocator);
    context.itfe = &EngineInterface;

    // TODO : Configure any necessary default parameters
    //_Logger->WriteLog("Setting up Menrva Effects Engine Parameters...", logPrefix);

    context.ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_READY;
    _Logger->WriteLog("Successfully Initialized Menrva Context!", LOG_SENDER, __func__);
}

int MenrvaModuleInterface::ReleaseModule(effect_handle_t moduleHandle) {
    _Logger->WriteLog("Releasing Menrva Module...", LOG_SENDER, __func__);
    auto module = (MenrvaModuleContext*)moduleHandle;

    if (module == nullptr) {
        _Logger->WriteLog("Invalid Module Provided.  Provided Module is not a Menrva Module.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Deleting Effects Engine & Module Pointers...", LOG_SENDER, __func__);
    module->ModuleStatus = MenrvaModuleStatus::MENRVA_MODULE_RELEASING;
    delete module->EffectsEngine;
    delete module->InputBuffer;
    delete module->OutputBuffer;
    delete module;

    _Logger->WriteLog("Successfully Released Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

int MenrvaModuleInterface::GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor) {
    _Logger->WriteLog("Getting Descriptor from UUID...", LOG_SENDER, __func__);
    if (pDescriptor == nullptr) {
        _Logger->WriteLog("Invalid Descriptor Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &MenrvaModuleInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog(StringOperations::FormatString("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", MenrvaModuleInterface::EngineUUID),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -ENOENT;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *pDescriptor = MenrvaModuleInterface::EffectDescriptor;
    return 0;
}

int MenrvaModuleInterface::GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor) {
    _Logger->WriteLog("Getting Descriptor from Module Pointer...", LOG_SENDER, __func__);
    auto module = (MenrvaModuleContext*)self;
    if (module == nullptr || pDescriptor == nullptr) {
        _Logger->WriteLog("Invalid Module Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *pDescriptor = MenrvaModuleInterface::EffectDescriptor;
    return 0;
}

// Required Exported Member for Android Audio Framework Entry Point
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        .tag = AUDIO_EFFECT_LIBRARY_TAG,
        .version = EFFECT_LIBRARY_API_VERSION,
        .name = MenrvaModuleInterface::EffectDescriptor.name,
        .implementor = MenrvaModuleInterface::EffectDescriptor.implementor,
        .create_effect = MenrvaModuleInterface::CreateModule,
        .release_effect = MenrvaModuleInterface::ReleaseModule,
        .get_descriptor = MenrvaModuleInterface::GetDescriptorFromUUID,
    };
}
