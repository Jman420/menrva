/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
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

#include "Engine_GetVersion_Handler.h"

Engine_GetVersion_Handler::Engine_GetVersion_Handler(LogWriterBase* logger)
        : TypedCommandHandlerBase(new Engine_GetVersion_Command(), logger, __PRETTY_FUNCTION__) {}

void Engine_GetVersion_Handler::Execute(ModuleContext& context) {
    _Logger->WriteLog("Received GetVersion Command...", LOG_SENDER, __func__);
    messages::Engine_GetVersion_Response& response = *_TypedCommand->GetTypedResponse();

    _Logger->WriteLog("Setting Engine Version on Response Object...", LOG_SENDER, __func__);
    response.set_major(MENRVA_ENGINE_MAJOR);
    response.set_minor(MENRVA_ENGINE_MINOR);
    response.set_patch(MENRVA_ENGINE_PATCH);

    _Logger->WriteLog("Successfully set Engine Version on Response Object.", LOG_SENDER, __func__);
}
