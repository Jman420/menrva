// Author : Jman420

#include "logger.h"

bool Logger::mInitialized = false;
LogLevel Logger::AppLogLevel = LogLevel::ERROR;

void Logger::Initialize() {
    Logger::AppLogLevel = LogLevel::VERBOSE;
    // TODO : Get AppLogLevel from Shared Settings

    mInitialized = true;
}

void Logger::WriteLog(std::string message, std::string prefix, LogLevel logLevel, ...) {
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

Logger::Logger() {}
