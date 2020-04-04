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

#include "TestHarnessApp.h"
#include "TestHarnessWindow.h"

IMPLEMENT_APP(TestHarnessApp)

bool TestHarnessApp::OnInit()
{
    HostServiceLocator serviceLocator;
    TextCtrlLogger& logger = *static_cast<TextCtrlLogger*>(serviceLocator.GetLogger());
    logger.SetLogLevel(LogLevel::Verbose);

    TestHarnessWindow* window = new TestHarnessWindow("Menrva Test Harness", wxDefaultPosition, wxSize(800, 600));
    window->Show();
    SetTopWindow(window);

    return true;
}
