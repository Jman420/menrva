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

#include <stdexcept>
#include "AudioOutputBuffer.h"
#include "../tools/StringOperations.h"

AudioOutputBuffer::AudioOutputBuffer(ILogWriter* logger, IBuffer<float>* buffer)
        : ILogProducer(logger, __PRETTY_FUNCTION__),
          AudioIOBufferBase() {
    _BufferWrapper = new audio_output_buffer_u();
    _BufferWrapper->PCM_Float = buffer;
    _OutputAudioFormat = AudioFormat::Sample;
}

AudioOutputBuffer::~AudioOutputBuffer() {
    _Logger->WriteLog("Disposing of Audio Output Buffer...", LOG_SENDER, __func__);
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
        case AudioFormat::PCM_32:
        case AudioFormat::PCM_Float:
            delete _BufferWrapper->PCM_Float;
            break;

        default:
            break;
    }

    delete _BufferWrapper;
    _Logger->WriteLog("Successfully disposed of Audio Output Buffer!", LOG_SENDER, __func__);
}

void AudioOutputBuffer::ResetData() {
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
        case AudioFormat::PCM_32:
        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->ResetData();
            break;

        default:
            std::string msg = "Unable to reset data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    AudioIOBufferBase::SetLength(0, 0);
}

void AudioOutputBuffer::SetFormat(AudioFormat outputAudioFormat) {
    _Logger->WriteLog(StringOperations::FormatString("Setting Audio Format to (%d)...", outputAudioFormat),
                      LOG_SENDER, __func__);
    if (_OutputAudioFormat == outputAudioFormat) {
        _Logger->WriteLog("Audio Format already set.", LOG_SENDER, __func__);
        return;
    }
    
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
        case AudioFormat::PCM_32:
        case AudioFormat::PCM_Float:
            _OutputAudioFormat = outputAudioFormat;
            break;

        default:
            std::string msg = "Unable to instantiate Conversion Buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    _Logger->WriteLog("Successfully set Audio Format!", LOG_SENDER, __func__);
}

void AudioOutputBuffer::SetData(void* data, uint32_t channelLength, size_t sampleLength) {
    size_t bufferLength = sampleLength * channelLength;

    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16->SetData((int16_t*)data, bufferLength);
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32->SetData((int32_t*)data, bufferLength);
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->SetData((float*)data, bufferLength);
            break;

        default:
            std::string msg = "Unable to set Data.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    AudioIOBufferBase::SetLength(channelLength, sampleLength);
}

void AudioOutputBuffer::SetData(AudioFormat outputAudioFormat, void* data, uint32_t channelLength, size_t sampleLength) {
    SetFormat(outputAudioFormat);
    SetData(data, channelLength, sampleLength);
}

void AudioOutputBuffer::SetValue(uint32_t channelIndex, size_t sampleIndex, sample value) {
    _Logger->WriteLog("Validating Parameters...", LOG_SENDER, __func__, LogLevel::Verbose);
    if (channelIndex > _ChannelLength) {
        throw std::runtime_error("Invalid Channel Index Provided.");
    }
    if (sampleIndex > _SampleLength) {
        throw std::runtime_error("Invalid Sample Index Provided.");
    }

    _Logger->WriteLog(StringOperations::FormatString("Calculating Buffer Index for Sample Index (%d) for Channel (%d)...", sampleIndex, channelIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    size_t bufferIndex = AudioIOBufferBase::CalculateBufferIndex(_ChannelLength, channelIndex, sampleIndex);

    _Logger->WriteLog(StringOperations::FormatString("Setting Normalized Value to Index (%d) from Original Value (%f)...", bufferIndex, value),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16: {
            auto normalizedValue = Normalize<int16_t>(value);
            (*_BufferWrapper->PCM_16)[bufferIndex] = normalizedValue;
            _Logger->WriteLog(StringOperations::FormatString("Successfully set Normalized Value (%d) to Index (%d)!", normalizedValue, sampleIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            break;
        }

        case AudioFormat::PCM_32: {
            auto normalizedValue = Normalize<int32_t>(value);
            (*_BufferWrapper->PCM_32)[bufferIndex] = normalizedValue;
            _Logger->WriteLog(StringOperations::FormatString("Successfully set Normalized Value (%d) to Index (%d)!", normalizedValue, sampleIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            break;
        }

        case AudioFormat::PCM_Float:
            (*_BufferWrapper->PCM_Float)[bufferIndex] = (float)value;
            break;

        default:
            std::string msg = "Unable to set Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Error);
            throw std::runtime_error(msg);
    }
}

void* AudioOutputBuffer::GetData() {
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
            return _BufferWrapper->PCM_16->GetData();

        case AudioFormat::PCM_32:
            return _BufferWrapper->PCM_32->GetData();

        case AudioFormat::PCM_Float:
            return _BufferWrapper->PCM_Float->GetData();

        default:
            std::string msg = "Unable to return data.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }
}

void* AudioOutputBuffer::operator()(uint32_t channelIndex, size_t sampleIndex) const {
    _Logger->WriteLog("Validating Parameters...", LOG_SENDER, __func__, LogLevel::Verbose);
    if (channelIndex > _ChannelLength) {
        throw std::runtime_error("Invalid Channel Index Provided.");
    }
    if (sampleIndex > _SampleLength) {
        throw std::runtime_error("Invalid Sample Index Provided.");
    }

    _Logger->WriteLog(StringOperations::FormatString("Calculating Buffer Index for Sample Index (%d) for Channel (%d)...", sampleIndex, channelIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    size_t bufferIndex = sampleIndex + channelIndex;

    _Logger->WriteLog(StringOperations::FormatString("Retrieving Value of Index (%d)...", bufferIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t* value = &(*_BufferWrapper->PCM_16)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%d) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            return value;
        }

        case AudioFormat::PCM_32: {
            int32_t* value = &(*_BufferWrapper->PCM_32)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%d) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            return value;
        }

        case AudioFormat::PCM_Float: {
            float* value = &(*_BufferWrapper->PCM_Float)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%f) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            return value;
        }

        default:
            std::string msg = "Unable to retrieve Normalized Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }
}

template<class TOutputType>
TOutputType AudioOutputBuffer::Normalize(sample data) {
    _Logger->WriteLog(StringOperations::FormatString("Normalizing value for AudioFormat (%d)...", _OutputAudioFormat),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    sample conversionScalar = 0;

    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16:
            conversionScalar = PCM16_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_32:
            conversionScalar = PCM32_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_Float:
            _Logger->WriteLog("Normalization not necessary for PCM Float Audio Format.", LOG_SENDER, __func__, LogLevel::Verbose);
            return (TOutputType)data;

        default:
            std::string msg = "Unable to Normalize Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    sample convertedData = data * conversionScalar;
    auto normalizedValue = (TOutputType)convertedData;
    if (normalizedValue > conversionScalar) {
        normalizedValue = (TOutputType)conversionScalar;
    }
    else if (normalizedValue < -conversionScalar) {
        normalizedValue = (TOutputType)(-conversionScalar);
    }

    _Logger->WriteLog(StringOperations::FormatString("Successfully normalized value to (%d).", normalizedValue),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    return normalizedValue;
}

template int16_t AudioOutputBuffer::Normalize<int16_t>(sample data);
template int32_t AudioOutputBuffer::Normalize<int32_t>(sample data);
