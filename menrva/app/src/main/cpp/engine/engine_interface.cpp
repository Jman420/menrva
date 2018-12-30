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

#include <cerrno>
#include "engine_interface.h"
#include "command_map.h"
#include "../module_interface.h"
#include "../audio/sample.h"

const effect_descriptor_t MenrvaEngineInterface::EffectDescriptor = {
    // UUID of to the OpenSL ES interface implemented by this effect (EFFECT_TYPE_NULL)
    .type = { 0xec7178ec, 0xe5e1, 0x4432, 0xa3f4, { 0x46, 0x57, 0xe6, 0x79, 0x52, 0x10 } },
    // UUID for this particular implementation (http://www.itu.int/ITU-T/asn1/uuid.html)
    .uuid = { 0xa91fdfe4, 0xd09e, 0x11e8, 0xa8d5, { 0xf2, 0x80, 0x1f, 0x1b, 0x9f, 0xd1 } },
    // Version of the effect control API implemented
    .apiVersion = EFFECT_CONTROL_API_VERSION,
    // effect engine capabilities/requirements flags (see below)
    .flags = EFFECT_FLAG_TYPE_INSERT | EFFECT_FLAG_INSERT_FIRST,
    // CPU load indication
    .cpuLoad = 10,
    // Data Memory usage
    .memoryUsage = 1,
    // human readable effect name
    .name = "MenrvaEngine",
    // human readable effect implementor name
    .implementor = "Jman420"
};

const char* MenrvaEngineInterface::EffectTypeUUID = "ec7178ec-e5e1-4432-a3f4-4657e6795210";
const char* MenrvaEngineInterface::EngineUUID = "a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1";

int MenrvaEngineInterface::Process(effect_handle_t handle, audio_buffer_t* in, audio_buffer_t* out)
{
    struct menrva_module_context *context = (menrva_module_context*)handle;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING) {
        return -ENODATA;
    }
    if (context->ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY) {
        return 0;
    }

    context->InputBuffer->SetData((sample*)in->f32, in->frameCount, false);
    context->OutputBuffer->SetData((sample*)out->f32, out->frameCount, false);
    int result = context->EffectsEngine->Process(context->InputBuffer, context->OutputBuffer);

    return result;
}

int MenrvaEngineInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize,
                                   void* pCmdData, uint32_t* replySize, void* pReplyData)
{
    struct menrva_module_context *context = (menrva_module_context*)self;

    if (context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING ||
        context->ModuleStatus == MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {

        return -EINVAL;
    }

    int result = MenrvaCommandMap::Command(context, cmdCode, cmdSize, pCmdData, replySize,
                                           pReplyData);
    return result;
}

int MenrvaEngineInterface::GetDescriptorFromModule(effect_handle_t self,
                                                   effect_descriptor_t* pDescriptor)
{
    menrva_module_context* module = (menrva_module_context*)self;
    if (module == NULL || pDescriptor == NULL) {
        return -EINVAL;
    }

    *pDescriptor = EffectDescriptor;
    return 0;
}

MenrvaEngineInterface::MenrvaEngineInterface() {}
