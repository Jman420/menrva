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

#ifndef MENRVA_LOG_LEVELS_H
#define MENRVA_LOG_LEVELS_H

#include <map>
#include <string>

enum class LogLevel : uint8_t {
    Verbose = 2,
    Debug = 3,
    Info = 4,
    Warn = 5,
    Error = 6,
    Fatal = 7,
};

typedef std::map<LogLevel, std::string> to_string_map;
typedef std::pair<LogLevel, std::string> to_string_element;

class LogLevels {
public:
    static std::string ConvertToString(LogLevel logLevel);
    static int GetLength();

private:
    static const to_string_map LOG_LEVEL_MAP;
};

#endif //MENRVA_LOG_LEVELS_H
