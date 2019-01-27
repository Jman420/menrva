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

#ifndef MENRVA_KFR_INTERFACE_H
#define MENRVA_KFR_INTERFACE_H

#include <kfr/dft.hpp>
#include "../abstracts/fft_interface_base.h"
#include "../abstracts/logging_base.h"

using namespace kfr;

class KfrInterface : public FftInterfaceBase,
                     public LoggingBase {
public:
    explicit KfrInterface(LoggerBase* logger);
    ~KfrInterface();

    size_t Initialize(size_t signalSize, size_t componentSize) override;
    void SignalToComponents(AudioBuffer* signal, AudioComponentsBuffer* components) override;
    void ComponentsToSignal(AudioComponentsBuffer* components, AudioBuffer* signal) override;

private:
    dft_plan_real_ptr<sample> _Plan;
    complex<sample>* _ComponentsBuffer;
    u8* _TempBuffer;
};


#endif //MENRVA_KFR_INTERFACE_H
