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

#ifndef MENRVA_MODULE_STRUCTURES_H
#define MENRVA_MODULE_STRUCTURES_H

#include "../engine/EffectsEngine.h"
#include "../audio/AudioInputBuffer.h"
#include "../audio/AudioOutputBuffer.h"

class CommandProcessor;  // Forward Declaration to avoid circular reference : ../engine/CommandProcessor.h

enum ModuleStatus {
    UNINITIALIZED,
    INITIALIZING,
    READY,
    RELEASING,
};

struct ModuleContext {
    ModuleStatus ModuleStatus;
    MenrvaEffectsEngine* EffectsEngine;
    CommandProcessor* CommandProcessor;

    uint32_t ChannelLength;
    AudioInputBuffer* InputBuffer;
    AudioOutputBuffer* OutputBuffer;
};

#endif //MENRVA_MODULE_STRUCTURES_H
