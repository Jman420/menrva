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
#include <menrvaEngine/host/HostServiceLocator.h>
#include <menrvaEngine/engine/EffectsEngine.h>
#include "App.h"
#include "viewModels/ConsoleViewModel.h"
#include "views/ConsoleView.h"

IMPLEMENT_APP(TestHarnessApp)

bool TestHarnessApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxXmlResource& xmlResource = *wxXmlResource::Get();
    xmlResource.InitAllHandlers();
    xmlResource.SetFlags(wxXRC_USE_LOCALE | wxXRC_USE_ENVVARS);
    if (!xmlResource.LoadAllFiles("xrc")) {
        return false;
    }

    HostServiceLocator& serviceLocator = *new HostServiceLocator();
    TextCtrlLogger* logger = static_cast<TextCtrlLogger*>(serviceLocator.GetLogger());
    logger->SetLogLevel(LogLevel::Verbose);

    MenrvaEffectsEngine* effectsEngine = new MenrvaEffectsEngine(logger, serviceLocator.GetFftEngine(), &serviceLocator);

    ConsoleViewModel* consoleViewModel = new ConsoleViewModel(logger, effectsEngine);
    ConsoleView* consoleView = new ConsoleView(consoleViewModel);
    consoleView->GetWindow()->Show();
    SetTopWindow(consoleView->GetWindow());
    
    return true;
}
