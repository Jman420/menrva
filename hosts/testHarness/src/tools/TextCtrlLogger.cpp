/* Menrva - Audio Effect Engine supporting Plug'n'Play style DSP Effects
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

#include <time.h>
#include <menrvaEngine/tools/StringOperations.h>
#include "TextCtrlLogger.h"

const std::string TextCtrlLogger::TIMESTAMP_FORMAT = "%m/%d/%Y %T";  // mm/dd/yyyy HH:MM:SS
const std::string TextCtrlLogger::PREFIX_FORMAT = "[%s] - %s - ";  // [%timestamp%] - %logLevel% 
const std::string TextCtrlLogger::SENDER_FUNC_FORMAT = ".%s()";  // .%senderFunction%()
const std::string TextCtrlLogger::NEW_LINE = "\n";

bool TextCtrlLogger::_Initialized = false;
wxTextCtrl* TextCtrlLogger::_Console = nullptr;

TextCtrlLogger::TextCtrlLogger()
{
    Initialize();
}

wxTextCtrl* TextCtrlLogger::GetTextCtrl()
{
    return _Console;
}

void TextCtrlLogger::WriteLogLine(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel)
{
    std::string timestamp = GetTimestamp();
    std::string logLevelStr = LogLevels::ConvertToString(logLevel);
    std::string prefix = StringOperations::FormatString(PREFIX_FORMAT, timestamp.c_str(), logLevelStr.c_str());

    std::string sender = senderClass;
    if (!senderFunction.empty()) {
        sender += StringOperations::FormatString(SENDER_FUNC_FORMAT, senderFunction.c_str());
    }
    if (!sender.empty()) {
        sender += " - ";
    }

    std::string logLine = prefix;
    logLine += sender;
    logLine += message;
    logLine += NEW_LINE;

    _Console->AppendText(logLine);
}

std::string TextCtrlLogger::GetTimestamp()
{
    const size_t TIMESTAMP_BUFFER_SIZE = 19;
    
    const time_t timer = time(NULL);
    tm* timeInfo = localtime(&timer);

    char timestamp[TIMESTAMP_BUFFER_SIZE];
    strftime(timestamp, TIMESTAMP_BUFFER_SIZE, TIMESTAMP_FORMAT.c_str(), timeInfo);

    return timestamp;
}

void TextCtrlLogger::Initialize()
{
    if (_Initialized) {
        return;
    }

    _Console = new wxTextCtrl();
}
