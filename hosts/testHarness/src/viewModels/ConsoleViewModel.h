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

#ifndef MENRVA_CONSOLE_VIEW_MODEL_H
#define MENRVA_CONSOLE_VIEW_MODEL_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <menrvaEngine/engine/EffectsEngine.h>
#include <menrvaEngine/log/LogLevels.h>
#include "../tools/TextCtrlLogger.h"

class ConsoleViewModel {
public:
    explicit ConsoleViewModel(TextCtrlLogger* logger, MenrvaEffectsEngine* effectsEngine);
    ~ConsoleViewModel();

    wxTextCtrl* GetConsoleCtrl();
    void DumpConsole(std::string filePath);
    void ClearConsole();

    void SetLogLevel(LogLevel logLevel);

private:
    static const std::string CONSOLE_HEADER;

    TextCtrlLogger* _Logger;
    wxTextCtrl* _Console;
    MenrvaEffectsEngine* _EffectsEngine;
};

#endif //MENRVA_CONSOLE_VIEW_MODEL_H
