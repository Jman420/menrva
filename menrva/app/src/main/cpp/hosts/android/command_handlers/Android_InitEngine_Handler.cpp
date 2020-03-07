/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2020 Justin Giannone (aka Jman420)
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

#include "Android_InitEngine_Handler.h"
#include "../commands/AndroidHost_Generic_Command.h"

Android_InitEngine_Handler::Android_InitEngine_Handler(LogWriterBase* logger)
        : CommandHandlerBase(new AndroidHost_Generic_Command(), logger, __PRETTY_FUNCTION__) {}

void Android_InitEngine_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Initializing Module Context...", LOG_SENDER, __func__);

    if (context.ModuleStatus > ModuleStatus::INITIALIZING) {
        _Logger->WriteLog("Module Context already Initialized!", LOG_SENDER, __func__);
        return;
    }

    context.ModuleStatus = ModuleStatus::INITIALIZING;
    // TODO : Configure any necessary default parameters
    //_Logger->WriteLog("Setting up Menrva Effects Engine Parameters...", logPrefix);

    context.ModuleStatus = ModuleStatus::READY;
    _Logger->WriteLog("Successfully Initialized Menrva Context!", LOG_SENDER, __func__);
}

uint32_t Android_InitEngine_Handler::SerializeResponse(void* responseBuffer) {
    *(int*)responseBuffer = 0;
    return sizeof(int);
}
