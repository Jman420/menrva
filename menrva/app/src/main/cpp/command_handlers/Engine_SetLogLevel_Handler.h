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

#ifndef MENRVA_ENGINE_Engine_SetLogLevel_HANDLER_H
#define MENRVA_ENGINE_Engine_SetLogLevel_HANDLER_H

#include "TypedCommandHandlerBase.h"
#include "../commands/Engine_SetLogLevel_Command.h"

class Engine_SetLogLevel_Handler
        : public TypedCommandHandlerBase<Engine_SetLogLevel_Command> {
public:
    Engine_SetLogLevel_Handler(LoggerBase* logger);

    virtual bool Execute(MenrvaModuleContext& context) override;
};

#endif //MENRVA_ENGINE_Engine_SetLogLevel_HANDLER_H
