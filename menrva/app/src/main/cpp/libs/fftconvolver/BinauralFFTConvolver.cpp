//
// Created by anyere on 18.09.17.
//

#include "BinauralFFTConvolver.h"

#include <cmath>

namespace fftconvolver
{

    BinauralFFTConvolver::BinauralFFTConvolver() :
            _blockSize(0),
            _segSize(0),
            _segCount(0),
            _fftComplexSize(0),
            _segments(),
            _segmentsLeftIR(),
            _fftBuffer(),
            _fftBufferR(),
            _fft(),
            _preMultipliedL(),
            _preMultipliedR(),
            _convL(),
            _convR(),
            _overlapL(),
            _overlapR(),
            _current(0),
            _inputBuffer(),
            _inputBufferFill(0)
    {
    }


    BinauralFFTConvolver::~BinauralFFTConvolver()
    {
        reset();
    }

    void BinauralFFTConvolver::reset() {
        reset(true);
    }

    void BinauralFFTConvolver::reset(bool inputReset)
    {
        if (inputReset) {
            for (size_t i=0; i<_segCount; ++i)
            {
                delete _segments[i];
                delete _segmentsLeftIR[i];
                delete _segmentsRightIR[i];
            }

            _blockSize = 0;
            _segSize = 0;
            _segCount = 0;
            _fftComplexSize = 0;
            _segments.clear();
            _fftBuffer.clear();
            _fftBufferR.clear();
            _fft.init(0);
            _overlapR.clear();
            _overlapL.clear();
            _current = 0;
            _inputBuffer.clear();
            _inputBufferFill = 0;
        } else {
            for (size_t i=0; i<_segCount; ++i)
            {
                delete _segmentsLeftIR[i];
                delete _segmentsRightIR[i];
            }
        }

        _segmentsLeftIR.clear();
        _segmentsRightIR.clear();
        _preMultipliedL.clear();
        _preMultipliedR.clear();
        _convL.clear();
        _convR.clear();
    }


    bool BinauralFFTConvolver::init(size_t blockSize, const Sample* irL, const Sample* irR, size_t irLen)
    {
        if (blockSize == 0)
        {
            return false;
        }

        if (irLen == 0)
        {
            reset();
            return true;
        }

        blockSize = NextPowerOf2(blockSize);
        const bool inputReset = _blockSize != blockSize;
        reset(inputReset);

        if (inputReset) {
            _blockSize = blockSize;
            _segSize = 2 * _blockSize;
            _segCount = static_cast<size_t>(::ceil(static_cast<float>(irLen) / static_cast<float>(_blockSize)));
            _fftComplexSize = audiofft::AudioFFT::ComplexSize(_segSize);

            // FFT
            _fft.init(_segSize);
            _fftBuffer.resize(_segSize);
            _fftBufferR.resize(_segSize);

            // Prepare segments
            for (size_t i=0; i<_segCount; ++i)
            {
                _segments.push_back(new SplitComplex(_fftComplexSize));
            }

            // Prepare input buffer
            _inputBuffer.resize(_blockSize);
            _inputBufferFill = 0;

            // Reset current position
            _current = 0;
        }

        // Prepare IR's
        for (size_t i=0; i<_segCount; ++i)
        {
            const size_t remaining = irLen - (i * _blockSize);
            const size_t sizeCopy = (remaining >= _blockSize) ? _blockSize : remaining;

            // left
            SplitComplex* segmentL = new SplitComplex(_fftComplexSize);
            CopyAndPad(_fftBuffer, &irL[i*_blockSize], sizeCopy);
            _fft.fft(_fftBuffer.data(), segmentL->re(), segmentL->im());
            _segmentsLeftIR.push_back(segmentL);

            // right
            SplitComplex* segmentR = new SplitComplex(_fftComplexSize);
            CopyAndPad(_fftBufferR, &irR[i*_blockSize], sizeCopy);
            _fft.fft(_fftBufferR.data(), segmentR->re(), segmentR->im());
            _segmentsRightIR.push_back(segmentR);
        }

        // Prepare convolution buffers
        _preMultipliedL.resize(_fftComplexSize);
        _preMultipliedR.resize(_fftComplexSize);
        _convL.resize(_fftComplexSize);
        _convR.resize(_fftComplexSize);
        _overlapL.resize(_blockSize);
        _overlapR.resize(_blockSize);

        return true;
    }

    void BinauralFFTConvolver::process(const Sample* input, Sample* outputL, Sample* outputR, size_t len)
    {
        if (_segCount == 0)
        {
            ::memset(outputL, 0, len * sizeof(Sample));
            ::memset(outputR, 0, len * sizeof(Sample));
            return;
        }

        size_t processed = 0;
        while (processed < len)
        {
            // Input buffer full => Next block
            if (_inputBufferFill == _blockSize)
            {
                // Save the left overlap
                ::memcpy(_overlapL.data(), _fftBuffer.data()+_blockSize, _blockSize * sizeof(Sample));
                // Save the right overlap
                ::memcpy(_overlapR.data(), _fftBufferR.data()+_blockSize, _blockSize * sizeof(Sample));

                // Input buffer is empty again now
                _inputBuffer.setZero();
                _inputBufferFill = 0;

                // Update current segment
                _current = (_current > 0) ? (_current - 1) : (_segCount - 1);
            }

            const bool inputBufferWasEmpty = (_inputBufferFill == 0);
            const size_t processing = std::min(len-processed, _blockSize-_inputBufferFill);
            const size_t inputBufferPos = _inputBufferFill;
            ::memcpy(_inputBuffer.data()+inputBufferPos, input+processed, processing * sizeof(Sample));

            // Forward FFT
            CopyAndPad(_fftBuffer, &_inputBuffer[0], _blockSize);
            _fft.fft(_fftBuffer.data(), _segments[_current]->re(), _segments[_current]->im());

            // Complex multiplication
            if (inputBufferWasEmpty)
            {
                _preMultipliedL.setZero();
                _preMultipliedR.setZero();
                // ignore the first index, its used below
                for (size_t i=1; i<_segCount; ++i)
                {
                    const size_t indexIr = i;
                    const size_t indexAudio = (_current + i) % _segCount;
                    ComplexMultiplyAccumulate(_preMultipliedL, *_segmentsLeftIR[indexIr], *_segments[indexAudio]);
                    ComplexMultiplyAccumulate(_preMultipliedR, *_segmentsRightIR[indexIr], *_segments[indexAudio]);
                }
            }
            _convL.copyFrom(_preMultipliedL);
            _convR.copyFrom(_preMultipliedR);
            ComplexMultiplyAccumulate(_convL, *_segments[_current], *_segmentsLeftIR[0]);
            ComplexMultiplyAccumulate(_convR, *_segments[_current], *_segmentsRightIR[0]);


            // Left backward FFT
            _fft.ifft(_fftBuffer.data(), _convL.re(), _convL.im());
            // Left add overlap
            Sum(outputL+processed, _fftBuffer.data()+inputBufferPos, _overlapL.data()+inputBufferPos, processing);

            // Right backward FFT
            _fft.ifft(_fftBufferR.data(), _convR.re(), _convR.im());
            // Right add overlap
            Sum(outputR+processed, _fftBufferR.data()+inputBufferPos, _overlapR.data()+inputBufferPos, processing);

            _inputBufferFill += processing;
            processed += processing;
        }
    }

    void BinauralFFTConvolver::process(Sample* outputL, Sample* outputR, size_t len)
    {
        if (_segCount == 0)
        {
            ::memset(outputL, 0, len * sizeof(Sample));
            ::memset(outputR, 0, len * sizeof(Sample));
            return;
        }

        size_t processed = 0;
        while (processed < len)
        {
            const size_t processing = std::min(len-processed, _blockSize);

            // Complex multiplication
            if (processed == 0)
            {
                _preMultipliedL.setZero();
                _preMultipliedR.setZero();
                // ignore the first index, its used below
                for (size_t i=1; i<_segCount; ++i)
                {
                    const size_t indexIr = i;
                    const size_t indexAudio = (_current + i) % _segCount;
                    ComplexMultiplyAccumulate(_preMultipliedL, *_segmentsLeftIR[indexIr], *_segments[indexAudio]);
                    ComplexMultiplyAccumulate(_preMultipliedR, *_segmentsRightIR[indexIr], *_segments[indexAudio]);
                }
            }
            _convL.copyFrom(_preMultipliedL);
            _convR.copyFrom(_preMultipliedR);
            ComplexMultiplyAccumulate(_convL, *_segmentsLeftIR[0], *_segments[_current]);
            ComplexMultiplyAccumulate(_convR, *_segmentsRightIR[0], *_segments[_current]);


            // Left backward FFT
            _fft.ifft(_fftBuffer.data(), _convL.re(), _convL.im());
            // Left add overlap
            Sum(outputL+processed, _fftBuffer.data()+processed, _overlapL.data()+processed, processing);

            // Right backward FFT
            _fft.ifft(_fftBufferR.data(), _convR.re(), _convR.im());
            // Right add overlap
            Sum(outputR+processed, _fftBufferR.data()+processed, _overlapR.data()+processed, processing);

            processed += processing;
        }
    }

} // End of namespace fftconvolver
