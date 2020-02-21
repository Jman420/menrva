/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
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

#include "Module_ResetBuffers_Handler.h"

Module_ResetBuffers_Handler::Module_ResetBuffers_Handler(LoggerBase* logger)
        : TypedCommandHandlerBase(new Android_SystemCommand_Command(), logger, __PRETTY_FUNCTION__) {}

void Module_ResetBuffers_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Received ResetBuffers Command...", LOG_SENDER, __func__);
    if (context.Engine == nullptr) {
        _Logger->WriteLog("Skipping ResetBuffers Command.  Invalid Engine Instance provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return;
    }

    _Logger->WriteLog("Resetting Effects Engine Buffers...", LOG_SENDER, __func__);
    context.Engine->ResetBuffers(context.config.inputCfg.samplingRate);

    _Logger->WriteLog("Successfully Reset Effects Engine Buffers!", LOG_SENDER, __func__);
}

uint32_t Module_ResetBuffers_Handler::SerializeResponse(void* responseBuffer) {
    return 0;
}
