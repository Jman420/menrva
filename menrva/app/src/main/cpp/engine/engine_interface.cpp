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

const std::string MenrvaEngineInterface::LOG_TAG = "Menrva-EngineInterface - ";

ServiceLocator* MenrvaEngineInterface::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaEngineInterface::_Logger = _ServiceLocator->GetLogger();

int MenrvaEngineInterface::Process(effect_handle_t handle, audio_buffer_t* in, audio_buffer_t* out) {
    struct menrva_module_context *context = (menrva_module_context*)handle;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING) {
        return -ENODATA;
    }
    if (context->ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY) {
        return 0;
    }

    context->InputBuffer->SetData((sample*)in->f32, in->frameCount, false);
    context->OutputBuffer->SetData((sample*)out->f32, out->frameCount, false);
    int result = context->EffectsEngine->Process(context->InputBuffer, context->OutputBuffer);

    return result;
}

int MenrvaEngineInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize,
                                   void* pCmdData, uint32_t* replySize, void* pReplyData) {
    // BEGIN DEBUG
    _Logger->WriteLog("Engine Interface Command() called with %d", LOG_TAG, LogLevel::VERBOSE, cmdCode);
    _Logger->WriteLog("Command Passed is INIT : %d", LOG_TAG, LogLevel::VERBOSE, (cmdCode == EFFECT_CMD_INIT));
    _Logger->WriteLog("Command Passed is SET_CONFIG : %d", LOG_TAG, LogLevel::VERBOSE, (cmdCode == EFFECT_CMD_SET_CONFIG));
    // END DEBUG

    struct menrva_module_context *context = (menrva_module_context*)self;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING ||
        context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {

        return -EINVAL;
    }

    int result = MenrvaCommandMap::Command(context, cmdCode, cmdSize, pCmdData, replySize,
                                           pReplyData);

    // BEGIN DEBUG
    _Logger->WriteLog("Command Result : %d", LOG_TAG, LogLevel::VERBOSE, result);
    // END DEBUG

    return result;
}

MenrvaEngineInterface::MenrvaEngineInterface() {}
