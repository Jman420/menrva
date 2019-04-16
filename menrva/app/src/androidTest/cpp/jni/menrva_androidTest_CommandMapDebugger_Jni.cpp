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
#include "../../../main/cpp/aosp/aosp_audio_effect_defs.h"
#include "../../../main/cpp/module_interface.h"
#include "../../../main/cpp/engine/command_map.h"
#include "../../../main/cpp/commands/engine_commands.h"

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_monkeystable_menrva_CommandMapDebugger_submitEngine_1GetVersion(JNIEnv *env, jobject instance, jbyteArray request_, jint requestSize) {
    effect_handle_t menrvaEffectHandle = nullptr;
    MenrvaModuleInterface::CreateModule(&MenrvaModuleInterface::EffectDescriptor.uuid, 0, 0, &menrvaEffectHandle);
    MenrvaModuleContext menrvaEngineContext = *(MenrvaModuleContext*)menrvaEffectHandle;

    uint32_t responseSize = 0;
    char* responseBytes = new char();
    jbyte *request = env->GetByteArrayElements(request_, NULL);
    MenrvaCommandMap::Process(menrvaEngineContext, EngineCommands::GET_VERSION, static_cast<uint32_t>(requestSize), request, &responseSize, responseBytes);

    jbyteArray responseArray = env->NewByteArray(responseSize);
    env->SetByteArrayRegion(responseArray, 0, responseSize, reinterpret_cast<const jbyte *>(responseBytes));

    env->ReleaseByteArrayElements(request_, request, 0);
    return responseArray;
}
