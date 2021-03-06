/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
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

#ifndef MENRVA_COMMAND_HANDLER_BASE_H
#define MENRVA_COMMAND_HANDLER_BASE_H

#include "../log/LogProducer.h"
#include "../commands/CommandBase.h"
#include "../host/ModuleStructures.h"

class CommandHandlerBase
        : public LogProducer {
public:
    CommandHandlerBase(CommandBase* command, LogWriterBase* logger, std::string prettyFunction);
    virtual ~CommandHandlerBase();

    CommandBase* GetCommand();
    int32_t GetReturnValue();

    virtual bool DeserializeRequest(void* data, int length);
    virtual uint32_t SerializeResponse(void* responseBuffer);

    virtual void Execute(ModuleContext& context) = 0;

protected:
    CommandBase* _Command;
    int32_t _ReturnValue;
};

#endif //MENRVA_COMMAND_HANDLER_BASE_H
