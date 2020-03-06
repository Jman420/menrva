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

#include "Android_GetConfig_Handler.h"
#include "../commands/AndroidHost_Generic_Command.h"
#include "../AndroidStructures.h"

Android_GetConfig_Handler::Android_GetConfig_Handler(LoggerBase* logger)
        : CommandHandlerBase(new AndroidHost_Generic_Command(), logger, __PRETTY_FUNCTION__) {
    _Config = nullptr;
}

void Android_GetConfig_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Storing Module Config...", LOG_SENDER, __func__);
    AndroidModuleContext& androidContext = *(AndroidModuleContext*)&context;
    _Config = &androidContext.config;
}

uint32_t Android_GetConfig_Handler::SerializeResponse(void* responseBuffer) {
    _Logger->WriteLog("Returning Module Config as Reply Data...", LOG_SENDER, __func__);
    *(effect_config_t*)responseBuffer = *_Config;
    _Config = nullptr;

    return sizeof(effect_config_t);
}
