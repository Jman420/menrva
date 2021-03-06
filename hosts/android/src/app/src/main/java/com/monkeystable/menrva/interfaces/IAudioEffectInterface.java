/*
 * Menrva - Over-Engineered Tunable Android Audio Effects
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

package com.monkeystable.menrva.interfaces;

import android.media.audiofx.AudioEffect;

import com.google.protobuf.InvalidProtocolBufferException;
import com.monkeystable.menrva.abstracts.CommandBase;

import java.lang.reflect.InvocationTargetException;

public interface IAudioEffectInterface {
    AudioEffect.Descriptor getDescriptor();

    boolean getEnabled();
    void setEnabled(boolean enabled);

    void sendCommand(CommandBase message)
            throws InvocationTargetException, IllegalAccessException, InvalidProtocolBufferException;
}
