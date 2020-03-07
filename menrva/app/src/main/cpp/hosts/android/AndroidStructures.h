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

#ifndef MENRVA_ANDROID_STRUCTURES_H
#define MENRVA_ANDROID_STRUCTURES_H

#include "aosp/aosp_audio_effect_defs.h"
#include "../ModuleStructures.h"

struct AndroidModuleContext
        : public ModuleContext {
    effect_config_t config;
};

struct AndroidModuleInterface {
    // The first member of this structure must be the effect_interface_s pointer in order to conform with the effect_handle_t structure from aosp/aosp_audio_effect_defs.h
    __unused const effect_interface_s* effectInterface;

    AndroidModuleContext* AndroidContext;
};

#endif //MENRVA_ANDROID_STRUCTURES_H
