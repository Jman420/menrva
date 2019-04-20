/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2018, 2019 Justin Giannone (aka Jman420)
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
#include "../../../main/cpp/config.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_EngineInterface_GetMajorVersion(JNIEnv *env, jclass type) {
    return MENRVA_ENGINE_MAJOR;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_EngineInterface_GetMinorVersion(JNIEnv *env, jclass type) {
    return MENRVA_ENGINE_MINOR;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_EngineInterface_GetPatchVersion(JNIEnv *env, jclass type) {
    return MENRVA_ENGINE_PATCH;
}
