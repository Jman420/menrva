package com.monkeystable.menrva.activities;

import android.media.audiofx.AudioEffect;
import android.media.audiofx.AudioEffectInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.monkeystable.menrva.R;

import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    public final static UUID EFFECT_TYPE_CUSTOM = UUID.fromString("f98765f4-c321-5de6-9a45-123459495ab2");
    public final static UUID EFFECT_JAMESDSP = UUID.fromString("f27317f4-c984-4de6-9a90-545759495bf2");

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

        //AudioEffect.Descriptor[] effects = AudioEffect.queryEffects();

        /**/
        TextView engineInfoText = findViewById(R.id.engineInfo);
        AudioEffect menrvaEffect = AudioEffectInterface.CreateAudioEffect(effectTypeUUID, engineUUID, 1, 0);
        AudioEffect.Descriptor menrvaDesc = menrvaEffect.getDescriptor();
        engineInfoText.setText("Effect Name : " + menrvaDesc.name + System.lineSeparator() +
                                "Implementor : " + menrvaDesc.implementor);
        /**/
    }
}
