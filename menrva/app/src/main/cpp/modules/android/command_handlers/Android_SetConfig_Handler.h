/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2020 Justin Giannone (aka Jman420)
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

#ifndef MENRVA_ANDROID_SET_CONFIG_HANDLER_H
#define MENRVA_ANDROID_SET_CONFIG_HANDLER_H

#include "../../../abstracts/CommandHandlerBase.h"

class Android_SetConfig_Handler
        : public CommandHandlerBase {
public:
    Android_SetConfig_Handler(LoggerBase* logger);

    virtual bool DeserializeRequest(void *data, int length) override;
    virtual void Execute(ModuleContext& context) override;
    virtual uint32_t SerializeResponse(void* responseBuffer) override;

private:
    effect_config_t* _Config;

    void LogBufferConfig(buffer_config_t& bufferConfig);
};

#endif //MENRVA_ANDROID_SET_CONFIG_HANDLER_H
