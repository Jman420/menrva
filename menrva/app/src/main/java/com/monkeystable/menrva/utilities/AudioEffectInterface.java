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

package com.monkeystable.menrva.utilities;

import android.media.audiofx.AudioEffect;

import com.google.protobuf.InvalidProtocolBufferException;
import com.monkeystable.menrva.abstracts.MenrvaCommand;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Locale;
import java.util.UUID;

public class AudioEffectInterface {
    public static final int MAX_RESPONSE_SIZE = 128;

    private static final String COMMAND_METHOD_NAME = "command";

    private static boolean _Initialized = false;
    private static Constructor _Constructor;
    private static Method _SendCommand;

    private AudioEffect _Effect;

    private static void initialize() throws NoSuchMethodException {
        try {
            _Constructor = AudioEffect.class.getConstructor(UUID.class, UUID.class, int.class, int.class);
        } catch (NoSuchMethodException e) {
            // TODO : Log Exception
            throw e;
        }

        try {
            _SendCommand = AudioEffect.class.getMethod(COMMAND_METHOD_NAME, int.class, byte[].class, byte[].class);
        } catch (NoSuchMethodException e) {
            // TODO : Log Exception
            throw e;
        }

        _Initialized = true;
    }

    public AudioEffectInterface(UUID type, UUID id, int priority, int audioSessionId)
            throws NoSuchMethodException, IllegalAccessException, InstantiationException, InvocationTargetException {

        if (!_Initialized) {
            AudioEffectInterface.initialize();
        }

        try {
            _Effect = (AudioEffect)_Constructor.newInstance(type, id, priority, audioSessionId);
        } catch (IllegalAccessException e) {
            // TODO : Log Exception
            throw e;
        } catch (InstantiationException e) {
            // TODO : Log Exception
            throw e;
        } catch (InvocationTargetException e) {
            // TODO : Log Exception
            throw e;
        }
    }

    public AudioEffect.Descriptor getDescriptor() {
        return _Effect.getDescriptor();
    }

    public boolean getEnabled() {
        return _Effect.getEnabled();
    }

    public void setEnabled(boolean enabled) {
        _Effect.setEnabled(enabled);
    }

    public void sendCommand(MenrvaCommand message)
            throws InvocationTargetException, IllegalAccessException, InvalidProtocolBufferException {
        byte[] requestBytes = message.getRequest().toByteArray();
        byte[] responseBuffer = new byte[MAX_RESPONSE_SIZE];
        int responseLength = invokeCommand(message.getCommandId(), requestBytes, responseBuffer);
        if (responseLength > responseBuffer.length) {
            String exceptionMsg = String.format(Locale.US,"Response Buffer Overflow.  Response Length %d exceeds Max Length %d.",
                    responseLength, responseBuffer.length);
            // TODO : Log Exception
            throw new InvalidProtocolBufferException(exceptionMsg);
        }

        byte[] responseBytes = Arrays.copyOfRange(responseBuffer, 0, responseLength);
        message.setResponse(message.getResponse().getParserForType().parseFrom(responseBytes));
    }

    private int invokeCommand(int command, byte[] value, byte[] result)
            throws IllegalAccessException, InvocationTargetException {
        try {
            return (int)_SendCommand.invoke(_Effect, command, value, result);
        } catch (IllegalAccessException e) {
            // TODO : Log Exception
            throw e;
        } catch (InvocationTargetException e) {
            // TODO : Log Exception
            throw e;
        }
    }
}
