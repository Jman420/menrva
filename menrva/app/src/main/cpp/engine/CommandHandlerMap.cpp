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

#include "CommandHandlerMap.h"
#include "../commands/MenrvaCommands.h"

bool CommandHandlerMap::_Initialized = false;
handler_map* CommandHandlerMap::_HandlerMap = new handler_map();

CommandHandlerMap::CommandHandlerMap(LoggerBase* logger) {
    if (!CommandHandlerMap::_Initialized) {
        CommandHandlerMap::Initialize(logger);
    }
}

handler_map* CommandHandlerMap::GetHandlerMap() {
    return CommandHandlerMap::_HandlerMap;
}

CommandHandlerBase* CommandHandlerMap::GetCommandHandler(uint32_t commandId) {
    handler_map& handlerMap = *CommandHandlerMap::_HandlerMap;

    auto cmdFunction = handlerMap.find(commandId);
    if (cmdFunction == handlerMap.end()) {
        return nullptr;
    }

    return cmdFunction->second;
}

#include "../tools/CommandIds.h"
#include "../command_handlers/Module_Init_Handler.h"
#include "../command_handlers/Module_GetConfig_Handler.h"
#include "../command_handlers/Module_SetConfig_Handler.h"
#include "../command_handlers/Module_ResetBuffers_Handler.h"
#include "../command_handlers/Module_EnableEngine_Handler.h"
#include "../command_handlers/Module_DisableEngine_Handler.h"

#include "../command_handlers/Engine_GetVersion_Handler.h"

void CommandHandlerMap::Initialize(LoggerBase* logger) {
    handler_map& handlerMap = *CommandHandlerMap::_HandlerMap;

    handlerMap.insert(handler_map::value_type(EFFECT_CMD_INIT, new Module_Init_Handler(logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_GET_CONFIG, new Module_GetConfig_Handler(logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_SET_CONFIG, new Module_SetConfig_Handler(logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_RESET, new Module_ResetBuffers_Handler(logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_ENABLE, new Module_EnableEngine_Handler(logger)));
    handlerMap.insert(handler_map::value_type(EFFECT_CMD_DISABLE, new Module_DisableEngine_Handler(logger)));

    handlerMap.insert(handler_map::value_type(CommandIds::Calculate(MenrvaCommands::Engine_GetVersion), new Engine_GetVersion_Handler(logger)));

    CommandHandlerMap::_Initialized = true;
}
