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

#include "Engine_SetLogLevelLimit_Handler.h"
#include "../tools/StringOperations.h"

Engine_SetLogLevelLimit_Handler::Engine_SetLogLevelLimit_Handler(ILogWriter* logger)
        : TypedCommandHandlerBase(new Engine_SetLogLevelLimit_Command(), logger, __PRETTY_FUNCTION__) {}

void Engine_SetLogLevelLimit_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Received SetLogLevelLimit Command...", LOG_SENDER, __func__);
    messages::Engine_SetLogLevelLimit_Request& request = *_TypedCommand->GetTypedRequest();
    messages::Engine_SetLogLevelLimit_Response& response = *_TypedCommand->GetTypedResponse();

    _Logger->WriteLog("Validating SetLogLevelLimit Request...", LOG_SENDER, __func__);
    int requestLogLevel = request.loglevel();
    if (requestLogLevel > ILogWriter::MAX_LOG_LEVEL_VALUE || requestLogLevel < ILogWriter::MIN_LOG_LEVEL_VALUE) {
        _Logger->WriteLog(StringOperations::FormatString("SetLogLevelLimit Request Validation Failed : Invalid Log Level Provided (%u)!", requestLogLevel),
                          LOG_SENDER, __func__, LogLevel::Error);
        response.set_loglevel(static_cast<uint8_t>(_Logger->GetLogLevelLimit()));
        response.set_success(false);
        return;
    }

    _Logger->WriteLog(StringOperations::FormatString("Setting New Log Level (%u)...", requestLogLevel),
                      LOG_SENDER, __func__);
    auto newLogLevel = static_cast<LogLevel>(requestLogLevel);
    _Logger->SetLogLevelLimit(newLogLevel);

    _Logger->WriteLog(StringOperations::FormatString("Successfully set New Log Level (%u).  Setting Response...", requestLogLevel),
                      LOG_SENDER, __func__);
    response.set_loglevel(static_cast<uint8_t>(_Logger->GetLogLevelLimit()));
    response.set_success(true);
    _Logger->WriteLog("Successfully set SetLogLevelLimit Success Response.", LOG_SENDER, __func__);
}
