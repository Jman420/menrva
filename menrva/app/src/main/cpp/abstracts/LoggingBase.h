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

#ifndef MENRVA_LOGGING_BASE_H
#define MENRVA_LOGGING_BASE_H

#include <string>
#include "LoggerBase.h"

class LoggingBase {
public:
    LoggingBase(LoggerBase* logger, std::string prettyFunction);

protected:
    std::string LOG_SENDER;
    LoggerBase* _Logger;

    /* InitializeLogSender() - Parses the Class Name from the provided __PRETTY_FUNCTION__ GCC
     *   Compiler Constant and stores it in LOG_SENDER for use in future calls to WriteLog();
     *   Call this method from a Deriving Class Constructor to Initialize LOG_SENDER
     * prettyFunction - The __PRETTY_FUNCTION__ Constant to parse
     */
    void InitializeLogSender(std::string prettyFunction);

private:
    static const std::string COLONS;
};

#endif //MENRVA_LOGGING_BASE_H
