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

#ifndef MENRVA_EQUALIZER_H
#define MENRVA_EQUALIZER_H

#include "../abstracts/effect_base.h"
#include "../abstracts/logging_base.h"

class Equalizer : public EffectBase, LoggingBase {
public:
    Equalizer(LoggerBase* logger);

    void Process(AudioBuffer* input, AudioBuffer* output);
    void ResetConfig(effect_config_t* bufferConfig);
    void ConfigureSetting(char* settingName, void* value);

private:
    static const std::string EFFECT_NAME;
};

#endif //MENRVA_EQUALIZER_H
