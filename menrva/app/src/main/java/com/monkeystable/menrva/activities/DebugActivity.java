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

package com.monkeystable.menrva.activities;

import android.content.Context;
import android.media.MediaPlayer;
import android.media.audiofx.AudioEffect;

import com.google.protobuf.InvalidProtocolBufferException;
import com.monkeystable.menrva.commands.Engine_GetVersion_Command;
import com.monkeystable.menrva.commands.messages.Engine_GetVersion.Engine_GetVersion_Response;
import com.monkeystable.menrva.utilities.AudioEffectInterface;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.SparseArray;
import android.view.View;
import android.widget.TextView;

import com.monkeystable.menrva.R;
import com.xw.repo.BubbleSeekBar;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.lang.reflect.InvocationTargetException;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.UUID;

public class DebugActivity extends AppCompatActivity {
    private final String TAB = "    ";

    private TextView _Console;
    private BubbleSeekBar _LogLevelSlider;

    private MediaPlayer _TestSong;
    private AudioEffectInterface _AudioEffect;

    @Override
    protected void onCreate(Bundle savedInstanceState)
            throws RuntimeException {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.debug_activity);

        _TestSong = MediaPlayer.create(DebugActivity.this, R.raw.test_song);

        UUID effectTypeUUID = UUID.fromString(JniInterface.getMenrvaEffectTypeUUID());
        UUID engineUUID = UUID.fromString(JniInterface.getMenrvaEffectEngineUUID());
        try {
            _AudioEffect = new AudioEffectInterface(effectTypeUUID, engineUUID, 0, _TestSong.getAudioSessionId());
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        Engine_GetVersion_Command getEngineVersionCmd = new Engine_GetVersion_Command();
        try {
            _AudioEffect.sendCommand(getEngineVersionCmd);
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        Engine_GetVersion_Response engineVersionResponse = getEngineVersionCmd.getResponse();
        String engineVersion = engineVersionResponse.getMajor() + "." + engineVersionResponse.getMinor() + "." + engineVersionResponse.getPatch();

        _LogLevelSlider = findViewById(R.id.logLevelSlider);
        final String[] logLevels = JniInterface.getLogLevelsForUI();
        _LogLevelSlider.setCustomSectionTextArray(new BubbleSeekBar.CustomSectionTextArray() {
            @NonNull
            @Override
            public SparseArray<String> onCustomize(int sectionCount, @NonNull SparseArray<String> array) {
                array.clear();

                for (int logLevelCounter = 0; logLevelCounter < logLevels.length; logLevelCounter++) {
                    array.put(logLevelCounter, logLevels[logLevelCounter]);
                }

                return array;
            }
        });
        _LogLevelSlider.setOnProgressChangedListener(new BubbleSeekBar.OnProgressChangedListener() {
            @Override
            public void onProgressChanged(BubbleSeekBar bubbleSeekBar, int progress, float progressFloat, boolean fromUser) {
                if (!fromUser) {
                    return;
                }

                // TODO : Send Command to Set Engine Log Level
            }

            @Override
            public void getProgressOnActionUp(BubbleSeekBar bubbleSeekBar, int progress, float progressFloat) { }

            @Override
            public void getProgressOnFinally(BubbleSeekBar bubbleSeekBar, int progress, float progressFloat, boolean fromUser) { }
        });
        int appLogLevel = JniInterface.getAppLogLevelForUI();
        _LogLevelSlider.setProgress(appLogLevel);

        _Console = findViewById(R.id.consoleOut);
        _Console.setMovementMethod(new ScrollingMovementMethod());
        _Console.setHorizontallyScrolling(true);
        writeToConsole("-----Console Output-----");

        writeToConsole("***Menrva Engine Details***");
        writeToConsole(TAB + "Effect Name : " + JniInterface.getMenrvaEffectName());
        writeToConsole(TAB + "Effect Type UUID : " + effectTypeUUID);
        writeToConsole(TAB + "Engine UUID : " + engineUUID);
        writeToConsole(TAB + "Engine version : " + engineVersion);

        AudioEffect.Descriptor[] effects = AudioEffect.queryEffects();
        writeToConsole("***Effects List***");
        for (AudioEffect.Descriptor audioEffect : effects) {
            writeToConsole(TAB + "Effect Name : " + audioEffect.name);
            writeToConsole(TAB + "Effect Type UUID : " + audioEffect.type);
            writeToConsole(TAB + "Engine UUID : " + audioEffect.uuid);
            writeToConsole("--------------------");
        }

        writeToConsole("***Instantiated Effect Details***");
        AudioEffect.Descriptor effectDesc = _AudioEffect.getDescriptor();
        writeToConsole(TAB + "Effect Name : " + effectDesc.name);
        writeToConsole(TAB + "Implementor : " + effectDesc.implementor);
        writeToConsole(TAB + "Audio Session Id : " + _TestSong.getAudioSessionId());
        writeToConsole(TAB + "Effect Enabled : " + _AudioEffect.getEnabled());
    }

    private void writeToConsole(String message) {
        final String CONSOLE_DELIMITER = " | ";
        final String TIMESTAMP_PATTERN = "HH:mm:ss.SSS";
        final SimpleDateFormat TIMESTAMP_FORMAT = new SimpleDateFormat(TIMESTAMP_PATTERN, Locale.getDefault());

        Date date = new Date();
        Timestamp timestamp = new Timestamp(date.getTime());
        _Console.append(TIMESTAMP_FORMAT.format(timestamp) + CONSOLE_DELIMITER + message + System.lineSeparator());
    }

    public void toggleSongButton_Click(View view) throws IOException {
        writeToConsole("Toggling Song...");
        if (_TestSong.isPlaying()) {
            _TestSong.stop();
            _TestSong.prepare();
            writeToConsole(TAB + "Stopped Song on Session ID : " + _TestSong.getAudioSessionId());
        }
        else {
            _TestSong.start();
            writeToConsole(TAB + "Started Song on Session ID : " + _TestSong.getAudioSessionId());
        }
    }

    public void toggleAudioEffectButton_Click(View view) {
        writeToConsole("Toggling Audio Effect...");
        if (_AudioEffect.getEnabled()) {
            _AudioEffect.setEnabled(false);
            writeToConsole(TAB + "Disabled Audio Effect!");
        }
        else {
            _AudioEffect.setEnabled(true);
            writeToConsole(TAB + "Enabled Audio Effect!");
        }
    }

    public void dumpConsoleButton_Click(View view) throws IOException {
        final String CONSOLE_DUMP_FILE_NAME = "consoleDump.txt";

        FileOutputStream outputStream = openFileOutput(CONSOLE_DUMP_FILE_NAME, Context.MODE_PRIVATE);
        OutputStreamWriter streamWriter = new OutputStreamWriter(outputStream);

        streamWriter.write(_Console.getText().toString());
        streamWriter.close();
    }
}
