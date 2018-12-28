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

bool AndroidLogger::mInitialized = false;

AndroidLogger::AndroidLogger() : LoggerBase() {
    Initialize();
}

void AndroidLogger::Initialize() {
    if (mInitialized) {
        return;
    }

    AppLogLevel = LogLevel::VERBOSE;
    // TODO : Get AppLogLevel from Shared Settings

    mInitialized = true;
}

void AndroidLogger::WriteLog(std::string message, std::string prefix, LogLevel logLevel, ...) {
    if (!mInitialized) {
        Initialize();
    }

    if (logLevel > AppLogLevel) {
        return;
    }

    va_list args;
    va_start(args, logLevel);
    const char* logTag = prefix.c_str();
    const char* logMsg = message.c_str();

    switch (logLevel) {
        case LogLevel::ERROR:
            __android_log_vprint(ANDROID_LOG_ERROR, logTag, logMsg, args);
            break;
        case LogLevel::WARNING:
            __android_log_vprint(ANDROID_LOG_WARN, logTag, logMsg, args);
            break;
        case LogLevel::INFO:
            __android_log_vprint(ANDROID_LOG_INFO, logTag, logMsg, args);
            break;
        case LogLevel::DEBUG:
            __android_log_vprint(ANDROID_LOG_DEBUG, logTag, logMsg, args);
            break;
        case LogLevel::VERBOSE:
            __android_log_vprint(ANDROID_LOG_VERBOSE, logTag, logMsg, args);
            break;
    }
}
