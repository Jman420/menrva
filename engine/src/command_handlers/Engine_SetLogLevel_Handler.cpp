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

#include "Engine_SetLogLevel_Handler.h"
#include "../tools/StringOperations.h"

Engine_SetLogLevel_Handler::Engine_SetLogLevel_Handler(LogWriterBase* logger)
        : TypedCommandHandlerBase(new Engine_SetLogLevel_Command(), logger, __PRETTY_FUNCTION__) {}

void Engine_SetLogLevel_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Received SetLogLevel Command...", LOG_SENDER, __func__);
    messages::Engine_SetLogLevel_Request& request = *_TypedCommand->GetTypedRequest();
    messages::Engine_SetLogLevel_Response& response = *_TypedCommand->GetTypedResponse();

    _Logger->WriteLog("Validating SetLogLevel Request...", LOG_SENDER, __func__);
    int requestLogLevel = request.loglevel();
    if (requestLogLevel > LogWriterBase::MAX_LOG_LEVEL_VALUE || requestLogLevel < LogWriterBase::MIN_LOG_LEVEL_VALUE) {
        _Logger->WriteLog(StringOperations::FormatString("SetLogLevel Request Validation Failed : Invalid Log Level Provided (%u)!", requestLogLevel),
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
    _Logger->WriteLog("Successfully set SetLogLevel Success Response.", LOG_SENDER, __func__);
}
