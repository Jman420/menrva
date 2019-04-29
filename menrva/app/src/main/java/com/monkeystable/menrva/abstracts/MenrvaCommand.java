/*
 * Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
 * File last modified : 4/20/19 9:16 AM
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

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.MessageLite;
import com.monkeystable.menrva.commands.MenrvaCommands;

public abstract class MenrvaCommand<TRequestBuilder extends MessageLite.Builder, TResponse extends MessageLite> {
    private int _CommandId;
    private TRequestBuilder _RequestBuilder;
    private TResponse.Builder _ResponseBuilder;
    private TResponse _Response;

    public MenrvaCommand(MenrvaCommands command, TRequestBuilder requestBuilder, TResponse.Builder responseBuilder) {
        _CommandId = command.getCommandId();
        _RequestBuilder = requestBuilder;
        _ResponseBuilder = responseBuilder;
    }

    public int getCommandId() {
        return _CommandId;
    }

    public TRequestBuilder getRequestBuilder() {
        return _RequestBuilder;
    }

    public TResponse getResponse() {
        return _Response;
    }

    public byte[] serializeRequest() {
        byte[] requestBytes = _RequestBuilder.build().toByteArray();
        _RequestBuilder.clear();

        return requestBytes;
    }

    public void deserializeResponse(byte[] responseBytes)
            throws InvalidProtocolBufferException {
        _ResponseBuilder.mergeFrom(responseBytes);

        //noinspection unchecked
        _Response = (TResponse)_ResponseBuilder.build();

        _ResponseBuilder.clear();
    }
}
