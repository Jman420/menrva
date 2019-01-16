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

#include "logging_base.h"

const std::string LoggingBase::COLONS = "::";

LoggingBase::LoggingBase(LoggerBase* logger, std::string prettyFunction) {
    InitializeLogSender(prettyFunction);
    _Logger = logger;
}

void LoggingBase::InitializeLogSender(std::string prettyFunction) {
    std::string className = "";
    size_t colons = prettyFunction.find(COLONS);
    if (colons == std::string::npos) {
        className = "";
    }
    else {
        size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
        size_t end = colons - begin;

        className =  prettyFunction.substr(begin,end);
    }

    LOG_SENDER = className;
}
