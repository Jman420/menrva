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
#include "../tools/StringOperations.h"

const std::string CommandProcessor::LOG_SENDER = "CommandProcessor";

CommandProcessor::CommandProcessor(LogWriterBase *logger) {
    _Logger = logger;
    _CommandHandlerMap = new CommandHandlerMap(_Logger);
}

int CommandProcessor::Process(ModuleContext& context, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog(StringOperations::FormatString("Processing Command Id (%u)...", cmdCode),
                      LOG_SENDER, __func__);
    if (context.ModuleStatus == ModuleStatus::RELEASING || context.ModuleStatus == ModuleStatus::INITIALIZING) {
        _Logger->WriteLog(StringOperations::FormatString("Skipping Processing Command Id (%u).  Module Status is invalid.", cmdCode),
                          LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog(StringOperations::FormatString("Looking up Handler for Command Id (%u)...", cmdCode),
                      LOG_SENDER, __func__);
    CommandHandlerBase* handlerPtr = _CommandHandlerMap->GetCommandHandler(cmdCode);
    if (handlerPtr == nullptr) {
        _Logger->WriteLog(StringOperations::FormatString("Unable to find Handler for Command Id (%u).  Skipping processing command.", cmdCode),
                          LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }

    _Logger->WriteLog(StringOperations::FormatString("Function found for Command Id (%u).  Initializing Handler...", cmdCode),
                      LOG_SENDER, __func__);
    CommandHandlerBase& handler = *handlerPtr;
    handler.DeserializeRequest(pCmdData, cmdSize);

    _Logger->WriteLog(StringOperations::FormatString("Executing Command Handler for Command Id (%u)...", cmdCode),
                      LOG_SENDER, __func__);
    handler.Execute(context);
    int32_t returnValue = handler.GetReturnValue();

    _Logger->WriteLog(StringOperations::FormatString("Successfully Processed Command Id (%u) with result (%u).  Serializing Response...", cmdCode, returnValue),
                      LOG_SENDER, __func__);
    *replySize = handler.SerializeResponse(pReplyData);

    _Logger->WriteLog(StringOperations::FormatString("Successfully Serialized Response for Command Id (%u).", cmdCode),
                      LOG_SENDER, __func__);
    return returnValue;
}

CommandHandlerMap* CommandProcessor::GetCommandHandlerMap() {
    return _CommandHandlerMap;
}
