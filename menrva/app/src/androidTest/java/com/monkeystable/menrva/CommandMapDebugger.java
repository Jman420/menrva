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

package com.monkeystable.menrva;

import android.support.test.runner.AndroidJUnit4;

import com.google.protobuf.InvalidProtocolBufferException;
import com.monkeystable.menrva.commands.Engine_GetVersion_Command;
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
public class CommandMapDebugger {
    static {
        System.loadLibrary("MenrvaEngineTest");
    }

    @Test
    public void debug1Engine_GetVersion()
            throws InvalidProtocolBufferException {
        Engine_GetVersion_Command command = new Engine_GetVersion_Command();
        byte[] requestBytes = command.getRequest().toByteArray();
        byte[] responseBuffer = new byte[AudioEffectInterface.MAX_RESPONSE_SIZE];

        int responseLength = submitEngine_GetVersion(requestBytes, requestBytes.length, responseBuffer);
        byte[] responseBytes = trimResponseBuffer(responseBuffer, responseLength);
        Engine_GetVersion.Engine_GetVersion_Response response = command.getResponse().getParserForType().parseFrom(responseBytes);

        int realMajor = EngineInterface.GetMajorVersion();
        int realMinor = EngineInterface.GetMinorVersion();
        int realPatch = EngineInterface.GetPatchVersion();

        Assert.assertEquals(realMajor, response.getMajor());
        Assert.assertEquals(realMinor, response.getMinor());
        Assert.assertEquals(realPatch, response.getPatch());
    }

    public native int submitEngine_GetVersion(byte[] requestBytes, int requestLength, byte[] responseBuffer);

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
