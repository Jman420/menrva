package com.monkeystable.menrva.activities;

public class JniInterface {

    static {
        System.loadLibrary("MenrvaEngine");
    }

    public static native String getMenrvaEffectTypeUUID();
    public static native String getMenrvaEffectEngineUUID();
}
