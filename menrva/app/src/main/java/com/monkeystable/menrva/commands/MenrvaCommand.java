package com.monkeystable.menrva.commands;

import com.google.protobuf.MessageLite;

public abstract class MenrvaCommand<TRequest extends MessageLite, TResponse extends MessageLite> {
    static private final int FIRST_PROPRIETARY_CMD = 0x10000;

    private int _Command;
    private TRequest _Request;
    private TResponse _Response;

    public MenrvaCommand(int command, TRequest request, TResponse response) {
        _Command = FIRST_PROPRIETARY_CMD + command;
        _Request = request;
        _Response = response;
    }

    public int getCommand() {
        return _Command;
    }

    public TRequest getRequest() {
        return _Request;
    }

    public TResponse getResponse() {
        return _Response;
    }

    public void setResponse(TResponse value) {
        _Response = value;
    }
}
