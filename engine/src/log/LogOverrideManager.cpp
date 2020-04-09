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

#include "LogOverrideManager.h"

const std::string LogOverrideManager::OVERRIDE_LIST_KEY_DELIMITER = ".";

bool LogOverrideManager::_OverrideListEnabled = false;
LogOverrideMap LogOverrideManager::_OverrideList = *new LogOverrideMap();

void LogOverrideManager::SetOverrideListEnabled(bool enabled) {
    _OverrideListEnabled = enabled;
}

bool LogOverrideManager::GetOverrideListEnabled() {
    return _OverrideListEnabled;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, bool enabled) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, LogLevel logLevel) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className));
    entry.ComponentLogLevel = logLevel;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, std::string functionName, bool enabled) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.ComponentLogLevel = logLevel;
}

void LogOverrideManager::SetOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel) {
    LogOverrideEntry& entry = *GetAddOverrideListElement(std::move(className), std::move(functionName));
    entry.Enabled = enabled;
    entry.ComponentLogLevel = logLevel;
}

void LogOverrideManager::RemoveOverrideListEntry(std::string className) {
    _OverrideList.erase(className);
}

void LogOverrideManager::RemoveOverrideListEntry(std::string className, std::string functionName) {
    _OverrideList.erase(GetOverrideListKey(std::move(className), std::move(functionName)));
}

bool LogOverrideManager::CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel) {
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

std::vector<LogOverrideEntry>* LogOverrideManager::GetOverrideList()
{
    std::vector<LogOverrideEntry>& overrideList = *new std::vector<LogOverrideEntry>();
    for (auto element = _OverrideList.begin(); element != _OverrideList.end(); element++) {
        overrideList.push_back(*element->second);
    }

    return &overrideList;
}

LogOverrideEntry* LogOverrideManager::GetAddOverrideListElement(std::string className) {
    const std::string& key = className;
    auto element = _OverrideList.find(key);
    if (element == _OverrideList.end()) {
        auto newEntry = new LogOverrideEntry();
        newEntry->ClassName = className;
        element = _OverrideList.insert(LogOverrideMapElement(key, newEntry)).first;
    }

    return element->second;
}

LogOverrideEntry* LogOverrideManager::GetAddOverrideListElement(std::string className, std::string functionName) {
    std::string key = GetOverrideListKey(className, functionName);
    auto element = _OverrideList.find(key);
    if (element == _OverrideList.end()) {
        auto newEntry = new LogOverrideEntry();
        newEntry->ClassName = className;
        newEntry->FunctionName = functionName;
        element = _OverrideList.insert(LogOverrideMapElement(key, newEntry)).first;
    }

    return element->second;
}

std::string LogOverrideManager::GetOverrideListKey(std::string className, std::string functionName) {
    return className + OVERRIDE_LIST_KEY_DELIMITER + functionName;
}
