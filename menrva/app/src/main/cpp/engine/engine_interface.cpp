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
#include "engine_interface.h"
#include "command_map.h"
#include "../module_interface.h"
#include "../audio/sample.h"
#include "../audio/audio_format.h"

const std::string MenrvaEngineInterface::LOG_SENDER = "EngineInterface";

ServiceLocator* MenrvaEngineInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaEngineInterface::_Logger = _ServiceLocator->GetLogger();

int MenrvaEngineInterface::Process(effect_handle_t handle, audio_buffer_t* in, audio_buffer_t* out) {
    _Logger->WriteLog("Buffer Input Received...", LOG_SENDER, __func__);
    auto* context = (menrva_module_context*)handle;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is in Releasing Status.", LOG_SENDER, __func__, LogLevel::WARN);
        return -ENODATA;
    }
    if (context->ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY) {
        _Logger->WriteLog("Skipping Processing Buffer.  Module is not in Ready Status.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }

    _Logger->WriteLog("Input Buffer Frame Count : %d", LOG_SENDER, __func__, in->frameCount);
    _Logger->WriteLog("Output Buffer Frame Count : %d", LOG_SENDER, __func__, out->frameCount);
    _Logger->WriteLog("Setting up AudioBuffer Data from Input & Output Buffers...", LOG_SENDER, __func__);
    switch (context->config->inputCfg.format) {
        case AUDIO_FORMAT_PCM_16_BIT:
            context->InputBuffer->SetData(in->s16, in->frameCount);
            context->OutputBuffer->SetData(out->s16, out->frameCount);
            break;

        case AUDIO_FORMAT_PCM_32_BIT:
            context->InputBuffer->SetData(in->s32, in->frameCount);
            context->OutputBuffer->SetData(out->s32, out->frameCount);
            break;

        case AUDIO_FORMAT_PCM_FLOAT:
            context->InputBuffer->SetData(in->f32, in->frameCount);
            context->OutputBuffer->SetData(out->f32, out->frameCount);
            break;

        default:
            _Logger->WriteLog("Skipping Processing Buffer.  Invalid Audio Format Provided : %d", LOG_SENDER, __func__, LogLevel::WARN, context->config->inputCfg.format);
            return -EINVAL;
    }

    _Logger->WriteLog("Passing AudioBuffers to EffectsEngine for Processing...", LOG_SENDER, __func__);
    int result = context->EffectsEngine->Process(context->InputBuffer, context->OutputBuffer);

    _Logger->WriteLog("EffectsEngine finished Processing with Result : %d !", LOG_SENDER, __func__, result);
    return result;
}

int MenrvaEngineInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize,
                                   void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Command Input Received...", LOG_SENDER, __func__);
    auto* context = (menrva_module_context*)self;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING ||
        context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {

        _Logger->WriteLog("Skipping Processing Command.  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::WARN);
        return -EINVAL;
    }

    _Logger->WriteLog("Passing Command Data to CommandMap for Processing...", LOG_SENDER, __func__);
    int result = MenrvaCommandMap::Process(context, cmdCode, cmdSize, pCmdData, replySize,
                                           pReplyData);

    _Logger->WriteLog("CommandMap finished Processing with Result : %d", LOG_SENDER, __func__, LogLevel::VERBOSE, result);
    return result;
}

MenrvaEngineInterface::MenrvaEngineInterface() = default;
