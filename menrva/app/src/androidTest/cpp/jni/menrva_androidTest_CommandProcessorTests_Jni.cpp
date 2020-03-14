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
#include <menrvaEngine/engine/CommandProcessor.h>
#include <menrvaEngine/commands/MenrvaCommands.h>
#include <menrvaEngine/host/CommandIdCalculator.h>
#include <menrvaEngine/commands/messages/Engine_GetVersion.pb.h>
#include "../../../main/cpp/hosts/android/aosp/aosp_audio_effect_defs.h"
#include "../../../main/cpp/hosts/android/AndroidInterface.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_CommandProcessorTests_submitCommand(JNIEnv* env, jobject instance, jint commandId, jbyteArray requestBytes_, jint requestLength, jbyteArray responseBuffer_) {
    effect_handle_t menrvaEffectHandle = nullptr;
    AndroidInterface::CreateModule(&AndroidInterface::EffectDescriptor.uuid, 0, 0, &menrvaEffectHandle);
    AndroidModuleInterface menrvaModuleInterface = *(AndroidModuleInterface*)menrvaEffectHandle;
    AndroidModuleContext menrvaEngineContext = *menrvaModuleInterface.AndroidContext;

    jbyte* requestBytes = env->GetByteArrayElements(requestBytes_, nullptr);
    jbyte* responseBuffer = env->GetByteArrayElements(responseBuffer_, nullptr);

    uint32_t responseLength = 0;
    menrvaModuleInterface.effectInterface->command(menrvaEffectHandle, static_cast<uint32_t>(commandId), static_cast<uint32_t>(requestLength), requestBytes, &responseLength, responseBuffer);

    env->ReleaseByteArrayElements(requestBytes_, requestBytes, 0);
    env->ReleaseByteArrayElements(responseBuffer_, responseBuffer, 0);

    return responseLength;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_monkeystable_menrva_CommandMapDebugger_submit_1Engine_1GetVersion(JNIEnv *env, jobject instance, jbyteArray requestBytes_, jint requestLength, jbyteArray responseBuffer_) {
    effect_handle_t menrvaEffectHandle = nullptr;
    AndroidInterface::CreateModule(&AndroidInterface::EffectDescriptor.uuid, 0, 0, &menrvaEffectHandle);
    ModuleContext menrvaEngineContext = *(ModuleContext*)menrvaEffectHandle;

    jbyte* requestBytes = env->GetByteArrayElements(requestBytes_, nullptr);
    jbyte* responseBuffer = env->GetByteArrayElements(responseBuffer_, nullptr);

    uint32_t responseLength = 0;
    uint32_t commandId = CommandIdCalculator::Calculate(MenrvaCommands::Engine_GetVersion);
    menrvaEngineContext.CommandProcessor->Process(menrvaEngineContext, commandId, static_cast<uint32_t>(requestLength), requestBytes, &responseLength, responseBuffer);

    env->ReleaseByteArrayElements(requestBytes_, requestBytes, 0);
    env->ReleaseByteArrayElements(responseBuffer_, responseBuffer, 0);

    return responseLength;
}
