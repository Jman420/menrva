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

#ifndef MENRVA_COMMANDBASE_H
#define MENRVA_COMMANDBASE_H

#include <google/protobuf/message_lite.h>

using google::protobuf::MessageLite;

class CommandBase {
public:
    CommandBase(int commandId, MessageLite* request, MessageLite* response);
    virtual ~CommandBase();

    int GetCommandId();
    virtual MessageLite* GetRequest();
    virtual MessageLite* GetResponse();

    uint32_t SerializeResponse(void* responseBuffer);

protected:
    int _CommandId;
    MessageLite* _Request;
    MessageLite* _Response;
};

#endif //MENRVA_COMMANDBASE_H
