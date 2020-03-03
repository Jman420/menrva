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

#ifndef MENRVA_COMMAND_HANDLER_MAP_H
#define MENRVA_COMMAND_HANDLER_MAP_H

#include <map>
#include "../command_handlers/CommandHandlerBase.h"

typedef std::map<uint32_t, CommandHandlerBase*> handler_map;

class CommandHandlerMap {
public:
    CommandHandlerMap(LoggerBase* logger);

    handler_map* GetHandlerMap();
    CommandHandlerBase* GetCommandHandler(uint32_t commandId);

private:
    static bool _Initialized;
    static handler_map* _HandlerMap;

    static void Initialize(LoggerBase* logger);
};

#endif //MENRVA_COMMAND_HANDLER_MAP_H
