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
    SetLogLevel(LogLevel::DEBUG);

    SetOverrideListEnabled(false);
    UpsertOverrideListEntry("MenrvaEffectsEngine", true, LogLevel::VERBOSE);
    // END DEBUG

    // TODO : Get LogLevel & Whitelist Settings from Persisted Settings

    _Initialized = true;
}

void AndroidLogger::WriteLogLine(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) {
    // Format Log Tag
    std::string prefix = APP_NAME;
    if (!senderClass.empty()) {
        prefix = prefix + LOG_ELEMENT_DELIMITER + senderClass;
    }
    if (!senderFunction.empty()) {
        prefix = prefix + LOG_ELEMENT_DELIMITER + senderFunction + FUNCTION_SUFFIX;
    }

    // Write Message
    __android_log_write(logLevel, prefix.c_str(), message.c_str());
}
