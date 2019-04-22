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

#include <android/log.h>
#include "AndroidLogger.h"

const std::string AndroidLogger::LOG_ELEMENT_DELIMITER = ".";
const std::string AndroidLogger::FUNCTION_SUFFIX = "()";

bool AndroidLogger::_Initialized = false;

AndroidLogger::AndroidLogger() : LoggerBase() {
    Initialize();
}

void AndroidLogger::Initialize() {
    if (_Initialized) {
        return;
    }

    // BEGIN DEBUG
    SetAppLogLevel(LogLevel::DEBUG);

    SetOverrideListEnabled(false);
    UpsertOverrideListEntry("MenrvaEffectsEngine", true, LogLevel::VERBOSE);
    // END DEBUG

    // TODO : Get AppLogLevel & Whitelist Settings from Shared Settings

    _Initialized = true;
}

void AndroidLogger::WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, va_list args) {
    if (GetOverrideListEnabled() && CheckOverrideList(senderClass, senderFunction, logLevel)) {
        WriteLogCatMsg(message, senderClass, senderFunction, logLevel, args);
        return;
    }

    if (logLevel >= GetAppLogLevel()) {
        WriteLogCatMsg(message, senderClass, senderFunction, logLevel, args);
    }
}

void AndroidLogger::WriteLogCatMsg(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, va_list args) {
    // Format Log Tag
    std::string prefix = APP_NAME;
    if (!senderClass.empty()) {
        prefix = prefix + LOG_ELEMENT_DELIMITER + senderClass;
    }
    if (!senderFunction.empty()) {
        prefix = prefix + LOG_ELEMENT_DELIMITER + senderFunction + FUNCTION_SUFFIX;
    }

    // Write Message
    __android_log_vprint(logLevel, prefix.c_str(), message.c_str(), args);
}