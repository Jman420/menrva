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

#include "LogLevels.h"

 const to_string_map LogLevels::LOG_LEVEL_MAP = {
    { LogLevel::Fatal, "Fatal" },
    { LogLevel::Error, "Error" },
    { LogLevel::Warn, "Warn" },
    { LogLevel::Info, "Info" },
    { LogLevel::Debug, "Debug" },
    { LogLevel::Verbose, "Verbose" },
};

std::string LogLevels::ConvertToString(LogLevel logLevel)
{
    auto element = LOG_LEVEL_MAP.find(logLevel);
    if (element == LOG_LEVEL_MAP.end()) {
        return std::string();
    }

    return element->second;
}

int LogLevels::GetLength()
{
    return LOG_LEVEL_MAP.size();
}
