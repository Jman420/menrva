// Author : Jman420

#include "com_monkeystable_menrva_activities_JniInterface.h"

#include "../engine_interface.h"

JNIEXPORT jstring JNICALL Java_com_monkeystable_menrva_activities_JniInterface_getMenrvaEffectTypeUUID
        (JNIEnv* env, jclass caller __unused) {
    return env->NewStringUTF(MenrvaEngineInterface::effectTypeUUID);
}

JNIEXPORT jstring JNICALL Java_com_monkeystable_menrva_activities_JniInterface_getMenrvaEffectEngineUUID
        (JNIEnv* env, jclass caller __unused) {
    return env->NewStringUTF(MenrvaEngineInterface::engineUUID);
}
