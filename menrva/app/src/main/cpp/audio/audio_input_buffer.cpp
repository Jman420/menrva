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

#include "audio_input_buffer.h"
#include "../tools/conversion_buffer.cpp"

AudioInputBuffer::AudioInputBuffer(LoggerBase* logger)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _BufferWrapper = new audio_input_buffer_u();
}

AudioInputBuffer::AudioInputBuffer(LoggerBase* logger, AudioFormat audioFormat)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _BufferWrapper = new audio_input_buffer_u();
    SetFormat(audioFormat);
}

AudioInputBuffer::~AudioInputBuffer() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            delete &_BufferWrapper->PCM_16;
            break;

        case AudioFormat::PCM_32:
            delete &_BufferWrapper->PCM_32;
            break;

        case AudioFormat::PCM_Float:
            delete &_BufferWrapper->PCM_Float;
            break;

        default:
            break;
    }

    delete _BufferWrapper;
}

size_t AudioInputBuffer::GetLength() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            return _BufferWrapper->PCM_16->GetLength();

        case AudioFormat::PCM_32:
            return _BufferWrapper->PCM_32->GetLength();

        case AudioFormat::PCM_Float:
            return _BufferWrapper->PCM_Float->GetLength();

        default:
            break;
    }

    return 0;
}

void AudioInputBuffer::ResetData() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16->ResetData();
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32->ResetData();
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->ResetData();
            break;

        default:
            break;
    }
}

void AudioInputBuffer::Free() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16->Free();
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32->Free();
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->Free();
            break;

        default:
            break;
    }
}

void AudioInputBuffer::SetFormat(AudioFormat audioFormat) {
    if (_AudioFormat == audioFormat) {
        return;
    }

    _AudioFormat = audioFormat;
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16 = new ConversionBuffer<int16_t, sample>();
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32 = new ConversionBuffer<int32_t, sample>();
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float = new ConversionBuffer<float, sample>();
            break;

        default:
            // TODO : Throw exception
            _Logger->WriteLog("Invalid Audio Format Provided.  No Data Set!", LOG_SENDER, __func__, LogLevel::WARN);
            break;
    }
}

void AudioInputBuffer::SetData(void* data, size_t length) {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16->SetData((int16_t*)data, length);
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32->SetData((int32_t*)data, length);
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->SetData((float*)data, length);
            break;

        default:
            // TODO : Throw exception
            _Logger->WriteLog("Invalid Audio Format Provided.  No Data Set!", LOG_SENDER, __func__, LogLevel::WARN);
            break;
    }
}

void AudioInputBuffer::SetData(AudioFormat audioFormat, void* data, size_t length) {
    SetFormat(audioFormat);
    SetData(data, length);
}

void* AudioInputBuffer::GetData() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            return _BufferWrapper->PCM_16->GetData();

        case AudioFormat::PCM_32:
            return _BufferWrapper->PCM_32->GetData();

        case AudioFormat::PCM_Float:
            return _BufferWrapper->PCM_Float->GetData();

        default:
            return nullptr;
    }
}

sample AudioInputBuffer::operator[](size_t index) const {
    // TODO : Scale PCM16 & 32 to Float Format
    switch (_AudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t value16 = (*_BufferWrapper->PCM_16)[index];
            return (sample)value16;
        }

        case AudioFormat::PCM_32: {
            int32_t value32 = (*_BufferWrapper->PCM_32)[index];
            return (sample)value32;
        }

        case AudioFormat::PCM_Float: {
            float valueFloat = (*_BufferWrapper->PCM_Float)[index];
            return (sample)valueFloat;
        }

        default:
            return 0;
    }
}
