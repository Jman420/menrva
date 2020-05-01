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

#include <algorithm>
#include <stdexcept>
#include "AudioInputBuffer.h"
#include "../tools/StringOperations.h"

AudioInputBuffer::AudioInputBuffer(ILogWriter* logger, IConversionBuffer<float, sample>* conversionBuffer)
        : ILogProducer(logger, __PRETTY_FUNCTION__),
          AudioIOBufferBase() {
    _BufferWrapper = new audio_input_buffer_u();
    _BufferWrapper->PCM_Float = conversionBuffer;
    _InputAudioFormat = AudioFormat::Sample;
}

AudioInputBuffer::~AudioInputBuffer() {
    _Logger->WriteLog("Disposing of Audio Input Buffer...", LOG_SENDER, __func__);
    switch (_InputAudioFormat) {
        case AudioFormat::PCM_16:
        case AudioFormat::PCM_32:
        case AudioFormat::PCM_Float:
            delete _BufferWrapper->PCM_Float;
            break;

        default:
            break;
    }

    delete _BufferWrapper;
    _Logger->WriteLog("Successfully disposed of Audio Input Buffer!", LOG_SENDER, __func__);
}

void AudioInputBuffer::ResetData() {
    switch (_InputAudioFormat) {
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
}

void AudioInputBuffer::SetFormat(AudioFormat inputAudioFormat) {
    _Logger->WriteLog(StringOperations::FormatString("Setting Audio Format to (%d)...", inputAudioFormat),
                      LOG_SENDER, __func__);
    if (_InputAudioFormat == inputAudioFormat) {
        _Logger->WriteLog("Audio Format already set.", LOG_SENDER, __func__);
        return;
    }

    switch (inputAudioFormat) {
        case AudioFormat::PCM_16:
        case AudioFormat::PCM_32:
        case AudioFormat::PCM_Float:
            _InputAudioFormat = inputAudioFormat;
            break;

        default:
            std::string msg = "Unable to instantiate Conversion Buffer.  Invalid Audio Format provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    _Logger->WriteLog("Successfully set Audio Format!", LOG_SENDER, __func__);
}

void AudioInputBuffer::SetData(void* data, uint32_t channelLength, size_t sampleLength) {
    size_t bufferLength = sampleLength * channelLength;

    switch (_InputAudioFormat) {
        case AudioFormat::PCM_16:
            _BufferWrapper->PCM_16->SetData(static_cast<int16_t*>(data), bufferLength);
            break;

        case AudioFormat::PCM_32:
            _BufferWrapper->PCM_32->SetData(static_cast<int32_t*>(data), bufferLength);
            break;

        case AudioFormat::PCM_Float:
            _BufferWrapper->PCM_Float->SetData(static_cast<float*>(data), bufferLength);
            break;

        default:
            std::string msg = "Unable to set Data.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    AudioIOBufferBase::SetLength(channelLength, sampleLength);
}

void AudioInputBuffer::SetData(AudioFormat inputAudioFormat, void* data, uint32_t channelLength, size_t sampleLength) {
    SetFormat(inputAudioFormat);
    SetData(data, channelLength, sampleLength);
}

void* AudioInputBuffer::GetData() {
    switch (_InputAudioFormat) {
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

sample AudioInputBuffer::operator()(uint32_t channelIndex, size_t sampleIndex) const {
    _Logger->WriteLog(StringOperations::FormatString("Validating Channel Index (%d) and Sample Index (%d)...", channelIndex, sampleIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    if (channelIndex > _ChannelLength) {
        throw std::runtime_error("Invalid Channel Index Provided.");
    }
    if (sampleIndex > _SampleLength) {
        throw std::runtime_error("Invalid Sample Index Provided.");
    }

    _Logger->WriteLog(StringOperations::FormatString("Calculating Buffer Index for Sample Index (%d) for Channel (%d)...", sampleIndex, channelIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    size_t bufferIndex = AudioIOBufferBase::CalculateBufferIndex(_ChannelLength, channelIndex, sampleIndex);

    _Logger->WriteLog(StringOperations::FormatString("Retrieving Normalized Value for Buffer Index (%d)...", bufferIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    sample normalizedValue;

    switch (_InputAudioFormat) {
        case AudioFormat::PCM_16: {
            int16_t value = (*_BufferWrapper->PCM_16)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Normalizing value (%d) from Audio Format (%d) to sample type...", value, _InputAudioFormat),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            normalizedValue = Normalize<int16_t>(value);
            break;
        }

        case AudioFormat::PCM_32: {
            int32_t value = (*_BufferWrapper->PCM_32)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Normalizing value (%d) from Audio Format (%d) to sample type...", value, _InputAudioFormat),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            normalizedValue = Normalize<int32_t>(value);
            break;
        }

        case AudioFormat::PCM_Float: {
            float value = (*_BufferWrapper->PCM_Float)[bufferIndex];
            _Logger->WriteLog(StringOperations::FormatString("Normalizing value (%f) from Audio Format (%d) to sample type...", value, _InputAudioFormat),
                              LOG_SENDER, __func__, LogLevel::Verbose);
            normalizedValue = static_cast<sample>(value);
            break;
        }

        default:
            std::string msg = "Unable to retrieve Normalized Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    _Logger->WriteLog(StringOperations::FormatString("Successfully retrieved Normalized Value (%f) for Buffer Index (%d)!", normalizedValue, bufferIndex),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    return normalizedValue;
}

template<class TInputType>
sample AudioInputBuffer::Normalize(TInputType data) const {
    _Logger->WriteLog(StringOperations::FormatString("Normalizing value for AudioFormat (%d)...", _InputAudioFormat),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    const auto dataValue = (sample)data;

    sample conversionScalar;

    switch (_InputAudioFormat) {
        case AudioFormat::PCM_16:
            conversionScalar = PCM16_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_32:
            conversionScalar = PCM32_FLOAT_SCALAR;
            break;

        case AudioFormat::PCM_Float:
            _Logger->WriteLog("Normalization not necessary for PCM Float Audio Format.", LOG_SENDER, __func__, LogLevel::Verbose);
            return dataValue;

        default:
            std::string msg = "Unable to Normalize Value.  Invalid Audio Format Provided.";
            _Logger->WriteLog(msg, LOG_SENDER, __func__, LogLevel::Fatal);
            throw std::runtime_error(msg);
    }

    sample normalizedValue = dataValue / conversionScalar;
    normalizedValue = std::max(std::min(normalizedValue, conversionScalar), -conversionScalar);
    _Logger->WriteLog(StringOperations::FormatString("Successfully normalized value to (%f).", normalizedValue),
                      LOG_SENDER, __func__, LogLevel::Verbose);
    return normalizedValue;
}

template sample AudioInputBuffer::Normalize<int16_t>(int16_t data) const;
template sample AudioInputBuffer::Normalize<int32_t>(int32_t data) const;
