package com.monkeystable.menrva.activities;

import android.media.audiofx.AudioEffect;
import android.media.audiofx.AudioEffectInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.monkeystable.menrva.R;

import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.sample_text);
        UUID menrvaEffectTypeUUID = UUID.fromString(JniInterface.getMenrvaEffectTypeUUID());
        UUID menrvaEngineUUID = UUID.fromString(JniInterface.getMenrvaEffectEngineUUID());
        tv.setText("Effect Type UUID : " + menrvaEffectTypeUUID + System.lineSeparator() +
                   "Engine UUID : " + menrvaEngineUUID);

        /*AudioEffect menrvaEffect = AudioEffectInterface.
                CreateAudioEffect(menrvaEffectTypeUUID, menrvaEngineUUID, 1, 0);

        AudioEffect.Descriptor menrvaDesc = menrvaEffect.getDescriptor();
        tv.setText("Effect Name : " + menrvaDesc.name + System.lineSeparator() +
                   "Implementor : " + menrvaDesc.implementor);*/
    }
}
