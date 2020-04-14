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
#include "ILogOverrideManager.h"

typedef std::map<std::string, LogOverrideEntry*> LogOverrideMap;
typedef std::pair<std::string, LogOverrideEntry*> LogOverrideMapElement;

class LogOverrideManager 
    : public ILogOverrideManager {
public:
    virtual void SetOverrideListEnabled(bool enabled) override;
    virtual bool GetOverrideListEnabled() override;

    virtual void SetOverrideListEntry(std::string className, bool enabled) override;
    virtual void SetOverrideListEntry(std::string className, LogLevel logLevel) override;
    virtual void SetOverrideListEntry(std::string className, bool enabled, LogLevel logLevel) override;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, bool enabled) override;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel) override;
    virtual void SetOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel) override;

    virtual void RemoveOverrideListEntry(std::string className) override;
    virtual void RemoveOverrideListEntry(std::string className, std::string functionName) override;

    virtual bool CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel) override;
    virtual std::vector<LogOverrideEntry>* GetOverrideList() override;

protected:
    LogOverrideEntry* GetAddOverrideListElement(std::string className);
    LogOverrideEntry* GetAddOverrideListElement(std::string className, std::string functionName);

    std::string GetOverrideListKey(std::string className, std::string functionName);

private:
    const static std::string OVERRIDE_LIST_KEY_DELIMITER;

    static bool _OverrideListEnabled;
    static LogOverrideMap _OverrideList;
};

#endif //MENRVA_LOG_OVERRIDE_MANAGER_H
