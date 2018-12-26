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

#ifndef MENRVA_LOG_MANAGER_H
#define MENRVA_LOG_MANAGER_H

#include <string>
#include "../aosp/liblog/android/log.h"

enum LogLevel {
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3,
    VERBOSE = 4,
};

class Logger {
public:
    static LogLevel AppLogLevel;

    static void WriteLog(std::string message, std::string prefix = "", LogLevel logLevel = LogLevel::VERBOSE, ...);

private:
    static bool mInitialized;
    static void Initialize();

    Logger();
};

#endif //MENRVA_LOG_MANAGER_H
