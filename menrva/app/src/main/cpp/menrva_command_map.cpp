//
// Created by jgiannone on 10/13/2018.
//

#include "menrva_command_map.h"

int MenrvaCommandMap::Process(menrva_engine_context *context, uint32_t cmdCode, uint32_t cmdSize,
                              void *pCmdData, uint32_t *replySize, void *pReplyData) {
    std::map<uint32_t, CommandFunc> commandMap = MenrvaCommandMap::CommandMap;

    function_map::iterator cmdFunction = commandMap.find(cmdCode);
    if (cmdFunction != commandMap.end()) {
        cmdFunction->second(context, cmdSize, pCmdData, replySize, pReplyData);
    }

    return 0;
}

void MenrvaCommandMap::Enable(menrva_engine_context *context, uint32_t cmdSize, void *pCmdData,
                              uint32_t *replySize, void *pReplyData) {
    context->moduleState = 1;
}

void MenrvaCommandMap::Disable(menrva_engine_context *context, uint32_t cmdSize, void *pCmdData,
                               uint32_t *replySize, void *pReplyData) {
    context->moduleState = 0;
}

function_map MenrvaCommandMap::CommandMap = {
    { EFFECT_CMD_ENABLE, &MenrvaCommandMap::Enable },
    { EFFECT_CMD_DISABLE, &MenrvaCommandMap::Disable }
};

MenrvaCommandMap::MenrvaCommandMap() {}
