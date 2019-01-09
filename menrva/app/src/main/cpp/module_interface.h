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

#include <string>
#include "aosp/aosp_audio_effect_defs.h"
#include "tools/service_locator.h"
#include "abstracts/logger_base.h"
#include "engine/effects_engine.h"
#include "audio/audio_input_buffer.h"
#include "audio/audio_output_buffer.h"

enum MenrvaModuleStatus {
    MENRVA_MODULE_UNINITIALIZED,
    MENRVA_MODULE_INITIALIZING,
    MENRVA_MODULE_READY,
    MENRVA_MODULE_RELEASING,
};

// Expected structure passed as effect_handle_t; Represents an instance of a MenrvaModule
struct menrva_module_context {
    __unused const effect_interface_s* itfe;
    effect_config_t* config;

    MenrvaModuleStatus ModuleStatus;
    MenrvaEffectsEngine* EffectsEngine;
    AudioInputBuffer* InputBuffer;
    AudioOutputBuffer* OutputBuffer;
};

// Represents the public interface for interacting with the Menrva Audio Effects Module
class MenrvaModuleInterface {
public:
    static const effect_descriptor_t EffectDescriptor;
    static const effect_interface_s EngineInterface;
    static const char* EffectTypeUUID;
    static const char* EngineUUID;

    static int CreateModule(const effect_uuid_t* uuid, int32_t sessionId, int32_t ioId,
                            effect_handle_t* pHandle);
    static int InitModule(menrva_module_context* context);
    static int ReleaseModule(effect_handle_t moduleHandle);
    static int GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;

    // Private Constructor to prevent instantiation of Static Class
    MenrvaModuleInterface();
};

#endif //MENRVA_MODULE_INTERFACE_H
