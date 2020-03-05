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
#include "../ModuleStructures.h"
#include "../../tools/ServiceLocator.h"
#include "../../tools/LoggerBase.h"

struct AndroidModuleContext
        : public MenrvaModuleContext {
    effect_config_t config;
};

struct AndroidModuleInterface {
    __unused const effect_interface_s* itfe;

    AndroidModuleContext* AndroidContext;
};

class AndroidInterface {
public:
    static const effect_descriptor_t EffectDescriptor;
    static const effect_interface_s EngineInterface;
    static const char* EffectTypeUUID;
    static const char* EngineUUID;

    static int CreateModule(const effect_uuid_t* uuid, int32_t sessionId, int32_t ioId, effect_handle_t* pHandle);
    static int ReleaseModule(effect_handle_t moduleHandle);
    static int GetDescriptorFromUUID(const effect_uuid_t* uuid, effect_descriptor_t* pDescriptor);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor);

    static int Process(effect_handle_t handle, audio_buffer_t* inBuffer, audio_buffer_t* outBuffer);
    static int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;

    // Private Constructor to prevent instantiation of Static Class
    AndroidInterface() = default;
};

#endif //MENRVA_MODULE_INTERFACE_H
