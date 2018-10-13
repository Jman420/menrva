//
// Created by jgiannone on 10/12/2018.
//

#ifndef MENRVA_MENRVA_EFFECT_H
#define MENRVA_MENRVA_EFFECT_H

#include <map>

#include "aosp/hardware/audio_effect.h"

typedef void (*CommandFunc)(uint32_t, void*, uint32_t*, void*);
typedef std::map<uint32_t, CommandFunc> function_map;

class MenrvaEffectsEngine {
protected:
    function_map mCommandMap= {
            { EFFECT_CMD_ENABLE, &MenrvaEffectsEngine::enable },
            { EFFECT_CMD_DISABLE, &MenrvaEffectsEngine::disable }
    };
    bool mEnabled;

    void enable(uint32_t cmdSize, void *pCmdData, uint32_t *replySize, void *pReplyData);
    void disable(uint32_t cmdSize, void *pCmdData, uint32_t *replySize, void *pReplyData);

public:
    MenrvaEffectsEngine();
    virtual ~MenrvaEffectsEngine();
    virtual int32_t process(audio_buffer_t *in, audio_buffer_t *out) = 0;
    virtual int32_t command(uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize, void* pReplyData) = 0;
};


#endif //MENRVA_MENRVA_EFFECT_H
