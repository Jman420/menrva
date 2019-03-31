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

#ifndef MENRVA_LOGGER_BASE_H
#define MENRVA_LOGGER_BASE_H

#include <string>
#include <map>

enum LogLevel {
    VERBOSE = 2,
    DEBUG = 3,
    INFO = 4,
    WARN = 5,
    ERROR = 6,
    FATAL = 7,
};

struct logger_override_entry {
    std::string ClassName,
                FunctionName;
    bool Enabled;
    LogLevel ComponentLogLevel;
};
typedef std::map<std::string, logger_override_entry*> logger_override_list;
typedef std::pair<std::string, logger_override_entry*> logger_override_list_element;

typedef std::map<std::string, LogLevel> log_level_map;
typedef std::pair<std::string, LogLevel> log_level_map_element;

class LoggerBase {
public:
    static const log_level_map LOG_LEVEL_MAP;

    void SetAppLogLevel(LogLevel logLevel);
    LogLevel GetAppLogLevel();

    void WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, ...);
    void WriteLog(std::string message, std::string senderClass, LogLevel logLevel, ...);
    void WriteLog(std::string message, LogLevel logLevel, ...);

    void WriteLog(std::string message, std::string senderClass, std::string senderFunction, ...);
    void WriteLog(std::string message, std::string senderClass, ...);
    void WriteLog(std::string message, ...);

    void SetOverrideListEnabled(bool enabled);
    bool GetOverrideListEnabled();

    void UpsertOverrideListEntry(std::string className, bool enabled);
    void UpsertOverrideListEntry(std::string className, LogLevel logLevel);
    void UpsertOverrideListEntry(std::string className, bool enabled, LogLevel logLevel);
    void UpsertOverrideListEntry(std::string className, std::string functionName, bool enabled);
    void UpsertOverrideListEntry(std::string className, std::string functionName, LogLevel logLevel);
    void UpsertOverrideListEntry(std::string className, std::string functionName, bool enabled, LogLevel logLevel);

    void RemoveOverrideListEntry(std::string className);
    void RemoveOverrideListEntry(std::string className, std::string functionName);

    bool CheckOverrideList(std::string className, std::string functionName, LogLevel logLevel);

protected:
    const static std::string APP_NAME;

    static LogLevel _AppLogLevel;
    static bool _OverrideListEnabled;
    static logger_override_list _OverrideList;

    virtual void WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, va_list args) = 0;

    logger_override_entry* GetAddOverrideListElement(std::string className);
    logger_override_entry* GetAddOverrideListElement(std::string className, std::string functionName);

    std::string GetOverrideListKey(std::string className, std::string functionName);

private:
    const static std::string OVERRIDE_LIST_KEY_DELIMITER;
    const static LogLevel DEFAULT_LOG_LEVEL;
    const static LogLevel DEFAULT_APP_LOG_LEVEL;
};

#endif //MENRVA_LOGGER_BASE_H
