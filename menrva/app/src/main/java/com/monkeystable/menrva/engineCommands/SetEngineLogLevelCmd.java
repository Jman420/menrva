package com.monkeystable.menrva.engineCommands;

public class SetEngineLogLevelCmd extends EngineCommand {
    private int _EngineLogLevel;

    public SetEngineLogLevelCmd() {
        super();
    }

    public void setEngineLogLevel(int value) {
        _EngineLogLevel = value;
    }

    public int getEngineLogLevel() {
        return _EngineLogLevel;
    }

    @Override
    protected int getBufferSize() {
        // Command includes Standard Header (short) and Log Level (short)
        return (Short.SIZE / Byte.SIZE) * 2;
    }

    @Override
    protected void addHeader() {
        // TODO : Add Standard Command Header (short which represents the command)
    }

    @Override
    protected boolean validate() {
        // TODO : Validate the value of Engine Log Level
        return false;
    }

    @Override
    protected void writeParameters() {
        addParameter(_EngineLogLevel);
    }
}
