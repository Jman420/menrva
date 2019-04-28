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

#include "Module_SetConfig_Handler.h"

Module_SetConfig_Handler::Module_SetConfig_Handler(LoggerBase* logger)
        : TypedCommandHandlerBase(new Android_SystemCommand_Command(), logger, __PRETTY_FUNCTION__) {
    _Config = nullptr;
}

bool Module_SetConfig_Handler::DeserializeRequest(void* data, int length) {
    _Config = static_cast<effect_config_t*>(data);
    return true;
}

bool Module_SetConfig_Handler::Execute(MenrvaModuleContext& context) {
    _Logger->WriteLog("Received SetConfig Command...", LOG_SENDER, __func__);
    effect_config_t& config = *_Config;

    _Logger->WriteLog("Input Buffer Configuration Details", LOG_SENDER, __func__, LogLevel::VERBOSE);
    LogBufferConfig(config.inputCfg);
    _Logger->WriteLog("Output Buffer Configuration Details", LOG_SENDER, __func__, LogLevel::VERBOSE);
    LogBufferConfig(config.outputCfg);

    _Logger->WriteLog("Validating Effect Config Parameters...", LOG_SENDER, __func__);
    if (config.inputCfg.samplingRate != config.outputCfg.samplingRate) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Sample Rate does not match Output Sample Rate.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (config.inputCfg.channels != config.outputCfg.channels && audio_channel_mask_in_to_out(config.inputCfg.channels) != config.outputCfg.channels) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Channels do not match Output Channels.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (config.inputCfg.format != AUDIO_FORMAT_PCM_16_BIT &&
        config.inputCfg.format != AUDIO_FORMAT_PCM_32_BIT &&
        config.inputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Format not supported (%u).", LOG_SENDER, __func__, LogLevel::ERROR, config.inputCfg.format);
        return -EINVAL;
    }
    if (config.outputCfg.format != AUDIO_FORMAT_PCM_16_BIT &&
        config.outputCfg.format != AUDIO_FORMAT_PCM_32_BIT &&
        config.outputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Output Format not supported (%u).", LOG_SENDER, __func__, LogLevel::ERROR, config.inputCfg.format);
        return -EINVAL;
    }
    if (config.outputCfg.accessMode != EFFECT_BUFFER_ACCESS_WRITE &&
        config.outputCfg.accessMode != EFFECT_BUFFER_ACCESS_ACCUMULATE) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Output Buffer Access Mode is not Write or Accumulate.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Calculating Channels Length...", LOG_SENDER, __func__);
    context.ChannelLength = audio_channel_count_from_out_mask(config.outputCfg.channels);
    if (context.ChannelLength < 1) {
        _Logger->WriteLog("Invalid Channels Length (%d).  Channel Mask must contain at least 1 channel.", LOG_SENDER, __func__, LogLevel::ERROR, context.ChannelLength);
    }

    if (!context.InputBuffer) {
        _Logger->WriteLog("Creating Audio Input Buffer Wrapper...", LOG_SENDER, __func__);
        context.InputBuffer = new AudioInputBuffer(_Logger);
    }
    if (!context.OutputBuffer) {
        _Logger->WriteLog("Creating Audio Output Buffer Wrapper...", LOG_SENDER, __func__);
        context.OutputBuffer = new AudioOutputBuffer(_Logger);
    }

    _Logger->WriteLog("Configuring Audio Buffer Wrappers...", LOG_SENDER, __func__);
    context.InputBuffer->SetFormat((AudioFormat)config.inputCfg.format);
    context.OutputBuffer->SetFormat((AudioFormat)config.outputCfg.format);

    _Logger->WriteLog("Configuring Effect Engine...", LOG_SENDER, __func__);
    context.config = config;
    context.EffectsEngine->SetBufferConfig(context.ChannelLength, config.inputCfg.samplingRate, MENRVA_DSP_FRAME_LENGTH);
    return true;
}

void Module_SetConfig_Handler::LogBufferConfig(buffer_config_t& bufferConfig) {
    _Logger->WriteLog("Buffer Format (0x%07x)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.format);
    _Logger->WriteLog("Buffer Sample Rate (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.samplingRate);
    _Logger->WriteLog("Buffer Channel Mask (0x%07x)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.channels);
    _Logger->WriteLog("Buffer Channel Count (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, audio_channel_count_from_out_mask(bufferConfig.channels));
    _Logger->WriteLog("Buffer Access Mode (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.accessMode);
}

uint32_t Module_SetConfig_Handler::SerializeResponse(void* responseBuffer) {
    *(int*)responseBuffer = 0;
    return sizeof(int);
}
