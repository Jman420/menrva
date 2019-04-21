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
#include "EngineInterface.h"
#include "CommandMap.h"
#include "../audio/AudioFormat.h"

const std::string MenrvaEngineInterface::LOG_SENDER = "EngineInterface";

ServiceLocator* MenrvaEngineInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaEngineInterface::_Logger = _ServiceLocator->GetLogger();

int MenrvaEngineInterface::Process(effect_handle_t handle, audio_buffer_t* inBufferPtr, audio_buffer_t* outBufferPtr) {
    _Logger->WriteLog("Buffer Input Received...", LOG_SENDER, __func__);
    MenrvaModuleContext& context = *(MenrvaModuleContext*)handle;
    audio_buffer_t& inBuffer = *inBufferPtr;
    audio_buffer_t& outBuffer = *outBufferPtr;

    if (context.ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is in Releasing Status.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -ENODATA;
    }
    if (context.ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is not in Ready Status.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }
    if (inBuffer.frameCount != outBuffer.frameCount) {
        _Logger->WriteLog("Skipping Processing Buffer.  Input Frame Count (%u) does not match Output Frame Count (%u).", LOG_SENDER, __func__, LogLevel::ERROR, inBuffer.frameCount, outBuffer.frameCount);
        return -EINVAL;
    }

    uint32_t channelLength = context.ChannelLength;
    _Logger->WriteLog("Input Buffer Frame Length (%u) and Channel Length (%u).", LOG_SENDER, __func__, inBuffer.frameCount, channelLength);
    _Logger->WriteLog("Output Buffer Frame Length (%u and Channel Length (%u).", LOG_SENDER, __func__, outBuffer.frameCount, channelLength);
    _Logger->WriteLog("Setting up AudioBuffer Data from Input & Output Buffers...", LOG_SENDER, __func__);
    switch (context.config.inputCfg.format) {
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
            _Logger->WriteLog("Skipping Processing Buffer.  Invalid Audio Format Provided (%d).", LOG_SENDER, __func__, LogLevel::ERROR, context.config.inputCfg.format);
            return -EINVAL;
    }

    _Logger->WriteLog("Passing AudioBuffers to EffectsEngine for Processing...", LOG_SENDER, __func__);
    int result = context.EffectsEngine->Process(*context.InputBuffer, *context.OutputBuffer);

    _Logger->WriteLog("EffectsEngine finished Processing with Result (%d)!", LOG_SENDER, __func__, result);
    return result;
}

int MenrvaEngineInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Command Input Received...", LOG_SENDER, __func__);
    auto contextPtr = (MenrvaModuleContext*)self;
    MenrvaModuleContext& context = *contextPtr;

    if (context.ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING ||
        context.ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {

        _Logger->WriteLog("Skipping Processing Command.  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Passing Command Data to CommandMap for Processing...", LOG_SENDER, __func__);
    int result = MenrvaCommandMap::Process(context, cmdCode, cmdSize, pCmdData, replySize, pReplyData);

    _Logger->WriteLog("CommandMap finished Processing with Result (%d).", LOG_SENDER, __func__, LogLevel::VERBOSE, result);
    return result;
}
