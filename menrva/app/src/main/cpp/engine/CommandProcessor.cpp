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
#include "CommandProcessor.h"
#include "../tools/CommandIds.h"
#include "../commands/MenrvaCommands.h"
#include "../commands/messages/Engine_GetVersion.pb.h"
#include "../commands/Engine_GetVersion_Command.h"
#include "../command_handlers/Engine_GetVersion_Handler.h"

const std::string CommandProcessor::LOG_SENDER = "CommandProcessor";
ServiceLocator* CommandProcessor::_ServiceLocator = new ServiceLocator();
LoggerBase* CommandProcessor::_Logger = _ServiceLocator->GetLogger();
CommandHandlerMap* CommandProcessor::_HandlerMap = new CommandHandlerMap(_Logger);

int CommandProcessor::Process(MenrvaModuleContext& context, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Processing Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    if (context.ModuleStatus != ModuleStatus::READY){
        _Logger->WriteLog("Skipping Processing Command Id (%u).  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::ERROR, cmdCode);
        return -EINVAL;
    }

    _Logger->WriteLog("Looking up Handler for Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    CommandHandlerBase* handlerPtr = CommandProcessor::_HandlerMap->GetCommandHandler(cmdCode);
    if (handlerPtr == nullptr) {
        _Logger->WriteLog("Unable to find Handler for Command Id (%u).  Skipping processing command.", LOG_SENDER, __func__, LogLevel::WARN, cmdCode);
        return 0;
    }

    _Logger->WriteLog("Function found for Command Id (%u).  Initializing Handler...", LOG_SENDER, __func__, cmdCode);
    CommandHandlerBase& handler = *handlerPtr;
    handler.DeserializeRequest(pCmdData, cmdSize);

    _Logger->WriteLog("Executing Command Handler for Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    handler.Execute(context);
    int32_t returnValue = handler.GetReturnValue();

    _Logger->WriteLog("Successfully Processed Command Id (%u) with result (%u).  Serializing Response...", LOG_SENDER, __func__, cmdCode, returnValue);
    *replySize = handler.SerializeResponse(pReplyData);

    _Logger->WriteLog("Successfully Serialized Response for Command Id (%u).", LOG_SENDER, __func__, cmdCode);
    return returnValue;
}
