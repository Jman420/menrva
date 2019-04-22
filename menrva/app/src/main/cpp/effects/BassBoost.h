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

#ifndef MENRVA_BASS_BOOST_H
#define MENRVA_BASS_BOOST_H

#include "../abstracts/SingleChannelEffectBase.h"
#include "../abstracts/LoggingBase.h"
#include "../ir/FirGenerator.h"
#include "../convolver/Convolver.h"

class BassBoost : public SingleChannelEffectBase,
                  public LoggingBase {
public:
    BassBoost(LoggerBase* logger, FirGenerator* firGenerator, Convolver* convolver);
    ~BassBoost() override;

    void Process(AudioBuffer& input, AudioBuffer& output) override;
    void ResetBuffers(sample sampleRate, size_t audioFrameLength) override;
    void ConfigureSetting(char* settingName, void* value) override;

private:
    static const std::string EFFECT_NAME;

    FirGenerator* _FirGenerator;
    Convolver* _Convolver;
};

#endif //MENRVA_BASS_BOOST_H