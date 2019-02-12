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

const std::string AndroidLogger::LOG_ELEMENT_DELIMITER = ".";
const std::string AndroidLogger::FUNCTION_SUFFIX = "()";

bool AndroidLogger::_Initialized = false;
bool AndroidLogger::_WhitelistActive = true;
logger_whitelist AndroidLogger::_Whitelist = *new logger_whitelist();

AndroidLogger::AndroidLogger() : LoggerBase() {
    Initialize();
}

void AndroidLogger::Initialize() {
    if (_Initialized) {
        return;
    }

    // BEGIN DEBUG
    AppLogLevel = LogLevel::DEBUG;

    _WhitelistActive = false;
    logger_whitelist_entry entry = *new logger_whitelist_entry();
    entry.Name = "MenrvaEffectsEngine";
    entry.Enabled = true;
    entry.ComponentLogLevel = LogLevel::VERBOSE;
    _Whitelist.insert(logger_whitelist_element(entry.Name, entry));
    // END DEBUG

    // TODO : Get AppLogLevel & Whitelist Settings from Shared Settings

    _Initialized = true;
}

void AndroidLogger::WriteLog(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel, va_list args) {
    if (!_Initialized) {
        Initialize();
    }

    // Check Whitelist for Class Disabled
    if (_WhitelistActive) {
        logger_whitelist whitelist = AndroidLogger::_Whitelist;
        auto whitelistEntry = whitelist.find(senderClass);
        if (whitelistEntry != whitelist.end() && whitelistEntry->second.Enabled && logLevel >= whitelistEntry->second.ComponentLogLevel) {
            WriteLogCatMsg(message, senderClass, senderFunction, logLevel, args);
            return;
        }
    }

    // Check if Log Level is Disabled
    if (logLevel >= AppLogLevel) {
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
