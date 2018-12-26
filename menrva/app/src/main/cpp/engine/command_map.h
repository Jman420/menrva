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

#ifndef MENRVA_MENRVA_COMMAND_MAP_H
#define MENRVA_MENRVA_COMMAND_MAP_H

#include <map>
#include "../aosp/aosp_audio_effect_defs.h"
#include "../module_interface.h"

// Type definitions for the FunctionMap
typedef int (*CommandFunc)(menrva_module_context*, uint32_t, void*, uint32_t*, void*);
typedef std::map<uint32_t, CommandFunc> function_map;

// union to hold command values
using value_t = union {
    int32_t integer;
    float decimal;
};

// Represents the Commands supported by the Menrva Audio Effects Module & Engine
class MenrvaCommandMap {
public:
    static int Command(menrva_module_context* context, uint32_t cmdCode, uint32_t cmdSize,
                       void* pCmdData, uint32_t* replySize, void* pReplyData);

private:
    // Private Constructor to prevent instantiation of Static Class
    MenrvaCommandMap();

    static int InitModule(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                          uint32_t* replySize, void* pReplyData);
    static int SetConfig(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                         uint32_t* replySize, void* pReplyData);
    static int ResetEngine(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                           uint32_t* replySize, void* pReplyData);
    static int EnableEngine(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                            uint32_t* replySize, void* pReplyData);
    static int DisableEngine(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                             uint32_t* replySize, void* pReplyData);
    static int SetParam(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                        uint32_t* replySize, void* pReplyData);
    static int GetParam(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                        uint32_t* replySize, void* pReplyData);
    static int GetConfig(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                         uint32_t* replySize, void* pReplyData);

    // Represents the Command to Function Map
    static function_map CommandMap;
};

#endif //MENRVA_MENRVA_COMMAND_MAP_H
