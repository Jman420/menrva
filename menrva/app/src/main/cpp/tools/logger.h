// Author : Jman420

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
