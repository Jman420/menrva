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

import android.media.MediaPlayer;
import android.media.audiofx.AudioEffect;
import android.media.audiofx.AudioEffectInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.TextView;

import com.monkeystable.menrva.R;

import java.io.IOException;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.UUID;

public class DebugActivity extends AppCompatActivity {
    private final String TAB = "    ";

    private TextView _Console;

    private MediaPlayer _TestSong;
    private AudioEffect _AudioEffect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.debug_activity);

        _Console = findViewById(R.id.consoleOut);
        _Console.setMovementMethod(new ScrollingMovementMethod());
        _Console.setHorizontallyScrolling(true);
        writeToConsole("-----Console Output-----");

        String effectName = "MenrvaEngine";
        UUID effectTypeUUID = UUID.fromString(JniInterface.getMenrvaEffectTypeUUID());
        UUID engineUUID = UUID.fromString(JniInterface.getMenrvaEffectEngineUUID());
        writeToConsole("***Details from JNI Interface***");
        writeToConsole(TAB + "Effect Name : " + effectName);
        writeToConsole(TAB + "Effect Type UUID : " + effectTypeUUID);
        writeToConsole(TAB + "Engine UUID : " + engineUUID);

        AudioEffect.Descriptor[] effects = AudioEffect.queryEffects();
        int effectIndex = effects.length - 1;
        effectName = effects[effectIndex].name;
        effectTypeUUID = effects[effectIndex].type;
        engineUUID = effects[effectIndex].uuid;
        writeToConsole("***Details from Effects List***");
        writeToConsole(TAB + "Effect Name : " + effectName);
        writeToConsole(TAB + "Effect Type UUID : " + effectTypeUUID);
        writeToConsole(TAB + "Engine UUID : " + engineUUID);

        _TestSong = MediaPlayer.create(DebugActivity.this, R.raw.test_song);
        _AudioEffect = AudioEffectInterface.CreateAudioEffect(effectTypeUUID, engineUUID, 0, _TestSong.getAudioSessionId());

        AudioEffect.Descriptor menrvaDesc = _AudioEffect.getDescriptor();
        writeToConsole("***Instantiated Effect Details***");
        writeToConsole(TAB + "Effect Name : " + menrvaDesc.name);
        writeToConsole(TAB + "Implementor : " + menrvaDesc.implementor);
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
}
