/* Menrva - Over-Engineered Tunable Android Audio Effects
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

/* --------------------------DO NOT EDIT THIS FILE--------------------------
 * This file has been generated by the generate_protobuf_commands.ps1 script.
 * To regenerate this file re-execute generate_protobuf_commands.ps1
 *
 * Template File : menrva/app/src/main/templates/CommandsEnum.h.template
 */

#ifndef MENRVA_MENRVA_COMMANDS_ENUM_H
#define MENRVA_MENRVA_COMMANDS_ENUM_H

enum MenrvaCommands {
    Host_Generic_Command = 0,

    Engine_GetLogLevel = 2,
    Engine_GetVersion = 1,
    Engine_SetLogLevel = 3,
};

#endif //MENRVA_MENRVA_COMMANDS_ENUM_H