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

#include "AudioOutputBuffer.h"
#include "../tools/Buffer.cpp"
#include "../tools/StringOperations.h"

AudioOutputBuffer::AudioOutputBuffer(LogWriterBase* logger)
        : LogProducer(logger, __PRETTY_FUNCTION__),
          AudioIOBufferBase() {
    _BufferWrapper = new audio_output_buffer_u();
    _OutputAudioFormat = AudioFormat::Sample;
}

AudioOutputBuffer::AudioOutputBuffer(LogWriterBase* logger, AudioFormat audioFormat)
        : LogProducer(logger, __PRETTY_FUNCTION__),
          AudioIOBufferBase() {
    _BufferWrapper = new audio_output_buffer_u();
    _OutputAudioFormat = AudioFormat::Sample;
    SetFormat(audioFormat);
}

AudioOutputBuffer::~AudioOutputBuffer() {
    _Logger->WriteLog("Disposing of Audio Output Buffer...", LOG_SENDER, __func__);
    switch (_OutputAudioFormat) {
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

void AudioOutputBuffer::ResetData() {
    switch (_OutputAudioFormat) {
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

    AudioIOBufferBase::SetData(0, 0);
}

void AudioOutputBuffer::Free() {
    switch (_OutputAudioFormat) {
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

void AudioOutputBuffer::SetFormat(AudioFormat outputAudioFormat) {
    _Logger->WriteLog(StringOperations::FormatString("Setting Audio Format to (%d)...", outputAudioFormat),
                      LOG_SENDER, __func__);
    if (_OutputAudioFormat == outputAudioFormat) {
        _Logger->WriteLog("Audio Format already set.", LOG_SENDER, __func__);
        return;
    }
    _OutputAudioFormat = outputAudioFormat;

    _Logger->WriteLog(StringOperations::FormatString("Instantiating Buffer for Audio Format (%d)...", _OutputAudioFormat),
                      LOG_SENDER, __func__);
    switch (_OutputAudioFormat) {
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
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }

    AudioIOBufferBase::SetData(channelLength, sampleLength);
}

void AudioOutputBuffer::SetData(AudioFormat outputAudioFormat, void* data, uint32_t channelLength, size_t sampleLength) {
    SetFormat(outputAudioFormat);
    SetData(data, channelLength, sampleLength);
}

void AudioOutputBuffer::SetValue(uint32_t channelIndex, size_t sampleIndex, sample value) {
    _Logger->WriteLog("Validating Parameters...", LOG_SENDER, __func__, LogLevel::VERBOSE);
    if (channelIndex > _ChannelLength) {
        throw std::runtime_error("Invalid Channel Index Provided.");
    }
    if (sampleIndex > _SampleLength) {
        throw std::runtime_error("Invalid Sample Index Provided.");
    }

    _Logger->WriteLog(StringOperations::FormatString("Calculating Buffer Index for Sample Index (%d) for Channel (%d)...", sampleIndex, channelIndex),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    size_t bufferIndex = AudioIOBufferBase::CalculateBufferIndex(_ChannelLength, channelIndex, sampleIndex);

    _Logger->WriteLog(StringOperations::FormatString("Setting Normalized Value to Index (%d) from Original Value (%f)...", bufferIndex, value),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16: {
            auto normalizedValue = Normalize<int16_t>(value);
            (*_BufferWrapper->PCM_16)[bufferIndex] = normalizedValue;
            _Logger->WriteLog(StringOperations::FormatString("Successfully set Normalized Value (%d) to Index (%d)!", normalizedValue, sampleIndex),
                              LOG_SENDER, __func__, LogLevel::VERBOSE);
            break;
        }

        case AudioFormat::PCM_32: {
            auto normalizedValue = Normalize<int32_t>(value);
            (*_BufferWrapper->PCM_32)[bufferIndex] = normalizedValue;
            _Logger->WriteLog(StringOperations::FormatString("Successfully set Normalized Value (%d) to Index (%d)!", normalizedValue, sampleIndex),
                              LOG_SENDER, __func__, LogLevel::VERBOSE);
            break;
        }

        case AudioFormat::PCM_Float:
            (*_BufferWrapper->PCM_Float)[bufferIndex] = (float)value;
            break;

        default:
            std::string msg = "Unable to set Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::ERROR);
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
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::FATAL);
            throw std::runtime_error(msg);
    }
}

void* AudioOutputBuffer::operator()(uint32_t channelIndex, size_t sampleIndex) const {
    _Logger->WriteLog("Validating Parameters...", LOG_SENDER, __func__, LogLevel::VERBOSE);
    if (channelIndex > _ChannelLength) {
        throw std::runtime_error("Invalid Channel Index Provided.");
    }
    if (sampleIndex > _SampleLength) {
        throw std::runtime_error("Invalid Sample Index Provided.");
    }

    _Logger->WriteLog(StringOperations::FormatString("Calculating Buffer Index for Sample Index (%d) for Channel (%d)...", sampleIndex, channelIndex),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    size_t bufferIndex = sampleIndex + channelIndex;

    _Logger->WriteLog(StringOperations::FormatString("Retrieving Value of Index (%d)...", bufferIndex),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    switch (_OutputAudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t* value = &(*_BufferWrapper->PCM_16)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%d) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::VERBOSE);
            return value;
        }

        case AudioFormat::PCM_32: {
            int32_t* value = &(*_BufferWrapper->PCM_32)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%d) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::VERBOSE);
            return value;
        }

        case AudioFormat::PCM_Float: {
            float* value = &(*_BufferWrapper->PCM_Float)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Value (%f) for Index (%d)!", *value, bufferIndex),
                              LOG_SENDER, __func__, LogLevel::VERBOSE);
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
    _Logger->WriteLog(StringOperations::FormatString("Normalizing value for AudioFormat (%d)...", _OutputAudioFormat),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    sample conversionScalar = 0;

    switch (_OutputAudioFormat) {
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

    sample convertedData = data * conversionScalar;
    auto normalizedValue = (TOutputType)convertedData;
    if (normalizedValue > conversionScalar) {
        normalizedValue = (TOutputType)conversionScalar;
    }
    else if (normalizedValue < -conversionScalar) {
        normalizedValue = (TOutputType)(-conversionScalar);
    }

    _Logger->WriteLog(StringOperations::FormatString("Successfully normalized value to (%d).", normalizedValue),
                      LOG_SENDER, __func__, LogLevel::VERBOSE);
    return normalizedValue;
}

template int16_t AudioOutputBuffer::Normalize<int16_t>(sample data);
template int32_t AudioOutputBuffer::Normalize<int32_t>(sample data);
