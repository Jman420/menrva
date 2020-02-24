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
#include "AndroidInterface.h"
#include "../../engine/CommandProcessor.h"
#include "../../tools/CommandIdCalculator.h"
#include "command_handlers/Android_InitEngine_Handler.h"
#include "command_handlers/Android_GetConfig_Handler.h"
#include "command_handlers/Android_SetConfig_Handler.h"
#include "command_handlers/Android_ResetBuffers_Handler.h"
#include "command_handlers/Android_EnableEngine_Handler.h"
#include "command_handlers/Android_DisableEngine_Handler.h"

const std::string ModuleInterface::LOG_SENDER = "ModuleInterface";
ServiceLocator* ModuleInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* ModuleInterface::_Logger = _ServiceLocator->GetLogger();

const char* ModuleInterface::EffectTypeUUID = "ec7178ec-e5e1-4432-a3f4-4657e6795210";
const char* ModuleInterface::EngineUUID = "a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1";

const effect_descriptor_t ModuleInterface::EffectDescriptor = {
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

const effect_interface_s ModuleInterface::EngineInterface =
{
        ModuleInterface::Process,
        ModuleInterface::Command,
        ModuleInterface::GetDescriptorFromModule,
        nullptr,
};

int ModuleInterface::CreateModule(const effect_uuid_t* uuid, int32_t sessionId __unused, int32_t ioId __unused, effect_handle_t* handlePtr) {
    _Logger->WriteLog("Creating Menrva Module...", LOG_SENDER, __func__);

    if (handlePtr == nullptr) {
        _Logger->WriteLog("Invalid Effect Handle Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &ModuleInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", LOG_SENDER, __func__, LogLevel::ERROR, ModuleInterface::EngineUUID);
        return -EINVAL;
    }

    _Logger->WriteLog("Creating Menrva Context...", LOG_SENDER, __func__);
    auto context = new ModuleContext();
    context->Status = ModuleStatus::UNINITIALIZED;
    InitModule(context);

    *handlePtr = (effect_handle_t)context;
    _Logger->WriteLog("Successfully Created Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

void ModuleInterface::InitModule(ModuleContext* context) {
    _Logger->WriteLog("Initializing Menrva Effects Engine & Interface...", LOG_SENDER, __func__);

    if (context->Status > ModuleStatus::INITIALIZING) {
        _Logger->WriteLog("Menrva Effects Engine & Interface already Initialized!", LOG_SENDER, __func__);
        return;
    }

    AndroidModuleContext& androidContext = *static_cast<AndroidModuleContext*>(context);
    androidContext.Status = ModuleStatus::INITIALIZING;
    androidContext.Engine = new EffectsEngine(_Logger, _ServiceLocator->GetFftEngine(), _ServiceLocator);
    androidContext.itfe = &EngineInterface;

    _Logger->WriteLog("Injecting Android Host Command Handlers...", LOG_SENDER, __func__);
    CommandProcessor* commandProcessor = new CommandProcessor();
    handler_map& commandHandlerMap = *commandProcessor->GetCommandHandlerMap()->GetHandlerMap();
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_INIT, new Android_InitEngine_Handler(_Logger)));
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_GET_CONFIG, new Android_GetConfig_Handler(_Logger)));
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_SET_CONFIG, new Android_SetConfig_Handler(_Logger)));
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_RESET, new Android_ResetBuffers_Handler(_Logger)));
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_ENABLE, new Android_EnableEngine_Handler(_Logger)));
    commandHandlerMap.insert(handler_map::value_type(EFFECT_CMD_DISABLE, new Android_DisableEngine_Handler(_Logger)));
    androidContext.CommandProcessor = commandProcessor;  // IDE seems to think these types are incompatible, but compiler builds successfully?

    // TODO : Configure any necessary default parameters
    //_Logger->WriteLog("Setting up Menrva Effects Engine Parameters...", logPrefix);

    androidContext.Status = ModuleStatus::READY;
    _Logger->WriteLog("Successfully Initialized Menrva Context!", LOG_SENDER, __func__);
}

int ModuleInterface::ReleaseModule(effect_handle_t handle) {
    _Logger->WriteLog("Releasing Menrva Module...", LOG_SENDER, __func__);
    auto module = (ModuleContext*)handle;

    if (module == nullptr) {
        _Logger->WriteLog("Invalid Module Provided.  Provided Module is not a Menrva Module.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Deleting Effects Engine & Module Pointers...", LOG_SENDER, __func__);
    module->Status = ModuleStatus::RELEASING;
    delete module->Config;
    delete module->Engine;
    delete module->InputBuffer;
    delete module->OutputBuffer;
    delete module;

    _Logger->WriteLog("Successfully Released Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

int ModuleInterface::GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* descriptorPtr) {
    _Logger->WriteLog("Getting Descriptor from UUID...", LOG_SENDER, __func__);
    if (descriptorPtr == nullptr) {
        _Logger->WriteLog("Invalid Descriptor Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &ModuleInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", LOG_SENDER, __func__, LogLevel::ERROR, ModuleInterface::EngineUUID);
        return -ENOENT;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *descriptorPtr = ModuleInterface::EffectDescriptor;
    return 0;
}

int ModuleInterface::GetDescriptorFromModule(effect_handle_t handle, effect_descriptor_t* descriptorPtr) {
    _Logger->WriteLog("Getting Descriptor from Module Pointer...", LOG_SENDER, __func__);
    auto module = (ModuleContext*)handle;
    if (module == nullptr || descriptorPtr == nullptr) {
        _Logger->WriteLog("Invalid Module Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *descriptorPtr = ModuleInterface::EffectDescriptor;
    return 0;
}

int ModuleInterface::Process(effect_handle_t handle, audio_buffer_t* inBufferPtr, audio_buffer_t* outBufferPtr) {
    _Logger->WriteLog("Buffer Input Received...", LOG_SENDER, __func__);
    AndroidModuleContext& context = *(AndroidModuleContext*)handle;
    ModuleConfig& config = *context.Config;
    audio_buffer_t& inBuffer = *inBufferPtr;
    audio_buffer_t& outBuffer = *outBufferPtr;

    if (context.Status == ModuleStatus::RELEASING) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is in Releasing Status.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -ENODATA;
    }
    if (context.Status != ModuleStatus::READY) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is not in Ready Status.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }
    if (inBuffer.frameCount != outBuffer.frameCount) {
        _Logger->WriteLog("Skipping Processing Buffer.  Input Frame Count (%u) does not match Output Frame Count (%u).", LOG_SENDER, __func__, LogLevel::ERROR, inBuffer.frameCount, outBuffer.frameCount);
        return -EINVAL;
    }

    uint32_t channelLength = config.ChannelLength;
    _Logger->WriteLog("Input Buffer Frame Length (%u) and Channel Length (%u).", LOG_SENDER, __func__, inBuffer.frameCount, channelLength);
    _Logger->WriteLog("Output Buffer Frame Length (%u and Channel Length (%u).", LOG_SENDER, __func__, outBuffer.frameCount, channelLength);
    _Logger->WriteLog("Setting up AudioBuffer Data from Input & Output Buffers...", LOG_SENDER, __func__);
    switch (config.Format) {
        case PCM_16:
            context.InputBuffer->SetData(inBuffer.s16, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.s16, channelLength, outBuffer.frameCount);
            break;

        case PCM_32:
            context.InputBuffer->SetData(inBuffer.s32, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.s32, channelLength, outBuffer.frameCount);
            break;

        case PCM_Float:
            context.InputBuffer->SetData(inBuffer.f32, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.f32, channelLength, outBuffer.frameCount);
            break;

        default:
            _Logger->WriteLog("Skipping Processing Buffer.  Invalid Audio Format Provided (%d).", LOG_SENDER, __func__, LogLevel::ERROR, config.Format);
            return -EINVAL;
    }

    _Logger->WriteLog("Passing AudioBuffers to EffectsEngine for Processing...", LOG_SENDER, __func__);
    int result = context.Engine->Process(*context.InputBuffer, *context.OutputBuffer);

    _Logger->WriteLog("EffectsEngine finished Processing with Result (%d)!", LOG_SENDER, __func__, result);
    return result;
}

int ModuleInterface::Command(effect_handle_t handle, uint32_t cmdCode, uint32_t cmdSize, void* cmdDataPtr, uint32_t* replySize, void* replyDataPtr) {
    _Logger->WriteLog("Command Input Received...", LOG_SENDER, __func__);
    ModuleContext& context = *(ModuleContext*)handle;

    if (context.Status == ModuleStatus::RELEASING || context.Status == ModuleStatus::INITIALIZING) {
        _Logger->WriteLog("Skipping Processing Command.  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Passing Command Data to CommandProcessor for Processing...", LOG_SENDER, __func__);
    int result = context.CommandProcessor->Process(context, cmdCode, cmdSize, cmdDataPtr, replySize, replyDataPtr);

    _Logger->WriteLog("Finished Processing Command with Result (%d).", LOG_SENDER, __func__, LogLevel::VERBOSE, result);
    return result;
}

// Required Exported Member for Android Audio Framework Entry Point
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        .tag = AUDIO_EFFECT_LIBRARY_TAG,
        .version = EFFECT_LIBRARY_API_VERSION,
        .name = ModuleInterface::EffectDescriptor.name,
        .implementor = ModuleInterface::EffectDescriptor.implementor,
        .create_effect = ModuleInterface::CreateModule,
        .release_effect = ModuleInterface::ReleaseModule,
        .get_descriptor = ModuleInterface::GetDescriptorFromUUID,
    };
}
