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
#include "../ServiceLocator.h"
#include "../../tools/StringOperations.h"
#include "../../engine/CommandProcessor.h"
#include "command_handlers/Android_InitEngine_Handler.h"
#include "command_handlers/Android_GetConfig_Handler.h"
#include "command_handlers/Android_SetConfig_Handler.h"
#include "command_handlers/Android_ResetBuffers_Handler.h"
#include "command_handlers/Android_EnableEngine_Handler.h"
#include "command_handlers/Android_DisableEngine_Handler.h"

const std::string AndroidInterface::LOG_SENDER = "AndroidInterface";
ServiceLocator* AndroidInterface::_ServiceLocator = new ServiceLocator();
LogWriterBase* AndroidInterface::_Logger = _ServiceLocator->GetLogger();

const effect_descriptor_t AndroidInterface::EffectDescriptor = {
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

// Required Exported Member for Android Audio Framework Entry Point
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
            .tag = AUDIO_EFFECT_LIBRARY_TAG,
            .version = EFFECT_LIBRARY_API_VERSION,
            .name = AndroidInterface::EffectDescriptor.name,
            .implementor = AndroidInterface::EffectDescriptor.implementor,
            .create_effect = AndroidInterface::CreateModule,
            .release_effect = AndroidInterface::ReleaseModule,
            .get_descriptor = AndroidInterface::GetDescriptorFromUUID,
    };
}

const effect_interface_s AndroidInterface::EngineInterface =
{
        AndroidInterface::Process,
        AndroidInterface::Command,
        AndroidInterface::GetDescriptorFromModule,
        nullptr
};

const char* AndroidInterface::EffectTypeUUID = "ec7178ec-e5e1-4432-a3f4-4657e6795210";
const char* AndroidInterface::EngineUUID = "a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1";

int AndroidInterface::CreateModule(const effect_uuid_t* uuid, int32_t sessionId __unused, int32_t ioId __unused, effect_handle_t* pHandle) {
    _Logger->WriteLog("Creating Menrva Module...", LOG_SENDER, __func__);
    
    if (pHandle == nullptr) {
        _Logger->WriteLog("Invalid Effect Handle Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &AndroidInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog(StringOperations::FormatString("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", AndroidInterface::EngineUUID),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Creating Command Processor and Adding Android Host Specific Command Handlers...", LOG_SENDER, __func__);
    CommandProcessor* commandProcessor = new CommandProcessor(_Logger);
    handler_map& handlerMap = *commandProcessor->GetCommandHandlerMap()->GetMap();
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_INIT, new Android_InitEngine_Handler(_Logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_GET_CONFIG, new Android_GetConfig_Handler(_Logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_SET_CONFIG, new Android_SetConfig_Handler(_Logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_RESET, new Android_ResetBuffers_Handler(_Logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_ENABLE, new Android_EnableEngine_Handler(_Logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_DISABLE, new Android_DisableEngine_Handler(_Logger)));

    _Logger->WriteLog("Creating Android Module Context...", LOG_SENDER, __func__);
    AndroidModuleContext& androidContext = *new AndroidModuleContext();
    androidContext.ModuleStatus = ModuleStatus::UNINITIALIZED;
    androidContext.EffectsEngine = new MenrvaEffectsEngine(_Logger, _ServiceLocator->GetFftEngine(), _ServiceLocator);
    androidContext.CommandProcessor = commandProcessor;  // IDE thinks these types are incompatible, but compiler & runtime execute successfully  ¯\_(ツ)_/¯

    _Logger->WriteLog("Creating Android Module Interface...", LOG_SENDER, __func__);
    AndroidModuleInterface* moduleInterface = new AndroidModuleInterface();
    moduleInterface->AndroidContext = &androidContext;
    moduleInterface->effectInterface = &EngineInterface;

    *pHandle = (effect_handle_t)moduleInterface;
    _Logger->WriteLog("Successfully Created Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

int AndroidInterface::ReleaseModule(effect_handle_t moduleHandle) {
    _Logger->WriteLog("Releasing Menrva Module...", LOG_SENDER, __func__);
    auto module = (AndroidModuleContext*)moduleHandle;

    if (module == nullptr) {
        _Logger->WriteLog("Invalid Module Provided.  Provided Module is not a Menrva Module.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Deleting Effects Engine & Module Pointers...", LOG_SENDER, __func__);
    module->ModuleStatus = ModuleStatus::RELEASING;
    delete module->EffectsEngine;
    delete module->InputBuffer;
    delete module->OutputBuffer;
    delete module;

    _Logger->WriteLog("Successfully Released Menrva Module!", LOG_SENDER, __func__);
    return 0;
}

int AndroidInterface::GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor) {
    _Logger->WriteLog("Getting Descriptor from UUID...", LOG_SENDER, __func__);
    if (pDescriptor == nullptr) {
        _Logger->WriteLog("Invalid Descriptor Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (uuid == nullptr) {
        _Logger->WriteLog("Invalid Effect UUID provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (memcmp(uuid, &AndroidInterface::EffectDescriptor.uuid, sizeof(*uuid)) != 0) {
        _Logger->WriteLog(StringOperations::FormatString("Incorrect Effect UUID provided. Does not match Menrva UUID (%s).", AndroidInterface::EngineUUID),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -ENOENT;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *pDescriptor = AndroidInterface::EffectDescriptor;
    return 0;
}

int AndroidInterface::GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor) {
    _Logger->WriteLog("Getting Descriptor from Module Pointer...", LOG_SENDER, __func__);
    auto module = (ModuleContext*)self;
    if (module == nullptr || pDescriptor == nullptr) {
        _Logger->WriteLog("Invalid Module Pointer provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Returning Effect Descriptor pointer!", LOG_SENDER, __func__);
    *pDescriptor = AndroidInterface::EffectDescriptor;
    return 0;
}

int AndroidInterface::Process(effect_handle_t handle, audio_buffer_t* inBufferPtr, audio_buffer_t* outBufferPtr) {
    _Logger->WriteLog("Buffer Input Received...", LOG_SENDER, __func__);
    ModuleContext& context = *(ModuleContext*)handle;
    audio_buffer_t& inBuffer = *inBufferPtr;
    audio_buffer_t& outBuffer = *outBufferPtr;

    if (context.ModuleStatus == ModuleStatus::RELEASING) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is in Releasing Status.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -ENODATA;
    }
    if (context.ModuleStatus != ModuleStatus::READY) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is not in Ready Status.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }
    if (inBuffer.frameCount != outBuffer.frameCount) {
        _Logger->WriteLog(StringOperations::FormatString("Skipping Processing Buffer.  Input Frame Count (%u) does not match Output Frame Count (%u).", inBuffer.frameCount, outBuffer.frameCount),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    uint32_t channelLength = context.ChannelLength;
    _Logger->WriteLog(StringOperations::FormatString("Input Buffer Frame Length (%u) and Channel Length (%u).", inBuffer.frameCount, channelLength),
                      LOG_SENDER, __func__);
    _Logger->WriteLog(StringOperations::FormatString("Output Buffer Frame Length (%u and Channel Length (%u).", outBuffer.frameCount, channelLength),
                      LOG_SENDER, __func__);
    _Logger->WriteLog("Setting up AudioBuffer Data from Input & Output Buffers...", LOG_SENDER, __func__);
    AndroidModuleContext& androidContext = *(AndroidModuleContext*)&context;
    switch (androidContext.config.inputCfg.format) {
        case AUDIO_FORMAT_PCM_16_BIT:
            context.InputBuffer->SetData(inBuffer.s16, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.s16, channelLength, outBuffer.frameCount);
            break;

        case AUDIO_FORMAT_PCM_32_BIT:
            context.InputBuffer->SetData(inBuffer.s32, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.s32, channelLength, outBuffer.frameCount);
            break;

        case AUDIO_FORMAT_PCM_FLOAT:
            context.InputBuffer->SetData(inBuffer.f32, channelLength, inBuffer.frameCount);
            context.OutputBuffer->SetData(outBuffer.f32, channelLength, outBuffer.frameCount);
            break;

        default:
            _Logger->WriteLog(StringOperations::FormatString("Skipping Processing Buffer.  Invalid Audio Format Provided (%d).", androidContext.config.inputCfg.format),
                              LOG_SENDER, __func__, LogLevel::ERROR);
            return -EINVAL;
    }

    _Logger->WriteLog("Passing AudioBuffers to EffectsEngine for Processing...", LOG_SENDER, __func__);
    int result = context.EffectsEngine->Process(*context.InputBuffer, *context.OutputBuffer);

    _Logger->WriteLog(StringOperations::FormatString("EffectsEngine finished Processing with Result (%d)!", result),
                      LOG_SENDER, __func__);
    return result;
}

int AndroidInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Command Input Received...", LOG_SENDER, __func__);
    auto contextPtr = (AndroidModuleInterface*)self;
    AndroidModuleContext& androidContext = *contextPtr->AndroidContext;

    _Logger->WriteLog("Passing Command Data to CommandProcessor for Processing...", LOG_SENDER, __func__);
    int result = androidContext.CommandProcessor->Process(androidContext, cmdCode, cmdSize, pCmdData, replySize, pReplyData);

    _Logger->WriteLog(StringOperations::FormatString("Finished Processing Command with Result (%d).", result),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    return result;
}
