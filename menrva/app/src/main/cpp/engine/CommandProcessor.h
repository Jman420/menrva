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

#ifndef MENRVA_COMMAND_MAP_H
#define MENRVA_COMMAND_MAP_H

#include "../aosp/aosp_audio_effect_defs.h"
#include "../ModuleInterface.h"
#include "CommandHandlerMap.h"

class CommandProcessor {
public:
    static int Process(MenrvaModuleContext& context, uint32_t cmdCode, uint32_t cmdSize,
                       void* pCmdData, uint32_t* replySize, void* pReplyData);

private:
    static const std::string LOG_SENDER;

    static ServiceLocator* _ServiceLocator;
    static LoggerBase* _Logger;
    static CommandHandlerMap* _HandlerMap;

    // Private Constructor to prevent instantiation of Static Class
    CommandProcessor() = default;
};

#endif //MENRVA_COMMAND_MAP_H
