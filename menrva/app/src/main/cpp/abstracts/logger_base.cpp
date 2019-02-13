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

const std::string LoggerBase::APP_NAME = "Menrva";
const LogLevel LoggerBase::DEFAULT_LOG_LEVEL = LogLevel::DEBUG;
const LogLevel LoggerBase::DEFAULT_APP_LOG_LEVEL = LogLevel::ERROR;

LogLevel LoggerBase::AppLogLevel = DEFAULT_APP_LOG_LEVEL;

bool LoggerBase::_WhitelistEnabled = false;
logger_whitelist LoggerBase::_Whitelist = *new logger_whitelist();

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
    _WhitelistEnabled = enabled;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, bool enabled) {
    logger_whitelist_entry entry = GetAddWhitelistElement(std::move(className));
    entry.Enabled = enabled;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, LogLevel logLevel) {
    logger_whitelist_entry entry = GetAddWhitelistElement(std::move(className));
    entry.ComponentLogLevel = logLevel;
}

void LoggerBase::UpsertOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) {
    logger_whitelist_entry entry = GetAddWhitelistElement(std::move(className));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

logger_whitelist_entry LoggerBase::GetAddWhitelistElement(std::string className) {
    auto element = _Whitelist.find(className);
    if (element == _Whitelist.end()) {
        logger_whitelist_entry newEntry = *new logger_whitelist_entry();
        newEntry.ClassName = className;
        element = _Whitelist.insert(logger_whitelist_element(className, newEntry)).first;
    }

    return element->second;
}

void LoggerBase::RemoveOverrideListEntry(std::string className) {
    _Whitelist.erase(className);
}

bool LoggerBase::CheckOverrideList(std::string className, LogLevel logLevel) {
    auto element = _Whitelist.find(className);
    if (element == _Whitelist.end()) {
        return false;
    }

    auto entry = element->second;
    return entry.Enabled && logLevel >= entry.ComponentLogLevel;
}
