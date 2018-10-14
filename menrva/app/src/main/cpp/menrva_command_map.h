//
// Created by jgiannone on 10/13/2018.
//

#ifndef MENRVA_MENRVA_COMMAND_MAP_H
#define MENRVA_MENRVA_COMMAND_MAP_H

#include <map>

#include "aosp/hardware/audio_effect.h"

#include "menrva_module_interface.h"

typedef int (*CommandFunc)(menrva_module_context*, uint32_t, void*, uint32_t*, void*);
typedef std::map<uint32_t, CommandFunc> function_map;

// union to hold command values
using value_t = union {
    int32_t integer;
    float decimal;
};

class MenrvaCommandMap {
public:
    static int Command(menrva_module_context *context, uint32_t cmdCode, uint32_t cmdSize,
                       void *pCmdData, uint32_t *replySize, void *pReplyData);

private:
    MenrvaCommandMap();  // This is a Static Class
    static int InitModule(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                          uint32_t *replySize, void *pReplyData);
    static int SetConfig(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                         uint32_t *replySize, void *pReplyData);
    static int ResetEngine(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                           uint32_t *replySize, void *pReplyData);
    static int Enable(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                      uint32_t *replySize, void *pReplyData);
    static int Disable(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                       uint32_t *replySize, void *pReplyData);
    static int SetParam(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                        uint32_t *replySize, void *pReplyData);
    static int GetParam(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                        uint32_t *replySize, void *pReplyData);
    static int GetConfig(menrva_module_context *context, uint32_t cmdSize, void *pCmdData,
                         uint32_t *replySize, void *pReplyData);

    static function_map CommandMap;
};

#endif //MENRVA_MENRVA_COMMAND_MAP_H
