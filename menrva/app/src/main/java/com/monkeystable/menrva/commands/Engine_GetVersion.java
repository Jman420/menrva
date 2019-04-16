package com.monkeystable.menrva.commands;

import com.monkeystable.menrva.commands.messages.Engine_GetVersion.Engine_GetVersion_Request;
import com.monkeystable.menrva.commands.messages.Engine_GetVersion.Engine_GetVersion_Response;

public class Engine_GetVersion
        extends MenrvaCommand<Engine_GetVersion_Request, Engine_GetVersion_Response> {
    public Engine_GetVersion() {
        super(EngineCommands.GET_VERSION.ordinal(), Engine_GetVersion_Request.newBuilder().build(), Engine_GetVersion_Response.newBuilder().build());
    }
}
