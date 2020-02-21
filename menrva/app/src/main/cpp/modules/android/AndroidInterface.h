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

#ifndef MENRVA_ANDROID_INTERFACE_H
#define MENRVA_ANDROID_INTERFACE_H

#include "../ModuleStructures.h"
#include "aosp/aosp_audio_effect_defs.h"
#include "../../tools/ServiceLocator.h"
#include "../../abstracts/LoggerBase.h"
#include "../../engine/EffectsEngine.h"
#include "../../audio/AudioInputBuffer.h"
#include "../../audio/AudioOutputBuffer.h"

struct AndroidModuleContext : public ModuleContext {
    __unused const effect_interface_s* itfe;
    effect_config_t AndroidConfig;
};

class ModuleInterface {
public:
    static const effect_descriptor_t EffectDescriptor;
    static const effect_interface_s EngineInterface;
    static const char* EffectTypeUUID;
    static const char* EngineUUID;

    static int CreateModule(const effect_uuid_t* uuid, int32_t sessionId, int32_t ioId, effect_handle_t* handlePtr);
    static void InitModule(ModuleContext* context);
    static int ReleaseModule(effect_handle_t handle);
    static int GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* descriptorPtr);
    static int GetDescriptorFromModule(effect_handle_t handle, effect_descriptor_t* descriptorPtr);

    static int Process(effect_handle_t handle, audio_buffer_t* inBufferPtr, audio_buffer_t* outBufferPtr);
    static int Command(effect_handle_t handle, uint32_t cmdCode, uint32_t cmdSize, void* cmdDataPtr, uint32_t* replySize, void* replyDataPtr);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;

    // Private Constructor to prevent instantiation of Static Class
    ModuleInterface() = default;
};

#endif //MENRVA_ANDROID_INTERFACE_H
