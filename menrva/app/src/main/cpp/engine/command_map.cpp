// Author : Jman420

#include <cerrno>
#include "command_map.h"

function_map MenrvaCommandMap::CommandMap = {
    { EFFECT_CMD_INIT, &MenrvaCommandMap::InitModule },
    { EFFECT_CMD_SET_CONFIG, &MenrvaCommandMap::SetConfig },
    { EFFECT_CMD_RESET, &MenrvaCommandMap::ResetEngine },
    { EFFECT_CMD_ENABLE, &MenrvaCommandMap::EnableEngine },
    { EFFECT_CMD_DISABLE, &MenrvaCommandMap::DisableEngine },
    { EFFECT_CMD_SET_PARAM, &MenrvaCommandMap::SetParam },
    { EFFECT_CMD_GET_PARAM, &MenrvaCommandMap::GetParam },
    { EFFECT_CMD_GET_CONFIG, &MenrvaCommandMap::GetConfig },
};

// The value offset of an effect parameter is computed by rounding up
// the parameter size to the next 32 bit alignment.
// This method was taken from https://android.googlesource.com/platform/frameworks/av/+/master/media/libeffects/dynamicsproc/EffectDynamicsProcessing.cpp#77
static inline uint32_t computeParamVOffset(const effect_param_t* p) {
    return ((p->psize + sizeof(int32_t) - 1) / sizeof(int32_t)) * sizeof(int32_t);
}

static uint32_t getExpectedCmdSize(uint32_t paramSize, void* pParam) {
    if (paramSize < sizeof(int32_t)) {
        return 0;
    }

    int32_t param = *(int32_t*)pParam;

    switch (param) {
        // TODO : Add logic to return an expected parameter size
    }

    return 0;
}

int MenrvaCommandMap::Command(menrva_module_context* context, uint32_t cmdCode, uint32_t cmdSize,
                              void* pCmdData, uint32_t* replySize, void* pReplyData) {
    if (context->moduleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY){
        return -EINVAL;
    }

    std::map<uint32_t, CommandFunc> commandMap = MenrvaCommandMap::CommandMap;

    int result = 0;
    function_map::iterator cmdFunction = commandMap.find(cmdCode);
    if (cmdFunction != commandMap.end()) {
        result = cmdFunction->second(context, cmdSize, pCmdData, replySize, pReplyData);
    }

    return result;
}

int MenrvaCommandMap::InitModule(menrva_module_context* context, uint32_t cmdSize __unused,
                                 void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
    if (pReplyData == NULL || *replySize != sizeof(int)) {
        return -EINVAL;
    }

    MenrvaModuleInterface::InitModule(context);
    *(int*)pReplyData = 0;
    return 0;
}

int MenrvaCommandMap::SetConfig(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                                uint32_t* replySize, void* pReplyData) {
    if (pCmdData == NULL || cmdSize != sizeof(effect_config_t) || pReplyData == NULL ||
        replySize == NULL || *replySize != sizeof(int)) {

        return -EINVAL;
    }

    effect_config_t* config = (effect_config_t*)pCmdData;

    if (config->inputCfg.samplingRate != config->outputCfg.samplingRate) {
        return -EINVAL;
    }
    if (config->inputCfg.channels != config->outputCfg.channels) {
        return -EINVAL;
    }
    if (config->inputCfg.format != config->outputCfg.format) {
        return -EINVAL;
    }
    if (config->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_WRITE &&
        config->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_ACCUMULATE) {
        return -EINVAL;
    }
    if (config->inputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        return -EINVAL;
    }

    *context->config = *config;
    MenrvaCommandMap::ResetEngine(context, (uint32_t)NULL, NULL, NULL, NULL);

    *(int*)pReplyData = 0;
    return 0;
}

int MenrvaCommandMap::ResetEngine(menrva_module_context* context, uint32_t cmdSize __unused,
                                  void* pCmdData __unused, uint32_t* replySize __unused,
                                  void* pReplyData __unused) {
    if (context->effectsEngine != NULL) {
        context->effectsEngine->ResetEffects();
    }
    return 0;
}

int MenrvaCommandMap::EnableEngine(menrva_module_context* context, uint32_t cmdSize __unused,
                                   void* pCmdData __unused, uint32_t* replySize __unused,
                                   void* pReplyData __unused) {
    if (pReplyData == NULL || replySize == NULL || *replySize != sizeof(int)) {
        return -EINVAL;
    }

    context->effectsEngine->engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_ENABLED;

    *(int*)pReplyData = 0;
    return 0;
}

int MenrvaCommandMap::DisableEngine(menrva_module_context* context, uint32_t cmdSize __unused,
                                    void* pCmdData __unused, uint32_t* replySize __unused,
                                    void* pReplyData __unused) {
    if (pReplyData == NULL || replySize == NULL || *replySize != sizeof(int)) {
        return -EINVAL;
    }

    context->effectsEngine->engineStatus = MenrvaEngineStatus::MENRVA_ENGINE_DISABLED;

    *(int*)pReplyData = 0;
    return 0;
}

int MenrvaCommandMap::SetParam(menrva_module_context* context, uint32_t cmdSize, void* pCmdData,
                               uint32_t* replySize, void* pReplyData) {
    if (pCmdData == NULL || cmdSize < (sizeof(effect_param_t) + sizeof(int32_t) + sizeof(int32_t))
        || pReplyData == NULL || replySize == NULL || *replySize != sizeof(int32_t)) {

        return -EINVAL;
    }

    const effect_param_t* effectParam = (effect_param_t*)pCmdData;
    const uint32_t valueOffset = computeParamVOffset(effectParam);

    const uint32_t paramSize = effectParam->psize;
    if (paramSize < sizeof(int)) {
        return -EINVAL;
    }

    const uint32_t* param = (uint32_t*)effectParam->data;
    const int command = param[0];
    const uint32_t valueSize = effectParam->vsize;
    const void* pValue = (void*)(effectParam->data + valueOffset);
    const value_t* value = (value_t*)pValue;

    switch (command) {
        // TODO : Handle the expected engine parameter changes
    }

    return 0;
}

int MenrvaCommandMap::GetParam(menrva_module_context* context, uint32_t cmdSize __unused,
                               void* pCmdData, uint32_t* replySize, void* pReplyData) {
    if (pCmdData == NULL || pReplyData == NULL || replySize == NULL) {
        return -EINVAL;
    }

    const effect_param_t* pEffectParam = (effect_param_t*)pCmdData;
    const uint32_t expectedCmdSize = getExpectedCmdSize(pEffectParam->psize,
                                                        (void*)pEffectParam->data);
    memcpy(pReplyData, pCmdData, expectedCmdSize);
    const effect_param_t* replyData = (effect_param_t*)pReplyData;
    const uint32_t valueOffset = computeParamVOffset(replyData);

    const uint32_t* param = (uint32_t*)replyData->data;
    const int command = param[0];
    const uint32_t valueSize = replyData->vsize;
    const void* pValue = (void*)(replyData->data + valueOffset);
    const value_t* value = (value_t*)pValue;

    switch (command) {
        // TODO : Handle retrieving expected engine parameter values
    }

    *replySize = sizeof(effect_param_t) + valueOffset + replyData->vsize;
    return 0;
}

int MenrvaCommandMap::GetConfig(menrva_module_context* context, uint32_t cmdSize __unused,
                                void* pCmdData __unused, uint32_t* replySize, void* pReplyData) {
    if (pReplyData == NULL || *replySize != sizeof(effect_config_t)) {
        return -EINVAL;
    }

    pReplyData = context->config;
    return 0;
}

MenrvaCommandMap::MenrvaCommandMap() {}
