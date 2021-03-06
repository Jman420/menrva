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

#include "CommandHandlerBase.h"

using google::protobuf::MessageLite;

CommandHandlerBase::CommandHandlerBase(CommandBase* command, LogWriterBase* logger, std::string prettyFunction)
        : LogProducer(logger, prettyFunction) {
    _Command = command;
    _ReturnValue = 0;
}

CommandHandlerBase::~CommandHandlerBase() {
    delete _Command;
}

CommandBase *CommandHandlerBase::GetCommand() {
    return _Command;
}

int32_t CommandHandlerBase::GetReturnValue() {
    return _ReturnValue;
}

bool CommandHandlerBase::DeserializeRequest(void* data, int length) {
    if (length < 1) {
        return false;
    }

    MessageLite& request = *_Command->GetRequest();
    request.ParseFromArray(data, length);
    return true;
}

uint32_t CommandHandlerBase::SerializeResponse(void* responseBuffer) {
    return _Command->SerializeResponse(responseBuffer);
}
