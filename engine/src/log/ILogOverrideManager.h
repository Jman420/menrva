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

#ifndef MENRVA_ILOG_OVERRIDE_MANAGER_H
#define MENRVA_ILOG_OVERRIDE_MANAGER_H

#include <vector>
#include "LogLevels.h"

struct LogOverrideEntry {
    std::string ClassName,
        FunctionName;
    bool Enabled;
    LogLevel ComponentLogLevel;
};

class ILogOverrideManager {
public:
    virtual ~ILogOverrideManager() = default;

    virtual void SetOverrideListEnabled(bool enabled) = 0;
    virtual bool GetOverrideListEnabled() = 0;

    virtual void SetOverrideListEntry(std::string className, bool enabled) = 0;
    virtual void SetOverrideListEntry(std::string className, LogLevel logLevel) = 0;
    virtual void SetOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) = 0;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, bool enabled) = 0;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel) = 0;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel) = 0;

    virtual void RemoveOverrideListEntry(std::string className) = 0;
    virtual void RemoveOverrideListEntry(std::string className, std::string functionName) = 0;

    virtual bool CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel) = 0;
    virtual std::vector<LogOverrideEntry>* GetOverrideList() = 0;
};

#endif //MENRVA_ILOG_OVERRIDE_MANAGER_H
