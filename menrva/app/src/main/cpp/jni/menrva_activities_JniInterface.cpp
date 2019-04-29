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

#include <jni.h>
#include "../ModuleInterface.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_monkeystable_menrva_activities_JniInterface_getMenrvaEffectName(JNIEnv *env, jclass type) {
    return env->NewStringUTF(MenrvaModuleInterface::EffectDescriptor.name);
}

extern "C"
JNIEXPORT jstring
JNICALL Java_com_monkeystable_menrva_activities_JniInterface_getMenrvaEffectTypeUUID(JNIEnv* env, jclass caller __unused) {
    return env->NewStringUTF(MenrvaModuleInterface::EffectTypeUUID);
}

extern "C"
JNIEXPORT jstring
JNICALL Java_com_monkeystable_menrva_activities_JniInterface_getMenrvaEffectEngineUUID(JNIEnv* env, jclass caller __unused) {
    return env->NewStringUTF(MenrvaModuleInterface::EngineUUID);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_activities_JniInterface_getLogLevelsLength(JNIEnv *env, jclass type) {
    auto result = LoggerBase::LOG_LEVEL_MAP.size();
    return static_cast<jint>(result);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_activities_JniInterface_getMaxLogLevel(JNIEnv* env, jclass type) {
    return LoggerBase::MAX_LOG_LEVEL_VALUE;
}