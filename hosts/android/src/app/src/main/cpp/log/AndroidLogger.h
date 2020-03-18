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

#ifndef MENRVA_ANDROID_LOGGER_H
#define MENRVA_ANDROID_LOGGER_H

#include <string>
#include <map>
#include <menrvaEngine/host/HostLogger.h>

class AndroidLogger
        : public HostLogger {
public:
    AndroidLogger();

protected:
    void WriteLogLine(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) override;

private:
    static const std::string LOG_ELEMENT_DELIMITER,
                             FUNCTION_SUFFIX;
    static bool _Initialized;

    void Initialize();
};

#endif //MENRVA_ANDROID_LOGGER_H
