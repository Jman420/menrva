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

const std::string TestHarnessWindow::CONSOLE_HEADER = "-----Menrva TestHarness Console-----";

BEGIN_EVENT_TABLE(TestHarnessWindow, wxFrame)
    EVT_MENU(TestHarnessWindowControls::MENU_Quit, TestHarnessWindow::Quit)
    EVT_MENU(TestHarnessWindowControls::MENU_DumpConsole, TestHarnessWindow::DumpConsole)
    EVT_MENU(TestHarnessWindowControls::MENU_ClearConsole, TestHarnessWindow::ClearConsole)
END_EVENT_TABLE()

TestHarnessWindow::TestHarnessWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(nullptr, -1, title, pos, size) {
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(TestHarnessWindowControls::MENU_Quit, _("&Quit"), _("&Quit Application"));

    wxMenu* consoleMenu = new wxMenu();
    consoleMenu->Append(TestHarnessWindowControls::MENU_DumpConsole, _("&Dump to File"), _("&Dump Console to File"));
    consoleMenu->AppendSeparator();
    consoleMenu->Append(TestHarnessWindowControls::MENU_ClearConsole, _("&Clear"), _("&Clear Console"));

    wxMenu* loggingMenu = new wxMenu();
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Fatal, _("&Fatal"), _("Set LogLevel to &Fatal"));
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Error, _("&Error"), _("Set LogLevel to &Error"));
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Warn, _("&Warn"), _("Set LogLevel to &Warn"));
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Info, _("&Info"), _("Set LogLevel to &Info"));
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Debug, _("&Debug"), _("Set LogLevel to &Debug"));
    loggingMenu->AppendRadioItem(TestHarnessWindowControls::MENU_SetLogLevel_Verbose, _("&Verbose"), _("Set LogLevel to &Verbose"));
    loggingMenu->AppendSeparator();
    loggingMenu->AppendCheckItem(TestHarnessWindowControls::MENU_ToggleLoggingOverrides, _("&Overrides Enabled"), _("Enable/Disable Logging Overrides"));
    loggingMenu->Append(TestHarnessWindowControls::MENU_ManageLoggingOverrides, _("&Manage Overrides"), _("&Manage Log Level Overrides"));

    wxMenu* playbackMenu = new wxMenu();
    playbackMenu->Append(TestHarnessWindowControls::MENU_LoadAudioFile, _("&Load"), _("&Load Audio File for Playback"));
    playbackMenu->Append(TestHarnessWindowControls::MENU_PlayAudioFile, _("&Play"), _("&Play Audio File"));
    playbackMenu->Append(TestHarnessWindowControls::MENU_StopAudioFile, _("&Stop"), _("&Stop Audio File"));

    wxMenu* engineMenu = new wxMenu();
    engineMenu->AppendCheckItem(TestHarnessWindowControls::MENU_ToggleEngine, _("&Engine Enabled"), _("&Enable/Disable Effects Engine"));

    _MenuBar = new wxMenuBar();
    _MenuBar->Append(fileMenu, _("&File"));
    _MenuBar->Append(consoleMenu, _("&Console"));
    _MenuBar->Append(loggingMenu, _("&Logging"));
    _MenuBar->Append(playbackMenu, _("&Playback"));
    _MenuBar->Append(engineMenu, _("&Engine"));
    SetMenuBar(_MenuBar);

    _ServiceLocator = new HostServiceLocator();
    HostServiceLocator& serviceLocator = *_ServiceLocator;
    _Logger = static_cast<TextCtrlLogger*>(serviceLocator.GetLogger());
    
    _Console = _Logger->GetTextCtrl();
    _Console->Create(this, TestHarnessWindowControls::TEXT_Console, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    _Logger->WriteLog(CONSOLE_HEADER);
    _Logger->WriteLog("First Test Line!");
        
    _EffectsEngine = new MenrvaEffectsEngine(_Logger, serviceLocator.GetFftEngine(), _ServiceLocator);
}

void TestHarnessWindow::Quit(wxCommandEvent& event)
{
    Close(true);
}

void TestHarnessWindow::DumpConsole(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, _("Save Console to File..."), wxEmptyString, "consoleDump.txt", "Text Files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    _Console->SaveFile(saveFileDialog.GetFilename());
}

void TestHarnessWindow::ClearConsole(wxCommandEvent& event)
{
    _Console->Clear();
    _Logger->WriteLog(CONSOLE_HEADER);
}
