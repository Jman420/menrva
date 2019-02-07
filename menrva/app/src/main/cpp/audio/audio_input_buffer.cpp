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
    _AudioFormat = AudioFormat::Sample;
}

AudioInputBuffer::AudioInputBuffer(LoggerBase* logger, AudioFormat audioFormat)
        : LoggingBase(logger, __PRETTY_FUNCTION__) {
    _BufferWrapper = new audio_input_buffer_u();
    _AudioFormat = AudioFormat::Sample;
    SetFormat(audioFormat);
}

AudioInputBuffer::~AudioInputBuffer() {
    _Logger->WriteLog("Disposing of Audio Input Buffer...", LOG_SENDER, __func__);
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
    _Logger->WriteLog("Successfully disposed of Audio Input Buffer!", LOG_SENDER, __func__);
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
            std::string msg = "Unable to retrieve length.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
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
            std::string msg = "Unable to reset data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
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
            std::string msg = "Unable to free buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void AudioInputBuffer::SetFormat(AudioFormat audioFormat) {
    _Logger->WriteLog("Setting Audio Format to (%d)...", LOG_SENDER, __func__, audioFormat);
    if (_AudioFormat == audioFormat) {
        _Logger->WriteLog("Audio Format already set.", LOG_SENDER, __func__);
        return;
    }

    _Logger->WriteLog("Instantiating Conversion Buffer for Audio Format (%d)...", LOG_SENDER, __func__, audioFormat);
    switch (audioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16 = new ConversionBuffer<int16_t, sample>();
            _Logger->WriteLog("Successfully instantiated PCM16 Conversion Buffer!", LOG_SENDER, __func__);
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32 = new ConversionBuffer<int32_t, sample>();
            _Logger->WriteLog("Successfully instantiated PCM32 Conversion Buffer!", LOG_SENDER, __func__);
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float = new ConversionBuffer<float, sample>();
            _Logger->WriteLog("Successfully instantiated PCM Float Conversion Buffer!", LOG_SENDER, __func__);
            break;

        default:
            std::string msg = "Unable to instantiate Conversion Buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }

    _AudioFormat = audioFormat;
    _Logger->WriteLog("Successfully set Audio Format!", LOG_SENDER, __func__);
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
            std::string msg = "Unable to set Data.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
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
            std::string msg = "Unable to return data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

sample AudioInputBuffer::operator[](size_t index) const {
    _Logger->WriteLog("Retrieving Normalized Value of Index (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, index);
    sample normalizedValue;

    switch (_AudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t value = (*_BufferWrapper->PCM_16)[index];
            normalizedValue = Normalize<int16_t>(value);
            break;
        }

        case AudioFormat::PCM_32: {
            int32_t value = (*_BufferWrapper->PCM_32)[index];
            normalizedValue = Normalize<int32_t>(value);
            break;
        }

        case AudioFormat::PCM_Float: {
            float value = (*_BufferWrapper->PCM_Float)[index];
            normalizedValue = (sample)value;
            break;
        }

        default:
            std::string msg = "Unable to retrieve Normalized Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }

    _Logger->WriteLog("Successfully retrieved Normalized Value (%f) for Index (%d)!", LOG_SENDER, __func__, LogLevel::VERBOSE, normalizedValue, index);
    return normalizedValue;
}

template<class TInputType>
sample AudioInputBuffer::Normalize(TInputType data) const {
    _Logger->WriteLog("Normalizing value for AudioFormat (%d)...", LOG_SENDER, __func__, LogLevel::VERBOSE, _AudioFormat);
    const auto dataValue = (sample)data;

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
            return dataValue;

        default:
            std::string msg = "Unable to Normalize Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }

    sample normalizedValue = dataValue / conversionScalar;
    normalizedValue = std::max(std::min(normalizedValue, conversionScalar), -conversionScalar);
    _Logger->WriteLog("Successfully normalized value to (%f).", LOG_SENDER, __func__, LogLevel::VERBOSE, normalizedValue);
    return normalizedValue;
}

template sample AudioInputBuffer::Normalize<int16_t>(int16_t data) const;
template sample AudioInputBuffer::Normalize<int32_t>(int32_t data) const;
