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

#include "CommandBase.h"

CommandBase::CommandBase(int commandId, MessageLite* request, MessageLite* response) {
    _CommandId = commandId;
    _Request = request;
    _Response = response;
}

CommandBase::~CommandBase() {
    delete _Request;
    delete _Response;
}

int CommandBase::GetCommandId() {
    return _CommandId;
}

MessageLite* CommandBase::GetRequest() {
    return _Request;
}

MessageLite* CommandBase::GetResponse() {
    return _Response;
}

uint32_t CommandBase::SerializeResponse(void* responseBuffer) {
    MessageLite& response = *_Response;
    size_t responseSize = response.ByteSizeLong();
    *(char*)responseBuffer = *new char[responseSize];

    response.SerializeToArray(responseBuffer, responseSize);
    return responseSize;
}
