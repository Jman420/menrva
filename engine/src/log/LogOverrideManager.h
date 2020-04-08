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

#ifndef MENRVA_LOG_OVERRIDE_MANAGER_H
#define MENRVA_LOG_OVERRIDE_MANAGER_H

#include <map>
#include <string>
#include "LogLevels.h"

struct logger_override_entry {
    std::string ClassName,
        FunctionName;
    bool Enabled;
    LogLevel ComponentLogLevel;
};
typedef std::map<std::string, logger_override_entry*> logger_override_list;
typedef std::pair<std::string, logger_override_entry*> logger_override_list_element;

class LogOverrideManager {
public:
    LogOverrideManager();

    void SetOverrideListEnabled(bool enabled);
    bool GetOverrideListEnabled();

    void SetOverrideListEntry(std::string className, bool enabled);
    void SetOverrideListEntry(std::string className, LogLevel logLevel);
    void SetOverrideListEntry(std::string className, bool enabled, LogLevel logLevel);
    void SetOverrideListEntry(std::string className, std::string functionName, bool enabled);
    void SetOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel);
    void SetOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel);

    void RemoveOverrideListEntry(std::string className);
    void RemoveOverrideListEntry(std::string className, std::string functionName);

    bool CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel);

protected:
    logger_override_entry* GetAddOverrideListElement(std::string className);
    logger_override_entry* GetAddOverrideListElement(std::string className, std::string functionName);

    std::string GetOverrideListKey(std::string className, std::string functionName);

private:
    const static std::string OVERRIDE_LIST_KEY_DELIMITER;

    static bool _OverrideListEnabled;
    static logger_override_list _OverrideList;
};

#endif //MENRVA_LOG_OVERRIDE_MANAGER_H
