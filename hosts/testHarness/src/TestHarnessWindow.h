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

#ifndef MENRVA_TEST_HARNESS_WINDOW_H
#define MENRVA_TEST_HARNESS_WINDOW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <menrvaEngine/host/HostServiceLocator.h>
#include <menrvaEngine/engine/EffectsEngine.h>
#include "tools/TextCtrlLogger.h"

enum TestHarnessWindowControls {
    TEXT_Console = wxID_HIGHEST + 1,
    
    // File Menu
    MENU_Quit,

    // Console Menu
    MENU_DumpConsole,
    MENU_ClearConsole,
    
    // Logging Menu
    MENU_SetLogLevel_Fatal,
    MENU_SetLogLevel_Error,
    MENU_SetLogLevel_Warn,
    MENU_SetLogLevel_Info,
    MENU_SetLogLevel_Debug,
    MENU_SetLogLevel_Verbose,
    MENU_ToggleLoggingOverrides,
    MENU_ManageLoggingOverrides,

    // Playback Menu
    MENU_LoadAudioFile,
    MENU_PlayAudioFile,
    MENU_StopAudioFile,

    // Engine Menu
    MENU_ToggleEngine,
};

class TestHarnessWindow
        : public wxFrame
{
public:
    TestHarnessWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    void Quit(wxCommandEvent& event);
    void DumpConsole(wxCommandEvent& event);
    void ClearConsole(wxCommandEvent& event);

private:
    static const std::string CONSOLE_HEADER;

    wxMenuBar* _MenuBar;
    wxTextCtrl* _Console;

    HostServiceLocator* _ServiceLocator;
    TextCtrlLogger* _Logger;
    MenrvaEffectsEngine* _EffectsEngine;

    DECLARE_EVENT_TABLE();
};

#endif //MENRVA_TEST_HARNESS_WINDOW_H
