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
#include "command_map.h"

const std::string MenrvaCommandMap::LOG_SENDER = "CommandMap";
ServiceLocator* MenrvaCommandMap::_ServiceLocator = new ServiceLocator();
LoggerBase* MenrvaCommandMap::_Logger = _ServiceLocator->GetLogger();

const function_map MenrvaCommandMap::_CommandMap = {
    { EFFECT_CMD_INIT, &MenrvaCommandMap::InitModule },
    { EFFECT_CMD_RESET, &MenrvaCommandMap::ResetBuffers },
    { EFFECT_CMD_GET_CONFIG, &MenrvaCommandMap::GetConfig },
    { EFFECT_CMD_SET_CONFIG, &MenrvaCommandMap::SetConfig },
    { EFFECT_CMD_GET_PARAM, &MenrvaCommandMap::GetParam },
    { EFFECT_CMD_SET_PARAM, &MenrvaCommandMap::SetParam },
    { EFFECT_CMD_ENABLE, &MenrvaCommandMap::EnableEngine },
    { EFFECT_CMD_DISABLE, &MenrvaCommandMap::DisableEngine },
};

int MenrvaCommandMap::Process(menrva_module_context& context, uint32_t cmdCode, uint32_t cmdSize,
                              void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Processing Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    if (context.ModuleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY){
        _Logger->WriteLog("Skipping Processing Command Id (%u).  Module Status is invalid.", LOG_SENDER, __func__, LogLevel::ERROR, cmdCode);
        return -EINVAL;
    }

    _Logger->WriteLog("Looking up Function for Command Id (%u)...", LOG_SENDER, __func__, cmdCode);
    auto cmdFunction = MenrvaCommandMap::_CommandMap.find(cmdCode);
    if (cmdFunction == MenrvaCommandMap::_CommandMap.end()) {
        _Logger->WriteLog("Unable to Process Command Id (%u).  No Function found.", LOG_SENDER, __func__, LogLevel::WARN, cmdCode);
        return 0;
    }

    _Logger->WriteLog("Function found for Command Id (%u).  Calling Function...", LOG_SENDER, __func__, cmdCode);
    int result = cmdFunction->second(context, cmdSize, pCmdData, replySize, pReplyData);
    _Logger->WriteLog("Successfully Processed Command Id (%u).  Command Function Result (%d)", LOG_SENDER, __func__, cmdCode, result);
    return result;
}

int MenrvaCommandMap::InitModule(menrva_module_context& context, uint32_t cmdSize __unused,
                                 void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
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

int MenrvaCommandMap::SetConfig(menrva_module_context& context, uint32_t cmdSize, void* pCmdData,
                                uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received SetConfig Command...", LOG_SENDER, __func__);
    if (pCmdData == nullptr || cmdSize != sizeof(effect_config_t) || pReplyData == nullptr ||
        replySize == nullptr || *replySize != sizeof(int)) {

        _Logger->WriteLog("Skipping SetConfig Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    auto config = (effect_config_t*)pCmdData;
    _Logger->WriteLog("Input Buffer Configuration Details", LOG_SENDER, __func__, LogLevel::VERBOSE);
    LogBufferConfig(config->inputCfg);
    _Logger->WriteLog("Output Buffer Configuration Details", LOG_SENDER, __func__, LogLevel::VERBOSE);
    LogBufferConfig(config->outputCfg);

    _Logger->WriteLog("Validating Effect Config Parameters...", LOG_SENDER, __func__);
    if (config->inputCfg.samplingRate != config->outputCfg.samplingRate) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Sample Rate does not match Output Sample Rate.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (config->inputCfg.channels != config->outputCfg.channels) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Channels do not match Output Channels.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    if (config->inputCfg.format != AUDIO_FORMAT_PCM_16_BIT &&
        config->inputCfg.format != AUDIO_FORMAT_PCM_32_BIT &&
        config->inputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Input Format not supported (%u).", LOG_SENDER, __func__, LogLevel::ERROR, config->inputCfg.format);
        return -EINVAL;
    }
    if (config->outputCfg.format != AUDIO_FORMAT_PCM_16_BIT &&
        config->outputCfg.format != AUDIO_FORMAT_PCM_32_BIT &&
        config->outputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Output Format not supported (%u).", LOG_SENDER, __func__, LogLevel::ERROR, config->inputCfg.format);
        return -EINVAL;
    }
    if (config->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_WRITE &&
        config->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_ACCUMULATE) {
        _Logger->WriteLog("Invalid Effect Config Parameters.  Output Buffer Access Mode is not Write or Accumulate.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
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
    context.InputBuffer->SetFormat((AudioFormat)config->inputCfg.format);
    context.OutputBuffer->SetFormat((AudioFormat)config->outputCfg.format);

    _Logger->WriteLog("Configuring Effect Engine...", LOG_SENDER, __func__);
    context.config = *config;
    int result = MenrvaCommandMap::ResetBuffers(context, (uint32_t) NULL, nullptr, nullptr, nullptr);
    *(int*)pReplyData = result;

    _Logger->WriteLog("Successfully Reconfigured Effect Engine with Result (%i)!", LOG_SENDER, __func__, result);
    return 0;
}

int MenrvaCommandMap::ResetBuffers(menrva_module_context &context, uint32_t cmdSize __unused,
                                   void* pCmdData __unused, uint32_t* replySize __unused,
                                   void* pReplyData __unused) {
    _Logger->WriteLog("Received ResetBuffers Command...", LOG_SENDER, __func__);
    if (context.EffectsEngine == nullptr) {
        _Logger->WriteLog("Skipping ResetBuffers Command.  Invalid Engine Instance provided.", LOG_SENDER, __func__, LogLevel::WARN);
        return 0;
    }

    _Logger->WriteLog("Resetting Effects Engine Buffers...", LOG_SENDER, __func__);
    context.EffectsEngine->ResetBuffers(context.config);

    _Logger->WriteLog("Successfully Reset Effects Engine Buffers!", LOG_SENDER, __func__);
    return 0;
}

int MenrvaCommandMap::EnableEngine(menrva_module_context& context, uint32_t cmdSize __unused,
                                   void* pCmdData __unused, uint32_t* replySize __unused,
                                   void* pReplyData __unused) {
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

int MenrvaCommandMap::DisableEngine(menrva_module_context& context, uint32_t cmdSize __unused,
                                    void* pCmdData __unused, uint32_t* replySize __unused,
                                    void* pReplyData __unused) {
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

int MenrvaCommandMap::SetParam(menrva_module_context& context, uint32_t cmdSize, void* pCmdData,
                               uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received SetParam Command...", LOG_SENDER, __func__);
    if (pCmdData == nullptr || cmdSize < (sizeof(effect_param_t) + sizeof(int32_t) + sizeof(int32_t))
        || pReplyData == nullptr || replySize == nullptr || *replySize != sizeof(int32_t)) {

        _Logger->WriteLog("Skipping SetParam Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Calculating Parameter Value Offset...", LOG_SENDER, __func__);
    const effect_param_t* effectParam = (effect_param_t*)pCmdData;
    if (effectParam->psize < sizeof(int)) {
        _Logger->WriteLog("Skipping SetParam Command.  Invalid Parameter Size provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }
    const uint32_t valueOffset = ComputeParamVOffset(*effectParam);
    _Logger->WriteLog("Successfully calculated Parameter Value Offset (%u).", LOG_SENDER, __func__, valueOffset);

    _Logger->WriteLog("Extracting Parameter Command Id...", LOG_SENDER, __func__);
    const uint32_t* param = (uint32_t*)effectParam->data;
    const uint32_t command = param[0];
    _Logger->WriteLog("Successfully extracted Parameter Command Id (%d).", LOG_SENDER, __func__, command);

    _Logger->WriteLog("Extracting Parameter Value...", LOG_SENDER, __func__);
    const uint32_t valueSize = effectParam->vsize;
    const void* pValue = (void*)(effectParam->data + valueOffset);
    const value_t* value = (value_t*)pValue;
    _Logger->WriteLog("Successfully calculated Parameter Value (%d).", LOG_SENDER, __func__, value);

    switch (command) {
        // TODO : Handle the expected engine parameter changes
    }

    _Logger->WriteLog("Successfully Processed Parameter Command Id (%u) with Parameter Value (%d)!", LOG_SENDER, __func__, command, value);
    return 0;
}

int MenrvaCommandMap::GetParam(menrva_module_context& context, uint32_t cmdSize __unused,
                               void* pCmdData, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received GetParam Command...", LOG_SENDER, __func__);
    if (pCmdData == nullptr || pReplyData == nullptr || replySize == nullptr) {
        _Logger->WriteLog("Skipping GetParam Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Calculating Expected Reply Data Size...", LOG_SENDER, __func__);
    const effect_param_t* pEffectParam = (effect_param_t*)pCmdData;
    const uint32_t expectedReplySize = GetExpectedReplySize(pEffectParam->psize, (void*)pEffectParam->data);
    _Logger->WriteLog("Successfully calculated Expected Reply Data Size (%u).", LOG_SENDER, __func__, expectedReplySize);

    _Logger->WriteLog("Preparing Reply Data...", LOG_SENDER, __func__);
    memcpy(pReplyData, pCmdData, expectedReplySize);
    auto replyData = (effect_param_t*)pReplyData;
    _Logger->WriteLog("Successfully prepared Reply Data.", LOG_SENDER, __func__);

    _Logger->WriteLog("Extracting Parameter Command Id...", LOG_SENDER, __func__);
    const uint32_t* param = (uint32_t*)replyData->data;
    const uint32_t command = param[0];
    _Logger->WriteLog("Successfully extracted Parameter Id (%u).", LOG_SENDER, __func__, command);

    _Logger->WriteLog("Calculating Parameter Value Offset...", LOG_SENDER, __func__);
    const uint32_t valueOffset = ComputeParamVOffset(*replyData);
    _Logger->WriteLog("Successfully calculated Parameter Value Offset (%u).", LOG_SENDER, __func__, valueOffset);

    _Logger->WriteLog("Retrieving Parameter Value for Parameter Id (%u).", LOG_SENDER, __func__, command);
    // TODO : Draft Structures to contain necessary Reply Data Types
    void* pValue = replyData->data + valueOffset;
    uint32_t valueSize = 0;
    switch (command) {
        // TODO : Handle retrieving expected engine parameter values
    }
    replyData->vsize = valueSize;
    *replySize = sizeof(effect_param_t) + valueOffset + replyData->vsize;

    _Logger->WriteLog("Successfully retrieved Parameter Value for Parameter Id (%u).", LOG_SENDER, __func__, command);
    return 0;
}

int MenrvaCommandMap::GetConfig(menrva_module_context& context, uint32_t cmdSize __unused,
                                void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
    _Logger->WriteLog("Received GetConfig Command...", LOG_SENDER, __func__);
    if (pReplyData == nullptr || *replySize != sizeof(effect_config_t)) {
        _Logger->WriteLog("Skipping GetConfig Command.  Invalid parameters provided.", LOG_SENDER, __func__, LogLevel::ERROR);
        return -EINVAL;
    }

    _Logger->WriteLog("Successfully Retrieved Engine Config.", LOG_SENDER, __func__);
    pReplyData = &context.config;
    return 0;
}

uint32_t MenrvaCommandMap::GetExpectedReplySize(uint32_t paramSize, void* pParam) {
    if (paramSize < sizeof(int32_t)) {
        return 0;
    }

    int32_t param = *(int32_t*)pParam;

    switch (param) {
        // TODO : Add logic to return an expected parameter size
    }

    return 0;
}

// The value offset of an effect parameter is computed by rounding up
// the parameter size to the next 32 bit alignment.
// This method was taken from https://android.googlesource.com/platform/frameworks/av/+/master/media/libeffects/dynamicsproc/EffectDynamicsProcessing.cpp#77
uint32_t MenrvaCommandMap::ComputeParamVOffset(const effect_param_t& p) {
    return ((p.psize + sizeof(int32_t) - 1) / sizeof(int32_t)) * sizeof(int32_t);
}

void MenrvaCommandMap::LogBufferConfig(buffer_config_t& bufferConfig) {
    _Logger->WriteLog("Buffer Format (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.format);
    _Logger->WriteLog("Buffer Sample Rate (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.samplingRate);
    _Logger->WriteLog("Buffer Channel Count (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.channels);
    _Logger->WriteLog("Buffer Access Mode (%u)", LOG_SENDER, __func__, LogLevel::VERBOSE, bufferConfig.accessMode);
}
