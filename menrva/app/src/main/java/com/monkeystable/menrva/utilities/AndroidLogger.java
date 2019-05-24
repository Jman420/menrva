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

package com.monkeystable.menrva.utilities;

import android.util.Log;

import com.monkeystable.menrva.abstracts.MenrvaLogger;

public class AndroidLogger extends MenrvaLogger {
    private static final String APP_NAME = "Menrva";
    private static final String LOG_ELEMENT_DELIMITER = ".";
    private static final String FUNCTION_SUFFIX = "()";

    private static boolean _Initialized = false;

    public AndroidLogger() {
        Initialize();
    }

    @Override
    public void writeLog(String message, String senderClass, String senderFunction, LogLevel logLevel) {
        if (logLevel.ordinal() > _LogLevel.ordinal()) {
            return;
        }

        // Format Log Tag
        String prefix = APP_NAME;
        if (senderClass != null && !senderClass.isEmpty()) {
            prefix += LOG_ELEMENT_DELIMITER + senderClass;
        }
        if (senderFunction != null && !senderFunction.isEmpty()) {
            prefix += LOG_ELEMENT_DELIMITER + senderFunction + FUNCTION_SUFFIX;
        }

        Log.println(logLevel.getLogLevelId(), prefix, message);
    }

    private void Initialize() {
        if (_Initialized) {
            return;
        }

        // BEGIN DEBUG
        setLogLevel(LogLevel.DEBUG);
        // END DEBUG

        // TODO : Get LogLevel & Whitelist Settings from Persisted Settings

        _Initialized = true;
    }
}
