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

#include "kfr_interface.h"

KfrInterface::KfrInterface(LoggerBase* logger) : FftInterfaceBase(logger) {}

size_t KfrInterface::Initialize(size_t signalSize, size_t componentSize) {
    // TODO : Implement KFR Initialization Logic

    return FftInterfaceBase::Initialize(signalSize, componentSize);
}

void KfrInterface::SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) {
    // TODO : Implement KFR SignalToComponents Logic
}

void KfrInterface::ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) {
    // TODO : Implement KFR ComponentsToSignal Logic
}

sample* KfrInterface::Allocate(size_t size) {
    sample* buffer = (sample*)malloc(size);
    memset(buffer, 0, size);
    return buffer;
}

void KfrInterface::Deallocate(sample* data) {
    free(data);
}
