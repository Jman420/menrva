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

package com.monkeystable.menrva.utilities;

import android.media.audiofx.AudioEffect;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.nio.charset.StandardCharsets;
import java.util.UUID;

public class AudioEffectInterface {
    private static boolean _Initialized = false;
    private static Constructor _Constructor;
    private static Method _GetParameter;
    private static Method _SetParameter;

    private AudioEffect _Effect;
    private ByteConverter _ByteConverter;

    private static void initialize() throws NoSuchMethodException {
        try {
            _Constructor = AudioEffect.class.getConstructor(UUID.class, UUID.class, int.class, int.class);
        } catch (NoSuchMethodException e) {
            // TODO : Log Exception
            throw e;
        }

        try {
            _GetParameter = AudioEffect.class.getMethod("getParameter", int.class, byte[].class);
        } catch (NoSuchMethodException e) {
            // TODO : Log Exception
            throw e;
        }

        try {
            _SetParameter = AudioEffect.class.getMethod("setParameter", int.class, byte[].class);
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

        _ByteConverter = new ByteConverter();
    }

    public boolean getEnabled() {
        return _Effect.getEnabled();
    }

    public void setEnabled(boolean enabled) {
        _Effect.setEnabled(enabled);
    }

    public void setParameter(int parameter, boolean value)
            throws IllegalAccessException, InvocationTargetException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, short value)
            throws IllegalAccessException, InvocationTargetException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, short[] value)
            throws IllegalAccessException, InvocationTargetException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, int value)
            throws IllegalAccessException, InvocationTargetException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, int[] value)
            throws InvocationTargetException, IllegalAccessException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, float value)
            throws InvocationTargetException, IllegalAccessException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, float[] value)
            throws InvocationTargetException, IllegalAccessException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public void setParameter(int parameter, String value)
            throws InvocationTargetException, IllegalAccessException {
        byte[] valueBytes = _ByteConverter.convertToBytes(value);
        invokeSetParameter(parameter, valueBytes);
    }

    public AudioEffect.Descriptor getDescriptor() {
        return _Effect.getDescriptor();
    }

    private void invokeSetParameter(int parameter, byte[] value)
            throws IllegalAccessException, InvocationTargetException {
        try {
            _SetParameter.invoke(_Effect, parameter, value);
        } catch (IllegalAccessException e) {
            // TODO : Log Exception
            throw e;
        } catch (InvocationTargetException e) {
            // TODO : Log Exception
            throw e;
        }
    }
}
