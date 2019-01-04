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

#ifndef MENRVA_ENGINE_INTERFACE_H
#define MENRVA_ENGINE_INTERFACE_H

#include "../aosp/aosp_audio_effect_defs.h"
#include "../abstracts/logger_base.h"
#include "../tools/service_locator.h"

class MenrvaEngineInterface {
public:
    static int Process(effect_handle_t handle, audio_buffer_t* in, audio_buffer_t* out);
    static int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData,
                       uint32_t* replySize, void* pReplyData);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;

    // Private Constructor to prevent instantiation of Static Class
    MenrvaEngineInterface();
};

#endif //MENRVA_ENGINE_INTERFACE_H
