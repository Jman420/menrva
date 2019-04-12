package com.monkeystable.menrva.utilities;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.nio.charset.StandardCharsets;

public class ByteConverter {
    public byte[] convertToBytes(boolean value) {
        byte[] result = new byte[1];
        result[0] = (byte)(value ? 1 : 0);
        return result;
    }

    public byte[] convertToBytes(short value) {
        int sizeInBytes = getSizeInBytes(Short.SIZE);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        byteBuffer.putShort(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(short[] value) {
        int sizeInBytes = getSizeInBytes(Short.SIZE, value.length);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        ShortBuffer shortBuffer = byteBuffer.asShortBuffer();
        shortBuffer.put(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(int value) {
        int sizeInBytes = getSizeInBytes(Integer.SIZE);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        byteBuffer.putInt(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(int[] value) {
        int sizeInBytes = getSizeInBytes(Integer.SIZE * value.length);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        IntBuffer shortBuffer = byteBuffer.asIntBuffer();
        shortBuffer.put(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(float value) {
        int sizeInBytes = getSizeInBytes(Integer.SIZE);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        byteBuffer.putFloat(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(float[] value) {
        int sizeInBytes = getSizeInBytes(Integer.SIZE * value.length);
        ByteBuffer byteBuffer = ByteBuffer.allocate(sizeInBytes);
        FloatBuffer shortBuffer = byteBuffer.asFloatBuffer();
        shortBuffer.put(value);

        return byteBuffer.array();
    }

    public byte[] convertToBytes(String value) {
        return value.getBytes(StandardCharsets.US_ASCII);
    }

    private int getSizeInBytes(int typeSize) {
        return getSizeInBytes(typeSize, 1);
    }

    private int getSizeInBytes(int typeSize, int arrayLength) {
        return (typeSize / Byte.SIZE) * arrayLength;
    }
}
