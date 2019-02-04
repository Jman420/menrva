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

#include "audio_output_buffer.h"
#include "../tools/buffer.cpp"

AudioOutputBuffer::AudioOutputBuffer(LoggerBase* logger)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _BufferWrapper = new audio_output_buffer_u();
    _AudioFormat = AudioFormat::Sample;
}

AudioOutputBuffer::AudioOutputBuffer(LoggerBase* logger, AudioFormat audioFormat)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _BufferWrapper = new audio_output_buffer_u();
    _AudioFormat = AudioFormat::Sample;
    SetFormat(audioFormat);
}

AudioOutputBuffer::~AudioOutputBuffer() {
    _Logger->WriteLog("Disposing of Audio Output Buffer...", LOG_SENDER, __func__);
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            delete _BufferWrapper->PCM_16;
            break;

        case AudioFormat::PCM_32:
            delete _BufferWrapper->PCM_32;
            break;

        case AudioFormat::PCM_Float:
            delete _BufferWrapper->PCM_Float;
            break;

        default:
            break;
    }

    delete _BufferWrapper;
    _Logger->WriteLog("Successfully disposed of Audio Output Buffer!", LOG_SENDER, __func__);
}

size_t AudioOutputBuffer::GetLength() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            return _BufferWrapper->PCM_16->GetLength();

        case AudioFormat::PCM_32:
            return _BufferWrapper->PCM_32->GetLength();

        case AudioFormat::PCM_Float:
            return _BufferWrapper->PCM_Float->GetLength();

        default:
            std::string msg = "Unable to retrieve length.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioOutputBuffer::ResetData() {
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
            std::string msg = "Unable to reset data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioOutputBuffer::Free() {
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
            std::string msg = "Unable to free buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioOutputBuffer::SetFormat(AudioFormat audioFormat) {
    _Logger->WriteLog("Setting Audio Format to (%d)...", LOG_SENDER, __func__, audioFormat);
    if (_AudioFormat == audioFormat) {
        _Logger->WriteLog("Audio Format already set.", LOG_SENDER, __func__);
        return;
    }
    _AudioFormat = audioFormat;

    _Logger->WriteLog("Instantiating Buffer for Audio Format (%d)...", LOG_SENDER, __func__, _AudioFormat);
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16 = new Buffer<int16_t>();
            _Logger->WriteLog("Successfully instantiated PCM16 Buffer!", LOG_SENDER, __func__);
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32 = new Buffer<int32_t>();
            _Logger->WriteLog("Successfully instantiated PCM32 Buffer!", LOG_SENDER, __func__);
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float = new Buffer<float>();
            _Logger->WriteLog("Successfully instantiated PCM Float Buffer!", LOG_SENDER, __func__);
            break;

        default:
            std::string msg = "Unable to instantiate Conversion Buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioOutputBuffer::SetData(void* data, size_t length) {
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
            std::string msg = "Unable to set Data.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioOutputBuffer::SetData(AudioFormat audioFormat, void* data, size_t length) {
    SetFormat(audioFormat);
    SetData(data, length);
}

void AudioOutputBuffer::SetValue(size_t index, sample value) {
    _Logger->WriteLog("Setting Normalized Value to Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, index);
    switch (_AudioFormat) {
        case AudioFormat::PCM_16: {
            auto normalizedValue = Normalize<int16_t>(value);
            (*_BufferWrapper->PCM_16)[index] = normalizedValue;
            _Logger->WriteLog("Successfully set Normalized Value (%d) to Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, normalizedValue, index);
            break;
        }

        case AudioFormat::PCM_32: {
            auto normalizedValue = Normalize<int32_t>(value);
            (*_BufferWrapper->PCM_32)[index] = normalizedValue;
            _Logger->WriteLog("Successfully set Normalized Value (%d) to Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, normalizedValue, index);
            break;
        }

        case AudioFormat::PCM_Float:
            (*_BufferWrapper->PCM_Float)[index] = (float)value;
            break;

        default:
            std::string msg = "Unable to set Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::ERROR);
            throw std::runtime_error(msg);
    }
}

void* AudioOutputBuffer::GetData() {
    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            return _BufferWrapper->PCM_16->GetData();

        case AudioFormat::PCM_32:
            return _BufferWrapper->PCM_32->GetData();

        case AudioFormat::PCM_Float:
            return _BufferWrapper->PCM_Float->GetData();

        default:
            std::string msg = "Unable to return data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void* AudioOutputBuffer::operator[](size_t index) const {
    _Logger->WriteLog("Retrieving Value of Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, index);
    switch (_AudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t* value = &(*_BufferWrapper->PCM_16)[index];
            _Logger->WriteLog("Successfully retrieved Value (%d) for Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, *value, index);
            return value;
        }

        case AudioFormat::PCM_32: {
            int32_t* value = &(*_BufferWrapper->PCM_32)[index];
            _Logger->WriteLog("Successfully retrieved Value (%d) for Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, *value, index);
            return value;
        }

        case AudioFormat::PCM_Float: {
            float* value = &(*_BufferWrapper->PCM_Float)[index];
            _Logger->WriteLog("Successfully retrieved Value (%f) for Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, *value, index);
            return value;
        }

        default:
            std::string msg = "Unable to retrieve Normalized Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

template<class TOutputType>
TOutputType AudioOutputBuffer::Normalize(sample data) {
    _Logger->WriteLog("Normalizing value for AudioFormat (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, _AudioFormat);
    sample conversionScalar;

    switch (_AudioFormat) {
        case AudioFormat::PCM_16:
            conversionScalar = PCM16_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_32:
            conversionScalar = PCM32_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_Float:
            _Logger->WriteLog("Normalization not necessary for PCM Float Audio Format.", LOG_SENDER, __func__, LogLevel::VERBOSE);
            return (TOutputType)data;

        default:
            std::string msg = "Unable to Normalize Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }

    auto normalizedValue = (TOutputType)(data * conversionScalar);
    normalizedValue = std::max(std::min(normalizedValue, conversionScalar), -conversionScalar);
    _Logger->WriteLog("Successfully normalized value to (%d).", LOG_SENDER, __func__, LogLevel::VERBOSE, normalizedValue);
    return normalizedValue;
}

template int16_t AudioOutputBuffer::Normalize<int16_t>(sample data);
template int32_t AudioOutputBuffer::Normalize<int32_t>(sample data);
