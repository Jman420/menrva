//
// Created by jgiannone on 4/25/2019.
//

#ifndef MENRVA_MODULE_ENABLEENGINE_HANDLER_H
#define MENRVA_MODULE_ENABLEENGINE_HANDLER_H

#include "TypedCommandHandlerBase.h"
#include "../commands/Android_SystemCommand_Command.h"

class Module_EnableEngine_Handler
        : public TypedCommandHandlerBase<Android_SystemCommand_Command> {
public:
    Module_EnableEngine_Handler(LoggerBase *logger);

    virtual bool Execute(MenrvaModuleContext& context) override;
};

#endif //MENRVA_MODULE_ENABLEENGINE_HANDLER_H
