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

#ifndef MENRVA_KISSFFT_INTERFACE_H
#define MENRVA_KISSFFT_INTERFACE_H

#include <map>
#include "FftInterfaceBase.h"
#include "../log/LoggingBase.h"
#include "KissFftFunctions.h"

typedef std::map<std::string, kissfft_plan_pair> KissFftPlanCache;

class KissFftInterface : public FftInterfaceBase,
                         public LoggingBase {
public:
    explicit KissFftInterface(LoggerBase* logger);
    ~KissFftInterface() override;

    size_t Initialize(size_t signalSize, size_t componentSize) override;
    void SignalToComponents(AudioBuffer& signal, AudioComponentsBuffer& components) override;
    void ComponentsToSignal(AudioComponentsBuffer& components, AudioBuffer& signal) override;

private:
    static KissFftPlanCache* _PlansCache;

    bool _Initialized;
    kissfft_plan_pair _Plans;
    kiss_fft_cpx* _ComponentsBuffer;

    void Dispose();
};


#endif //MENRVA_KISSFFT_INTERFACE_H
