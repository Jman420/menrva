/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2018 Justin Giannone (aka Jman420)
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

package com.monkeystable.menrva.commands;

import com.monkeystable.menrva.commands.base.MenrvaCommand;
import com.monkeystable.menrva.commands.messages.Engine_GetVersion.Engine_GetVersion_Request;
import com.monkeystable.menrva.commands.messages.Engine_GetVersion.Engine_GetVersion_Response;

public class Engine_GetVersion
        extends MenrvaCommand<Engine_GetVersion_Request, Engine_GetVersion_Response> {
    public Engine_GetVersion() {
        super(EngineCommands.GET_VERSION.ordinal(), Engine_GetVersion_Request.newBuilder().build(), Engine_GetVersion_Response.newBuilder().build());
    }
}
