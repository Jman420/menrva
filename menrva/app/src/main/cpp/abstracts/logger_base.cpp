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
#include "logger_base.h"
#include "../aosp/liblog/android/log.h"

const std::string LoggerBase::APP_NAME = "Menrva";
const std::string LoggerBase::OVERRIDE_LIST_KEY_DELIMITER = ".";
const LogLevel LoggerBase::DEFAULT_LOG_LEVEL = LogLevel::DEBUG;
const LogLevel LoggerBase::DEFAULT_APP_LOG_LEVEL = LogLevel::ERROR;

LogLevel LoggerBase::_AppLogLevel = DEFAULT_APP_LOG_LEVEL;

bool LoggerBase::_OverrideListEnabled = false;
logger_override_list LoggerBase::_OverrideList = *new logger_override_list();

void LoggerBase::SetAppLogLevel(LogLevel logLevel) {
    _AppLogLevel = logLevel;
}

LogLevel LoggerBase::GetAppLogLevel() {
    return _AppLogLevel;
}

void LoggerBase::WriteLog(std::string message, std::string senderClass, std::string senderFunction,
                          LogLevel logLevel, ...) {
    va_list args;
    va_start(args, logLevel);
    WriteLog(std::move(message), std::move(senderClass), std::move(senderFunction), logLevel, args);
}

void LoggerBase::WriteLog(std::string message, std::string senderClass, LogLevel logLevel, ...) {
    va_list args;
    va_start(args, logLevel);
    WriteLog(std::move(message), std::move(senderClass), "", logLevel, args);
}

void LoggerBase::WriteLog(std::string message, LogLevel logLevel, ...) {
    va_list args;
    va_start(args, logLevel);
    WriteLog(std::move(message), "", "", logLevel, args);
}

void LoggerBase::WriteLog(std::string message, std::string senderClass, std::string senderFunction, ...) {
    va_list args;
    va_start(args, senderFunction);
    WriteLog(std::move(message), std::move(senderClass), std::move(senderFunction), DEFAULT_LOG_LEVEL, args);
}

void LoggerBase::WriteLog(std::string message, std::string senderClass, ...) {
    va_list args;
    va_start(args, senderClass);
    WriteLog(std::move(message), std::move(senderClass), "", DEFAULT_LOG_LEVEL, args);
}

void LoggerBase::WriteLog(std::string message, ...) {
    va_list args;
    va_start(args, message);
    WriteLog(std::move(message), "", "", DEFAULT_LOG_LEVEL, args);
}

void LoggerBase::SetOverrideListEnabled(bool enabled) {
    _OverrideListEnabled = enabled;
}

bool LoggerBase::GetOverrideListEnabled() {
    return _OverrideListEnabled;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, bool enabled) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.ComponentLogLevel = logLevel;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, std::string functionName, bool enabled) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.ComponentLogLevel = logLevel;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel) {
    logger_override_entry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LoggerBase::RemoveOverrideListEntry(std::string className) {
    _OverrideList.erase(className);
}

void LoggerBase::RemoveOverrideListEntry(std::string className, std::string functionName) {
    _OverrideList.erase(GetOverrideListKey(std::move(className), std::move(functionName)));
}

bool LoggerBase::CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel) {
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

logger_override_entry* LoggerBase::GetAddOverrideListElement(std::string className) {
    const std::string &key = className;
    auto element = _OverrideList.find(key);
    if (element == _OverrideList.end()) {
        auto newEntry = new logger_override_entry();
        newEntry->ClassName = className;
        element = _OverrideList.insert(logger_override_list_element(key, newEntry)).first;
    }

    return element->second;
}

logger_override_entry* LoggerBase::GetAddOverrideListElement(std::string className, std::string functionName) {
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

std::string LoggerBase::GetOverrideListKey(std::string className, std::string functionName) {
    return className + OVERRIDE_LIST_KEY_DELIMITER + functionName;
}
