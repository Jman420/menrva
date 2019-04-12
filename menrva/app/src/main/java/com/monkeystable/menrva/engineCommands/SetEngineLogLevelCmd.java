package com.monkeystable.menrva.engineCommands;

public class SetEngineLogLevelCmd extends EngineCommand {
    private static final short COMMAND_ID = 0;
    private static final int BUFFER_SIZE = (Short.SIZE / Byte.SIZE) * 2;

    private short _EngineLogLevel;

    public void setEngineLogLevel(short value) {
        _EngineLogLevel = value;
    }

    public short getEngineLogLevel() {
        return _EngineLogLevel;
    }

    @Override
    protected int getBufferSize() {
        // Command includes Standard Header (short) and Log Level (short)
        return BUFFER_SIZE;
    }

    @Override
    protected void writeHeader() {
        addParameter(COMMAND_ID);
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
