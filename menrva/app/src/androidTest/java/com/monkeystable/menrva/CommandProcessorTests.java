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

package com.monkeystable.menrva;

import android.support.test.runner.AndroidJUnit4;

import com.google.protobuf.InvalidProtocolBufferException;
import com.monkeystable.menrva.commands.Engine_GetLogLevel_Command;
import com.monkeystable.menrva.commands.Engine_GetVersion_Command;
import com.monkeystable.menrva.commands.MenrvaCommands;
import com.monkeystable.menrva.commands.messages.Engine_GetLogLevel;
import com.monkeystable.menrva.commands.messages.Engine_GetVersion;
import com.monkeystable.menrva.utilities.AudioEffectInterface;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Arrays;
import java.util.Locale;

/* Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class CommandProcessorTests {
    static {
        System.loadLibrary("MenrvaEngineTest");
    }

    @Test
    public void debug1Module_Init() {
        final int COMMAND_ID = 0;
        byte[] requestBytes = new byte[0];
        byte[] responseBuffer = getResponseBuffer();

        int responseLength = submitCommand(COMMAND_ID, requestBytes, requestBytes.length, responseBuffer);

        Assert.assertEquals(Integer.BYTES, responseLength);
    }

    @Test
    public void debug2Engine_GetVersion()
            throws InvalidProtocolBufferException {
        Engine_GetVersion_Command command = new Engine_GetVersion_Command();
        byte[] requestBytes = command.getRequest().toByteArray();
        byte[] responseBuffer = getResponseBuffer();

        int responseLength = submitCommand(MenrvaCommands.Engine_GetVersion.getCommandId(), requestBytes, requestBytes.length, responseBuffer);
        byte[] responseBytes = trimResponseBuffer(responseBuffer, responseLength);
        Engine_GetVersion.Engine_GetVersion_Response response = command.getResponse().getParserForType().parseFrom(responseBytes);

        int realMajor = EngineInterface.GetMajorVersion();
        int realMinor = EngineInterface.GetMinorVersion();
        int realPatch = EngineInterface.GetPatchVersion();

        Assert.assertEquals(realMajor, response.getMajor());
        Assert.assertEquals(realMinor, response.getMinor());
        Assert.assertEquals(realPatch, response.getPatch());
    }

    @Test
    public void debug3Engine_GetLogLevel()
            throws InvalidProtocolBufferException {
        Engine_GetLogLevel_Command command = new Engine_GetLogLevel_Command();
        byte[] requestBytes = command.getRequest().toByteArray();
        byte[] responseBuffer = getResponseBuffer();

        int responseLength = submitCommand(MenrvaCommands.Engine_GetLogLevel.getCommandId(), requestBytes, requestBytes.length, responseBuffer);
        byte[] responseBytes = trimResponseBuffer(responseBuffer, responseLength);
        Engine_GetLogLevel.Engine_GetLogLevel_Response response = command.getResponse().getParserForType().parseFrom(responseBytes);

        int realLogLevel = EngineInterface.GetLogLevel();

        Assert.assertEquals(realLogLevel, response.getLogLevel());
    }

    public native int submitCommand(int commandId, byte[] requestBytes, int requestLength, byte[] responseBuffer);

    private byte[] getResponseBuffer() {
        return new byte[AudioEffectInterface.MAX_RESPONSE_SIZE];
    }

    private byte[] trimResponseBuffer(byte[] buffer, int responseLength)
            throws InvalidProtocolBufferException {
        if (responseLength > buffer.length) {
            String exceptionMsg = String.format(Locale.US,"Response Buffer Overflow.  Response Length %d exceeds Max Length %d.",
                    responseLength, buffer.length);
            throw new InvalidProtocolBufferException(exceptionMsg);
        }

        return Arrays.copyOfRange(buffer, 0, responseLength);
    }
}
