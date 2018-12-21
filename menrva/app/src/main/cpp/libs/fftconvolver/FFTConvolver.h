// ==================================================================================
// Copyright (c) 2017 HiFi-LoFi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ==================================================================================

#ifndef _FFTCONVOLVER_FFTCONVOLVER_H
#define _FFTCONVOLVER_FFTCONVOLVER_H

#include "AudioFFT.h"
#include "Utilities.h"

#include <vector>


namespace fftconvolver
{ 

/**
* @class FFTConvolver
* @brief Implementation of a partitioned FFT convolution algorithm with uniform block size
*
* Some notes on how to use it:
*
* - After initialization with an impulse response, subsequent data portions of
*   arbitrary length can be convolved. The convolver internally can handle
*   this by using appropriate buffering.
*
* - The convolver works without "latency" (except for the required
*   processing time, of course), i.e. the output always is the convolved
*   input for each processing call.
*
* - The convolver is suitable for real-time processing which means that no
*   "unpredictable" operations like allocations, locking, API calls, etc. are
*   performed during processing (all necessary allocations and preparations take
*   place during initialization).
*/
class FFTConvolver
{  
public:
    FFTConvolver();
    virtual ~FFTConvolver();

    /**
    * @brief Initializes the convolver
    * @param blockSize Block size internally used by the convolver (partition size)
    * @param ir The impulse response
    * @param irLen Length of the impulse response
    * @return true: Success - false: Failed
    */
    bool init(size_t blockSize, const Sample* ir, size_t irLen);

    /**
    * @brief Convolves the the given input samples and immediately outputs the result
    * @param input The input samples
    * @param output The convolution result
    * @param len Number of input/output samples
    */
    void process(const Sample* input, Sample* output, size_t len);

    /**
    * @brief Resets the convolver and discards the set impulse response
    */
    void reset();
  
private:
    /// Block size internally used by the convolver (partition size, is a power of 2)
    size_t _blockSize;

    /// Size of one segment in samples (block size times 2)
    size_t _segSize;

    /// Count of all segments (fitting inside the impulse response)
    size_t _segCount;

    /// Size of real/imagenary part of the segments in the frequency domain
    size_t _fftComplexSize;

    /// Vector of all buffered input segments in the frequency domain
    std::vector<SplitComplex*> _segments;

    /// Vector of all impulse response segments in the frequency domain
    std::vector<SplitComplex*> _segmentsIR;

    /// Sample buffer, holding samples of the frequency domain (sized by segment size)
    SampleBuffer _fftBuffer;

    /// AudioFFT handle
    audiofft::AudioFFT _fft;

    /// Buffer for convolution proposes (sized by segment size)
    SplitComplex _preMultiplied;

    /// Buffer for convolution proposes (sized by segment size)
    SplitComplex _conv;

    /// Sample buffer, holding the overlap (sized by buffer size)
    SampleBuffer _overlap;

    /// Index of the current segment
    size_t _current;

    /// Sample buffer, holding samples of the time domain (sized by block size)
    SampleBuffer _inputBuffer;

    /// Position of the processed input in samples
    size_t _inputBufferFill;

    // Prevent uncontrolled usage
    FFTConvolver(const FFTConvolver&);
    FFTConvolver& operator=(const FFTConvolver&);
};
  
} // End of namespace fftconvolver

#endif // Header guard
