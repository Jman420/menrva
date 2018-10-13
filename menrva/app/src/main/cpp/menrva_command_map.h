//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_MENRVA_COMMAND_MAP_H
#define MENRVA_MENRVA_COMMAND_MAP_H

#include <map>

#include "aosp/hardware/audio_effect.h"

#include "menrva_interface.h"

typedef void (*CommandFunc)(menrva_engine_context*, uint32_t, void*, uint32_t*, void*);
typedef std::map<uint32_t, CommandFunc> function_map;

class MenrvaCommandMap {
public:
    static int Process(menrva_engine_context *context, uint32_t cmdCode, uint32_t cmdSize,
                           void *pCmdData, uint32_t *replySize, void *pReplyData);

private:
    MenrvaCommandMap();  // This is a Static Class
    static void Enable(menrva_engine_context *context, uint32_t cmdSize, void *pCmdData,
                       uint32_t *replySize, void *pReplyData);
    static void Disable(menrva_engine_context *context, uint32_t cmdSize, void *pCmdData,
                        uint32_t *replySize, void *pReplyData);

    static function_map CommandMap;
};

#endif //MENRVA_MENRVA_COMMAND_MAP_H
