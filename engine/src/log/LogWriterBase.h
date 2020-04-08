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

#ifndef MENRVA_LOG_WRITER_BASE_H
#define MENRVA_LOG_WRITER_BASE_H

#include <string>
#include "LogLevels.h"
#include "LogOverrideManager.h"

class LogWriterBase {
public:
    static const uint8_t MIN_LOG_LEVEL_VALUE;
    static const uint8_t MAX_LOG_LEVEL_VALUE;

    void SetLogLevelLimit(LogLevel logLevel);
    LogLevel GetLogLevelLimit();

    LogOverrideManager* GetLogOverrideManager();

    void WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel);
    void WriteLog(std::string message, std::string senderClass, LogLevel logLevel);
    void WriteLog(std::string message, LogLevel logLevel);

    void WriteLog(std::string message, std::string senderClass, std::string senderFunction);
    void WriteLog(std::string message, std::string senderClass);
    void WriteLog(std::string message);

protected:
    virtual void WriteLogLine(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) = 0;

private:
    const static LogLevel DEFAULT_LOG_LEVEL;
    const static LogLevel START_UP_LOG_LEVEL;

    static LogLevel _LogLevelLimit;
    static LogOverrideManager* _LogOverrideManager;
};

#endif //MENRVA_LOG_WRITER_BASE_H
