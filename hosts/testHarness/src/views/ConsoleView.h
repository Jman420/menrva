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

#ifndef MENRVA_CONSOLE_VIEW_H
#define MENRVA_CONSOLE_VIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "../viewModels/ConsoleViewModel.h"

class ConsoleView {
public:
    explicit ConsoleView(ConsoleViewModel* viewModel);
    ~ConsoleView();

    wxWindow* GetWindow();

private:
    ConsoleViewModel* _ViewModel;
    wxFrame* _Frame;

    void Quit(wxCommandEvent& event);

    void DumpConsole(wxCommandEvent& event);
    void ClearConsole(wxCommandEvent& event);

    void SetLogLevel(wxCommandEvent& event);
    void ToggleLoggingOverrides(wxCommandEvent& event);

    void BindLogLevelMenuItem(LogLevel logLevel);
};

#endif //MENRVA_CONSOLE_VIEW_H
