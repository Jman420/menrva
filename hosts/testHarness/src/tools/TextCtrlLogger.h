/* Menrva - Audio Effect Engine supporting Plug'n'Play style DSP Effects
 * Copyright (C) 2020 Justin Giannone (aka Jman420)
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

#ifndef MENRVA_ConsoleLogger_H
#define MENRVA_ConsoleLogger_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <menrvaEngine/host/HostLogger.h>

class TextCtrlLogger 
        : public HostLogger {
public:
    TextCtrlLogger();

    wxTextCtrl* GetTextCtrl();

protected:
    virtual void WriteLogLine(std::string message, std::string senderClass, std::string senderFunction, LogLevel logLevel) override;

private:
    const static std::string TIMESTAMP_FORMAT;
    const static std::string PREFIX_FORMAT;
    const static std::string SENDER_FUNC_FORMAT;
    const static std::string NEW_LINE;

    static wxTextCtrl* _Console;
    static bool _Initialized;

    static std::string GetTimestamp();
    static void Initialize();
};

#endif //MENRVA_ConsoleLogger_H
