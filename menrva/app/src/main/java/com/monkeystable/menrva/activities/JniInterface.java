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

package com.monkeystable.menrva.activities;

public class JniInterface {
    static {
        System.loadLibrary("MenrvaEngine");
    }

    public static native String getMenrvaEffectName();
    public static native String getMenrvaEffectTypeUUID();
    public static native String getMenrvaEffectEngineUUID();

    static String[] getLogLevelsForUI() {
        int logLevelsLength = getLogLevelsLength();
        String[] result = new String[logLevelsLength];

        result[0] = "Fatal";
        result[1] = "Error";
        result[2] = "Warn";
        result[3] = "Info";
        result[4] = "Debug";
        result[5] = "Verbose";

        return result;
    }

    static int getAppLogLevelForUI() {
        int appLogLevel = getAppLogLevel();
        int maxLogLevelValue = getMaxLogLevel();
        return maxLogLevelValue - appLogLevel;
    }

    private static native int getLogLevelsLength();
    private static native int getMaxLogLevel();
    private static native int getAppLogLevel();
}
