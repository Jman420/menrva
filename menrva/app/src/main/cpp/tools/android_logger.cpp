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

#include "android_logger.h"
#include "../aosp/liblog/android/log.h"

bool AndroidLogger::_Initialized = false;

AndroidLogger::AndroidLogger() : LoggerBase() {
    Initialize();
}

void AndroidLogger::Initialize() {
    if (_Initialized) {
        return;
    }

    AppLogLevel = LogLevel::VERBOSE;
    // TODO : Get AppLogLevel from Shared Settings

    _Initialized = true;
}

void AndroidLogger::WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, va_list args) {
    if (!_Initialized) {
        Initialize();
    }

    if (logLevel > AppLogLevel) {
        return;
    }

    std::string prefix = APP_NAME;
    if (senderClass != "") {
        prefix = prefix + "-" + senderClass;
    }
    if (senderFunction != "") {
        prefix = prefix + " - " + senderFunction + "()";
    }

    const char* logTag = prefix.c_str();
    const char* logMsg = message.c_str();
    android_LogPriority logPriority = static_cast<android_LogPriority>(logLevel);

    __android_log_vprint(logPriority, logTag, logMsg, args);
}
