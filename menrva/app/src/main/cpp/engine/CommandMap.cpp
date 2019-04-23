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

#include <cerrno>
#include "CommandMap.h"
#include "../commands/MenrvaCommands.h"
#include "../commands/messages/Engine_GetVersion.pb.h"
#include "../commands/Engine_GetVersion_Command.h"
#include "../tools/CommandIds.h"
#include "command_handlers/Engine_GetVersion_Handler.h"

const std::string MenrvaCommandMap::LOG_SENDER = "CommandMap";
ServiceLocator* MenrvaCommandMap::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaCommandMap::_Logger = _ServiceLocator->GetLogger();

const function_map MenrvaCommandMap::COMMAND_MAP = {
        { EFFECT_CMD_INIT, &MenrvaCommandMap::InitModule },
        { EFFECT_CMD_RESET, &MenrvaCommandMap::ResetBuffers },
        { EFFECT_CMD_GET_CONFIG, &MenrvaCommandMap::GetConfig },
        { EFFECT_CMD_SET_CONFIG, &MenrvaCommandMap::SetConfig },
        { EFFECT_CMD_ENABLE, &MenrvaCommandMap::EnableEngine },
        { EFFECT_CMD_DISABLE, &MenrvaCommandMap::DisableEngine },
        { CommandIds::Calculate(MenrvaCommands::Engine_GetVersion), &MenrvaCommandMap::GetVersion },
};

int MenrvaCommandMap::Process(MenrvaModuleContext& context, uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Processing Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    if (context.ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY){
        _Logger->WriteLog("Skipping Processing Command Id (%u).  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::ERROR, cmdCode);
        return -EINVAL;
    }

    _Logger->WriteLog("Looking up Function for Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    auto cmdFunction = MenrvaCommandMap::COMMAND_MAP.find(cmdCode);
    if (cmdFunction == MenrvaCommandMap::COMMAND_MAP.end()) {
        _Logger->WriteLog("Unable to Process Command Id (%u).  No Function found.", LOG_SENDER, __func__, LogLevel::WARN, cmdCode);
        return 0;
    }

    _Logger->WriteLog("Function found for Command Id (%u).  Calling Function...", LOG_SENDER, __func__, cmdCode);
    int result = cmdFunction->second(context, cmdSize, pCmdData, replySize, pReplyData);
    _Logger->WriteLog("Successfully Processed Command Id (%u).  Command Function Result (%d).", LOG_SENDER, __func__, cmdCode, result);
    return result;
}

int MenrvaCommandMap::InitModule(MenrvaModuleContext& context, uint32_t cmdSize __unused, void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received InitModule Command...", LOG_SENDER, __func__);
    if (pReplyData == nullptr || *replySize != sizeof(int)) {
        _Logger->WriteLog("Skipping InitModule Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Initializing Module Context...", LOG_SENDER, __func__);
    int result = MenrvaModuleInterface::InitModule(context);
    *(int*)pReplyData = result;

    _Logger->WriteLog("Successfully Initialized Module with Result (%i).", LOG_SENDER, __func__, result);
    return 0;
}

int MenrvaCommandMap::GetConfig(MenrvaModuleContext& context, uint32_t cmdSize __unused, void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received GetConfig Command...", LOG_SENDER, __func__);
    if (pReplyData == nullptr || *replySize != sizeof(effect_config_t)) {
        _Logger->WriteLog("Skipping GetConfig Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Successfully Retrieved Engine Config.", LOG_SENDER, __func__);
    *(effect_config_t*)pReplyData = context.config;
    return 0;
}

int MenrvaCommandMap::SetConfig(MenrvaModuleContext& context, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received SetConfig Command...", LOG_SENDER, __func__);
    if (pCmdData == nullptr || cmdSize != sizeof(effect_config_t) || pReplyData == nullptr ||
        replySize == nullptr || *replySize != sizeof(int)) {

        _Logger->WriteLog("Skipping SetConfig Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    effect_config_t& config = *static_cast<effect_config_t*>(pCmdData);
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
        context.InputBuffer = new AudioInputBuffer(_ServiceLocator->GetLogger());
    }
    if (!context.OutputBuffer) {
        _Logger->WriteLog("Creating Audio Output Buffer Wrapper...", LOG_SENDER, __func__);
        context.OutputBuffer = new AudioOutputBuffer(_ServiceLocator->GetLogger());
    }

    _Logger->WriteLog("Configuring Audio Buffer Wrappers...", LOG_SENDER, __func__);
    context.InputBuffer->SetFormat((AudioFormat)config.inputCfg.format);
    context.OutputBuffer->SetFormat((AudioFormat)config.outputCfg.format);

    _Logger->WriteLog("Configuring Effect Engine...", LOG_SENDER, __func__);
    context.config = config;
    int result = context.EffectsEngine->SetBufferConfig(context.ChannelLength, config.inputCfg.samplingRate, MENRVA_DSP_FRAME_LENGTH);
    *(int*)pReplyData = result;

    _Logger->WriteLog("Successfully Reconfigured Effect Engine with Result (%i)!", LOG_SENDER, __func__, result);
    return 0;
}

int MenrvaCommandMap::ResetBuffers(MenrvaModuleContext &context, uint32_t cmdSize __unused, void* pCmdData __unused, uint32_t* replySize __unused, void* pReplyData __unused) {
    _Logger->WriteLog("Received ResetBuffers Command...", LOG_SENDER, __func__);
    if (context.EffectsEngine == nullptr) {
        _Logger->WriteLog("Skipping ResetBuffers Command.  Invalid Engine Instance provided.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }

    _Logger->WriteLog("Resetting Effects Engine Buffers...", LOG_SENDER, __func__);
    context.EffectsEngine->ResetBuffers(context.config.inputCfg.samplingRate);

    _Logger->WriteLog("Successfully Reset Effects Engine Buffers!", LOG_SENDER, __func__);
    return 0;
}

int MenrvaCommandMap::EnableEngine(MenrvaModuleContext& context, uint32_t cmdSize __unused, void* pCmdData __unused, uint32_t* replySize __unused, void* pReplyData __unused) {
    _Logger->WriteLog("Received EnableEngine Command...", LOG_SENDER, __func__);
    if (pReplyData == nullptr || replySize == nullptr || *replySize != sizeof(int)) {
        _Logger->WriteLog("Skipping EnableEngine Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Enabling Effects Engine...", LOG_SENDER, __func__);
    context.EffectsEngine->_EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_ENABLED;
    *(int*)pReplyData = 0;

    _Logger->WriteLog("Successfully Enabled Effects Engine!", LOG_SENDER, __func__);
    return 0;
}

int MenrvaCommandMap::DisableEngine(MenrvaModuleContext& context, uint32_t cmdSize __unused, void* pCmdData __unused, uint32_t* replySize __unused, void* pReplyData __unused) {
    _Logger->WriteLog("Received DisableEngine Command...", LOG_SENDER, __func__);
    if (pReplyData == nullptr || replySize == nullptr || *replySize != sizeof(int)) {
        _Logger->WriteLog("Skipping DisableEngine Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Disabling Effects Engine...", LOG_SENDER, __func__);
    context.EffectsEngine->_EngineStatus = MenrvaEngineStatus::MENRVA_ENGINE_DISABLED;
    *(int*)pReplyData = 0;

    _Logger->WriteLog("Successfully Disabled Effects Engine!", LOG_SENDER, __func__);
    return 0;
}

int MenrvaCommandMap::GetVersion(MenrvaModuleContext& context, uint32_t __unused cmdSize, void* __unused pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received GetVersion Command...", LOG_SENDER, __func__);
    Engine_GetVersion_Handler handler(_ServiceLocator->GetLogger());
    handler.DeserializeRequest(pCmdData, cmdSize);
    if (!handler.Execute(context)) {
        *replySize = 0;
        return 0;
    }

    _Logger->WriteLog("Serializing GetVersion Response...", LOG_SENDER, __func__);
    int responseSize = handler.SerializeResponse(static_cast<byte*>(pReplyData));
    *replySize = static_cast<uint32_t>(responseSize);

    _Logger->WriteLog("Successfully returned GetVersion Response.", LOG_SENDER, __func__);
    return 0;
}

void MenrvaCommandMap::LogBufferConfig(buffer_config_t& bufferConfig) {
    _Logger->WriteLog("Buffer Format (0x%07x)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.format);
    _Logger->WriteLog("Buffer Sample Rate (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.samplingRate);
    _Logger->WriteLog("Buffer Channel Mask (0x%07x)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.channels);
    _Logger->WriteLog("Buffer Channel Count (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, audio_channel_count_from_out_mask(bufferConfig.channels));
    _Logger->WriteLog("Buffer Access Mode (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.accessMode);
}
