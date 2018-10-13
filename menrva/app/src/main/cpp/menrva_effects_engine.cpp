//
// Created by jgiannone on 10/12/2018.
//

#include "menrva_effects_engine.h"

void MenrvaEffectsEngine::enable(uint32_t cmdSize, void *pCmdData, uint32_t *replySize,
                           void *pReplyData) {
    mEnabled = true;

    // TODO : Setup any necessary dependencies & instantiate any necessary classes

    int32_t *replyData = (int32_t *) pReplyData;
    *replyData = 0;
}

void MenrvaEffectsEngine::disable(uint32_t cmdSize, void *pCmdData, uint32_t *replySize,
                            void *pReplyData) {
    mEnabled = false;

    // TODO : Dispose of all dependencies & classes

    int32_t *replyData = (int32_t *) pReplyData;
    *replyData = 0;
}

int32_t MenrvaEffectsEngine::command(uint32_t cmdCode, uint32_t cmdSize, void *pCmdData,
                              uint32_t *replySize, void *pReplyData) {
    function_map::iterator cmdFunction = mCommandMap.find(cmdCode);

    if (cmdFunction != mCommandMap.end()) {
        cmdFunction->second(cmdSize, pCmdData, replySize, pReplyData);
    }

    return 0;
}

int32_t MenrvaEffectsEngine::process(audio_buffer_t *in, audio_buffer_t *out) {
    if (!mEnabled) {
        return -ENODATA;
    }

    return 0;
}