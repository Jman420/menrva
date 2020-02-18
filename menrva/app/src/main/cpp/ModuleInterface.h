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

#ifndef MENRVA_MODULE_INTERFACE_H
#define MENRVA_MODULE_INTERFACE_H

#include "aosp/aosp_audio_effect_defs.h"
#include "tools/ServiceLocator.h"
#include "abstracts/LoggerBase.h"
#include "engine/EffectsEngine.h"
#include "audio/AudioInputBuffer.h"
#include "audio/AudioOutputBuffer.h"

enum ModuleStatus {
    UNINITIALIZED,
    INITIALIZING,
    READY,
    RELEASING,
};

// Expected structure passed as effect_handle_t; Represents an instance of a MenrvaModule
struct MenrvaModuleContext {
    __unused const effect_interface_s* itfe;
    effect_config_t config;

    ModuleStatus ModuleStatus;
    EffectsEngine* EffectsEngine;

    uint32_t ChannelLength;
    AudioInputBuffer* InputBuffer;
    AudioOutputBuffer* OutputBuffer;
};

// Represents the public interface for interacting with the Audio Effects Module
class ModuleInterface {
public:
    static const effect_descriptor_t EffectDescriptor;
    static const effect_interface_s EngineInterface;
    static const char* EffectTypeUUID;
    static const char* EngineUUID;

    static int CreateModule(const effect_uuid_t* uuid, int32_t sessionId, int32_t ioId, effect_handle_t* pHandle);
    static void InitModule(MenrvaModuleContext &context);
    static int ReleaseModule(effect_handle_t moduleHandle);
    static int GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;

    // Private Constructor to prevent instantiation of Static Class
    ModuleInterface() = default;
};

#endif //MENRVA_MODULE_INTERFACE_H
