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

Engine_GetVersion_Handler::Engine_GetVersion_Handler(LoggerBase* logger)
        : TypedCommandHandlerBase(new Engine_GetVersion_Command(), logger, __PRETTY_FUNCTION__) {}

bool Engine_GetVersion_Handler::Execute(MenrvaModuleContext& context) {
    messages::Engine_GetVersion_Response& response = *_TypedCommand->GetTypedResponse();

    response.set_major(MENRVA_ENGINE_MAJOR);
    response.set_minor(MENRVA_ENGINE_MINOR);
    response.set_patch(MENRVA_ENGINE_PATCH);

    return true;
}
