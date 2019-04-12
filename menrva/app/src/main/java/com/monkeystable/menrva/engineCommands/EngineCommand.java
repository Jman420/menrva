package com.monkeystable.menrva.engineCommands;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.nio.charset.StandardCharsets;

public abstract class EngineCommand {
    private ByteBuffer _ValueBuffer;

    public EngineCommand() {
        int bufferSize = getBufferSize();
        _ValueBuffer = ByteBuffer.allocate(bufferSize);

        writeHeader();
    }

    public byte[] getBytes() {
        return _ValueBuffer.array();
    }

    protected void addParameter(boolean value) {
        _ValueBuffer.put((byte)(value ? 1 : 0));
    }

    protected void addParameter(short value) {
        _ValueBuffer.putShort(value);
    }

    protected void addParameter(short[] value) {
        ShortBuffer buffer = _ValueBuffer.asShortBuffer();
        buffer.put(value);
    }

    protected void addParameter(int value) {
        _ValueBuffer.putInt(value);
    }

    protected void addParameter(int[] value) {
        IntBuffer buffer = _ValueBuffer.asIntBuffer();
        buffer.put(value);
    }

    protected void addParameter(float value) {
        _ValueBuffer.putFloat(value);
    }

    protected void addParameter(float[] value) {
        FloatBuffer buffer = _ValueBuffer.asFloatBuffer();
        buffer.put(value);
    }

    protected void addParameter(String value) {
        _ValueBuffer.put(value.getBytes(StandardCharsets.US_ASCII));
    }

    protected abstract int getBufferSize();

    protected abstract void writeHeader();

    protected abstract boolean validate();

    protected abstract void writeParameters();
}
