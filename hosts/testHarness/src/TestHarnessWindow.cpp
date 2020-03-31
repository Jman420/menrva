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

#include "TestHarnessWindow.h"

BEGIN_EVENT_TABLE(TestHarnessWindow, wxFrame)
    EVT_MENU(TestHarnessWindowControls::MENU_Quit, TestHarnessWindow::Quit)
END_EVENT_TABLE()

TestHarnessWindow::TestHarnessWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(nullptr, -1, title, pos, size) {
    _MenuBar = new wxMenuBar();


    _ServiceLocator = new HostServiceLocator();
    HostServiceLocator& serviceLocator = *_ServiceLocator;
    _Logger = static_cast<TextCtrlLogger*>(serviceLocator.GetLogger());

    _Console = _Logger->GetTextCtrl();
    wxTextCtrl& console = *_Console;
    console.SetParent(this);
    console.SetId(TestHarnessWindowControls::TEXT_Console);
    console.SetPosition(wxDefaultPosition);
    console.SetSize(wxDefaultSize);
    console.SetWindowStyle(wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY);
    _Logger->WriteLog("-----Menrva TestHarness Console-----");

    _EffectsEngine = new MenrvaEffectsEngine(_Logger, serviceLocator.GetFftEngine(), _ServiceLocator);
}

void TestHarnessWindow::Quit(wxCommandEvent& event)
{
    Close(true);
}
