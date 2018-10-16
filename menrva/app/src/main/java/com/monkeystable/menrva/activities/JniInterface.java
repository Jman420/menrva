package com.monkeystable.menrva.activities;

public class JniInterface {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("menrva-lib");
    }

    public static native String getMenrvaEffectTypeUUID();
    public static native String getMenrvaEffectEngineUUID();
}
