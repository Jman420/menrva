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

package com.monkeystable.menrva.abstracts;

import com.google.protobuf.MessageLite;
import com.monkeystable.menrva.commands.MenrvaCommands;

public abstract class MenrvaCommand<TRequest extends MessageLite, TResponse extends MessageLite> {
    static private final int FIRST_PROPRIETARY_CMD = 0x10000;

    private int _CommandId;
    private TRequest _Request;
    private TResponse _Response;

    public MenrvaCommand(MenrvaCommands command, TRequest request, TResponse response) {
        _CommandId = FIRST_PROPRIETARY_CMD + command.ordinal();
        _Request = request;
        _Response = response;
    }

    public int getCommandId() {
        return _CommandId;
    }

    public TRequest getRequest() {
        return _Request;
    }

    public TResponse getResponse() {
        return _Response;
    }

    public void setResponse(TResponse value) {
        _Response = value;
    }
}