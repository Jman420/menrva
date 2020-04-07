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

#include <menrvaEngine/tools/StringOperations.h>
#include "ConsoleViewModel.h"
#include "../eventData/SetLogLevel_EventData.h"

const std::string ConsoleViewModel::CONSOLE_HEADER = "-----Menrva TestHarness Console-----";

ConsoleViewModel::ConsoleViewModel(TextCtrlLogger* logger, MenrvaEffectsEngine* effectsEngine)
{
    _Logger = logger;
    _Console = _Logger->GetTextCtrl();
    _EffectsEngine = effectsEngine;

    _Logger->WriteLog(CONSOLE_HEADER);
}

ConsoleViewModel::~ConsoleViewModel()
{
    delete _Logger;
    delete _EffectsEngine;
}

wxTextCtrl* ConsoleViewModel::GetConsoleCtrl()
{
    return _Console;
}

void ConsoleViewModel::DumpConsole(std::string filePath)
{
    GetConsoleCtrl()->SaveFile(filePath);
    _Logger->WriteLog(StringOperations::FormatString("Console saved to : %s", filePath.c_str()));
}

void ConsoleViewModel::ClearConsole()
{
    _Console->Clear();
    _Logger->WriteLog(CONSOLE_HEADER);
}

void ConsoleViewModel::SetLogLevel(LogLevel logLevel)
{
    _Logger->SetLogLevel(logLevel);
    _Logger->WriteLog(StringOperations::FormatString("Set LogLevel to : %s", LogLevels::ConvertToString(_Logger->GetLogLevel()).c_str()));
}

void ConsoleViewModel::ToggleLoggingOverrides()
{
    _Logger->SetOverrideListEnabled(!_Logger->GetOverrideListEnabled());
    _Logger->WriteLog(StringOperations::FormatString("Logging Overrides Toggled : %s", StringOperations::Convert(_Logger->GetOverrideListEnabled())));
}
