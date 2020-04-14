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

#include <utility>
#include "ILogWriter.h"

const LogLevel ILogWriter::DEFAULT_LOG_LEVEL = LogLevel::Debug;
const LogLevel ILogWriter::START_UP_LOG_LEVEL = LogLevel::Error;
const uint8_t ILogWriter::MIN_LOG_LEVEL_VALUE = static_cast<uint8_t>(LogLevel::Verbose);
const uint8_t ILogWriter::MAX_LOG_LEVEL_VALUE = static_cast<uint8_t>(LogLevel::Fatal);

LogLevel ILogWriter::_LogLevelLimit = START_UP_LOG_LEVEL;

ILogWriter::ILogWriter(ILogOverrideManager& logOverrideManager)
{
    _LogOverrideManager = logOverrideManager;
}

void ILogWriter::SetLogLevelLimit(LogLevel logLevel) {
    _LogLevelLimit = logLevel;
}

LogLevel ILogWriter::GetLogLevelLimit() {
    return _LogLevelLimit;
}

ILogOverrideManager& ILogWriter::GetLogOverrideManager()
{
    return _LogOverrideManager;
}

void ILogWriter::WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) {
    if ((_LogOverrideManager.GetOverrideListEnabled() && _LogOverrideManager.CheckOverrideList(senderClass, senderFunction, logLevel)) || logLevel >= GetLogLevelLimit()) {
        WriteLogLine(std::move(message), std::move(senderClass), std::move(senderFunction), logLevel);
    }
}

void ILogWriter::WriteLog(std::string message, std::string senderClass, LogLevel logLevel) {
    WriteLog(std::move(message), std::move(senderClass), "", logLevel);
}

void ILogWriter::WriteLog(std::string message, LogLevel logLevel) {
    WriteLog(std::move(message), "", "", logLevel);
}

void ILogWriter::WriteLog(std::string message, std::string senderClass, std::string senderFunction) {
    WriteLog(std::move(message), std::move(senderClass), std::move(senderFunction), DEFAULT_LOG_LEVEL);
}

void ILogWriter::WriteLog(std::string message, std::string senderClass) {
    WriteLog(std::move(message), std::move(senderClass), "", DEFAULT_LOG_LEVEL);
}

void ILogWriter::WriteLog(std::string message) {
    WriteLog(std::move(message), "", "", DEFAULT_LOG_LEVEL);
}
