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
#include "LogWriterBase.h"

const log_level_map LogWriterBase::LOG_LEVEL_MAP = {
        { "Fatal", LogLevel::FATAL },
        { "Error", LogLevel::ERROR },
        { "Warn", LogLevel::WARN },
        { "Info", LogLevel::INFO },
        { "Debug", LogLevel::DEBUG },
        { "Verbose", LogLevel::VERBOSE },
};

const std::string LogWriterBase::APP_NAME = "Menrva";
const std::string LogWriterBase::OVERRIDE_LIST_KEY_DELIMITER = ".";
const LogLevel LogWriterBase::DEFAULT_LOG_LEVEL = LogLevel::DEBUG;
const LogLevel LogWriterBase::START_UP_LOG_LEVEL = LogLevel::ERROR;
const uint8_t LogWriterBase::MIN_LOG_LEVEL_VALUE = LogLevel::VERBOSE;
const uint8_t LogWriterBase::MAX_LOG_LEVEL_VALUE = LogLevel::FATAL;

LogLevel LogWriterBase::_LogLevel = START_UP_LOG_LEVEL;

bool LogWriterBase::_OverrideListEnabled = false;
logger_override_list LogWriterBase::_OverrideList = *new logger_override_list();

void LogWriterBase::SetLogLevel(LogLevel logLevel) {
    _LogLevel = logLevel;
}

LogLevel LogWriterBase::GetLogLevel() {
    return _LogLevel;
}

void LogWriterBase::WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) {
    if ((GetOverrideListEnabled() && CheckOverrideList(senderClass, senderFunction, logLevel)) || logLevel >= GetLogLevel()) {
        WriteLogLine(std::move(message), std::move(senderClass), std::move(senderFunction), logLevel);
    }
}

void LogWriterBase::WriteLog(std::string message, std::string senderClass, LogLevel logLevel) {
    WriteLog(std::move(message), std::move(senderClass), "", logLevel);
}

void LogWriterBase::WriteLog(std::string message, LogLevel logLevel) {
    WriteLog(std::move(message), "", "", logLevel);
}

void LogWriterBase::WriteLog(std::string message, std::string senderClass, std::string senderFunction) {
    WriteLog(std::move(message), std::move(senderClass), std::move(senderFunction), DEFAULT_LOG_LEVEL);
}

void LogWriterBase::WriteLog(std::string message, std::string senderClass) {
    WriteLog(std::move(message), std::move(senderClass), "", DEFAULT_LOG_LEVEL);
}

void LogWriterBase::WriteLog(std::string message) {
    WriteLog(std::move(message), "", "", DEFAULT_LOG_LEVEL);
}

void LogWriterBase::SetOverrideListEnabled(bool enabled) {
    _OverrideListEnabled = enabled;
}

bool LogWriterBase::GetOverrideListEnabled() {
    return _OverrideListEnabled;
}

void LogWriterBase::SetOverrideListEntry(std::string className, bool enabled) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
}

void LogWriterBase::SetOverrideListEntry(std::string className, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.ComponentLogLevel = logLevel;
}

void LogWriterBase::SetOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LogWriterBase::SetOverrideListEntry(std::string className, std::string functionName, bool enabled) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
}

void LogWriterBase::SetOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.ComponentLogLevel = logLevel;
}

void LogWriterBase::SetOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LogWriterBase::RemoveOverrideListEntry(std::string className) {
    _OverrideList.erase(className);
}

void LogWriterBase::RemoveOverrideListEntry(std::string className, std::string functionName) {
    _OverrideList.erase(GetOverrideListKey(std::move(className), std::move(functionName)));
}

bool LogWriterBase::CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel) {
    // Check for Function Override Entry
    auto element = _OverrideList.find(GetOverrideListKey(className, functionName));
    if (element == _OverrideList.end()) {
        // Check for Class Override Entry
        element = _OverrideList.find(className);
        if (element == _OverrideList.end()) {
            // No Entries found
            return false;
        }
    }

    auto entry = *element->second;
    return entry.Enabled && logLevel >= entry.ComponentLogLevel;
}

logger_override_entry* LogWriterBase::GetAddOverrideListElement(std::string className) {
    const std::string &key = className;
    auto element = _OverrideList.find(key);
    if (element == _OverrideList.end()) {
        auto newEntry = new logger_override_entry();
        newEntry->ClassName = className;
        element = _OverrideList.insert(logger_override_list_element(key, newEntry)).first;
    }

    return element->second;
}

logger_override_entry* LogWriterBase::GetAddOverrideListElement(std::string className, std::string functionName) {
    std::string key = GetOverrideListKey(className, functionName);
    auto element = _OverrideList.find(key);
    if (element == _OverrideList.end()) {
        auto newEntry = new logger_override_entry();
        newEntry->ClassName = className;
        newEntry->FunctionName = functionName;
        element = _OverrideList.insert(logger_override_list_element(key, newEntry)).first;
    }

    return element->second;
}

std::string LogWriterBase::GetOverrideListKey(std::string className, std::string functionName) {
    return className + OVERRIDE_LIST_KEY_DELIMITER + functionName;
}
