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

#ifndef MENRVA_MENRVA_ENGINE_INTERFACE_H
#define MENRVA_MENRVA_ENGINE_INTERFACE_H

#include "../aosp/aosp_audio_effect_defs.h"

// Represents the public interface for interacting with the Menrva Audio Effects Engine
class MenrvaEngineInterface {
public:
    // Represents the Descriptor for the Menrva Effects Engine
    static const effect_descriptor_t effectDescriptor;

    static const char* effectTypeUUID;
    static const char* engineUUID;

    static int Process(effect_handle_t self, audio_buffer_t* in, audio_buffer_t* out);
    static int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData,
                       uint32_t* replySize, void* pReplyData);
    static int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t* pDescriptor);

private:
    // Private Constructor to prevent instantiation of Static Class
    MenrvaEngineInterface();
};

#endif //MENRVA_MENRVA_ENGINE_INTERFACE_H
