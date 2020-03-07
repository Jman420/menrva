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

#ifndef MENRVA_MASTER_LIMITER_H
#define MENRVA_MASTER_LIMITER_H

#include "MultiChannelEffectBase.h"
#include "../log/LogProducer.h"

class MasterLimiter
        : public MultiChannelEffectBase,
          public LogProducer {
public:
    explicit MasterLimiter(LogWriterBase* logger);

    void Process(AudioBuffer* inputBuffers, AudioBuffer* outputBuffers, uint32_t channelLength) override;
    void ConfigureSetting(char* settingName, void* value) override;

private:
    static const std::string EFFECT_NAME;
};

#endif //MENRVA_MASTER_LIMITER_H
