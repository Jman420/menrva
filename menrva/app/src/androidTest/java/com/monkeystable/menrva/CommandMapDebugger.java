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
import com.monkeystable.menrva.commands.messages.Engine_GetVersion;

import org.junit.Test;
import org.junit.runner.RunWith;

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
        Engine_GetVersion.Engine_GetVersion_Request request = Engine_GetVersion.Engine_GetVersion_Request.newBuilder().build();
        Engine_GetVersion.Engine_GetVersion_Response response = Engine_GetVersion.Engine_GetVersion_Response.newBuilder().build();
        byte[] requestBytes = request.toByteArray();
        byte[] responseBytes = new byte[2];
        int responseLength = 0;
        submitEngine_GetVersion(requestBytes, requestBytes.length, responseBytes, responseLength);

        response = Engine_GetVersion.Engine_GetVersion_Response.parseFrom(responseBytes);
        int debug = 0;
    }

    public native void submitEngine_GetVersion(byte[] requestBytes, int requestLength, byte[] responseBytes, int responseLength);
}
