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

#include <wx/xrc/xmlres.h>
#include "ConsoleView.h"
#include "../eventData/SetLogLevel_EventData.h"

ConsoleView::ConsoleView(ConsoleViewModel* viewModel)
{
    _ViewModel = viewModel;

    wxXmlResource& xmlResource = *wxXmlResource::Get();
    _ConsoleFrame = xmlResource.LoadFrame(NULL, "ConsoleFrame");
    wxFrame& consoleFrame = *_ConsoleFrame;

    // Inject Console Text Control into ConsoleFrame
    wxTextCtrl* console = viewModel->GetConsoleCtrl();
    console->Create(_ConsoleFrame, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_RICH2);
    wxSizer* consoleSizer = consoleFrame.GetSizer();
    consoleSizer->Add(console, 0, wxALL | wxEXPAND);
    _ViewModel->ClearConsole();
    
    consoleFrame.Bind(wxEVT_COMMAND_MENU_SELECTED, &ConsoleView::Quit, this, XRCID("QuitMenuItem"));

    consoleFrame.Bind(wxEVT_COMMAND_MENU_SELECTED, &ConsoleView::DumpConsole, this, XRCID("DumpConsoleMenuItem"));
    consoleFrame.Bind(wxEVT_COMMAND_MENU_SELECTED, &ConsoleView::ClearConsole, this, XRCID("ClearConsoleMenuItem"));

    BindLogLevelMenuItem(LogLevel::Verbose);
    BindLogLevelMenuItem(LogLevel::Debug);
    BindLogLevelMenuItem(LogLevel::Info);
    BindLogLevelMenuItem(LogLevel::Warn);
    BindLogLevelMenuItem(LogLevel::Error);
    BindLogLevelMenuItem(LogLevel::Fatal);
}

ConsoleView::~ConsoleView()
{
    delete _ConsoleFrame;
    delete _ViewModel;
}

wxWindow* ConsoleView::GetWindow()
{
    return _ConsoleFrame;
}

void ConsoleView::Quit(wxCommandEvent& event)
{
    _ConsoleFrame->Close(true);
}

void ConsoleView::DumpConsole(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(_ConsoleFrame, _("Save Console to File..."), wxEmptyString, "consoleDump.txt", "Text Files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    _ViewModel->DumpConsole(saveFileDialog.GetFilename().ToStdString());
}

void ConsoleView::ClearConsole(wxCommandEvent& event)
{
    _ViewModel->ClearConsole();
}

void ConsoleView::SetLogLevel(wxCommandEvent& event)
{
    SetLogLevel_EventData& eventData = *static_cast<SetLogLevel_EventData*>(event.GetEventUserData());
    _ViewModel->SetLogLevel(eventData.GetLogLevel());
}

void ConsoleView::BindLogLevelMenuItem(LogLevel logLevel)
{
    std::string menuItemName = LogLevels::ConvertToString(logLevel);
    menuItemName += "LoggingMenuItem";
    int menuItemId = XRCID(menuItemName.c_str());
    SetLogLevel_EventData* eventData = new SetLogLevel_EventData(logLevel);

    _ConsoleFrame->Bind(wxEVT_COMMAND_MENU_SELECTED, &ConsoleView::SetLogLevel, this, menuItemId, menuItemId, eventData);
}
