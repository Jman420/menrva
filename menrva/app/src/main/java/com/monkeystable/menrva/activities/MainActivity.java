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

        String effectName = "MenrvaEngine";
        UUID effectTypeUUID = UUID.fromString(JniInterface.getMenrvaEffectTypeUUID());
        UUID engineUUID = UUID.fromString(JniInterface.getMenrvaEffectEngineUUID());
        TextView effectInfoJniText = findViewById(R.id.effectInfoJni);
        effectInfoJniText.setText("Details from JNI Interface" + System.lineSeparator() +
                                "Effect Name : " + effectName + System.lineSeparator() +
                                "Effect Type UUID : " + effectTypeUUID + System.lineSeparator() +
                                "Engine UUID : " + engineUUID);

        AudioEffect.Descriptor[] effects = AudioEffect.queryEffects();
        int effectIndex = effects.length - 1;
        effectName = effects[effectIndex].name;
        effectTypeUUID = effects[effectIndex].type;
        engineUUID = effects[effectIndex].uuid;
        TextView engineInfoSystemText = findViewById(R.id.effectInfoSystem);
        engineInfoSystemText.setText("Details from Effects List" + System.lineSeparator() +
                                "Effect Name : " + effectName + System.lineSeparator() +
                                "Effect Type UUID : " + effectTypeUUID + System.lineSeparator() +
                                "Engine UUID : " + engineUUID);

        TextView engineInfoText = findViewById(R.id.engineInfo);
        AudioEffect menrvaEffect = AudioEffectInterface.CreateAudioEffect(effectTypeUUID, engineUUID, 1, 0);
        AudioEffect.Descriptor menrvaDesc = menrvaEffect.getDescriptor();
        engineInfoText.setText("Effect Name : " + menrvaDesc.name + System.lineSeparator() +
                                "Implementor : " + menrvaDesc.implementor);
    }
}
