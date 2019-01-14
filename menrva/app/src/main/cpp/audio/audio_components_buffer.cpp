/* Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2018 Justin Giannone (aka Jman420)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cstdlib>
#include "audio_components_buffer.h"

AudioComponentsBuffer::AudioComponentsBuffer(LoggerBase* logger, FftInterfaceBase* fftEngine, size_t length)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _Length = length;
    _RealBuffer = new AudioBuffer(_Logger, fftEngine, length);
    _ImagBuffer = new AudioBuffer(_Logger, fftEngine, length);
}

AudioComponentsBuffer::~AudioComponentsBuffer() {
    Free();

    _Logger->WriteLog("Disposing of Real & Imaginary Audio Buffers...", LOG_SENDER, __func__);
    delete _RealBuffer;
    delete _ImagBuffer;
    _Logger->WriteLog("Successfully disposed of Real & Imaginary Audio Buffers!", LOG_SENDER, __func__);
}

void AudioComponentsBuffer::Free() {
    _Logger->WriteLog("Resetting Real & Imaginary Audio Buffers...", LOG_SENDER, __func__);
    _RealBuffer->Free();
    _ImagBuffer->Free();
    _Logger->WriteLog("Successfully reset Real & Imaginary Audio Buffers!", LOG_SENDER, __func__);
}

void AudioComponentsBuffer::ResetData() {
    _Logger->WriteLog("Resetting Real & Imaginary Audio Data...", LOG_SENDER, __func__);
    _RealBuffer->ResetData();
    _ImagBuffer->ResetData();
    _Length = 0;
    _Logger->WriteLog("Successfully reset Real & Imaginary Audio Data!", LOG_SENDER, __func__);
}

size_t AudioComponentsBuffer::GetLength() {
    return _Length;
}

sample* AudioComponentsBuffer::GetRealData() {
    return _RealBuffer->GetData();
}

AudioBuffer* AudioComponentsBuffer::GetRealBuffer() {
    return _RealBuffer;
}

sample* AudioComponentsBuffer::GetImagData() {
    return _ImagBuffer->GetData();
}

AudioBuffer* AudioComponentsBuffer::GetImagBuffer() {
    return _ImagBuffer;
}
