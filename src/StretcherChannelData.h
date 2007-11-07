/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    Rubber Band
    An audio time-stretching and pitch-shifting library.
    Copyright 2007 Chris Cannam.
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#ifndef _RUBBERBAND_STRETCHERCHANNELDATA_H_
#define _RUBBERBAND_STRETCHERCHANNELDATA_H_

#include "StretcherImpl.h"

#include <set>

namespace RubberBand
{

class Resampler;
	
class RubberBandStretcher::Impl::ChannelData
{
public:        
    /**
     * Construct a ChannelData structure.
     *
     * The block size passed in here is the size for the FFT
     * calculation, and most of the buffer sizes also depend on
     * it.  In practice it is always a power of two and except for
     * very extreme stretches is always either 1024, 2048 or 4096.
     *
     * The outbuf size depends on other factors as well, including
     * the pitch scale factor and any maximum processing block
     * size specified by the user of the code.
     */
    ChannelData(size_t blockSize, size_t outbufSize);

    /**
     * Construct a ChannelData structure that can process at
     * different FFT sizes without requiring reallocation when the
     * size changes.  The size can subsequently be changed with a
     * call to setBlockSize.  Reallocation will only be necessary
     * if setBlockSize is called with a value not equal to one of
     * those passed in to the constructor.
     *
     * The outbufSize should be the maximum possible outbufSize to
     * avoid reallocation, which will happen if setOutbufSize is
     * called subsequently.
     */
    ChannelData(const std::set<size_t> &blockSizes,
                size_t initialBlockSize, size_t outbufSize);
    ~ChannelData();

    /**
     * Reset buffers
     */
    void reset();

    /**
     * Set the FFT and buffer sizes from the given processing
     * block size.  If this ChannelData was constructed with a set
     * of block sizes and the given block size here was among
     * them, no reallocation will be required.
     */
    void setBlockSize(size_t blockSize);

    /**
     * Set the outbufSize for the channel data.  Reallocation will
     * occur.
     */
    void setOutbufSize(size_t outbufSize);

    RingBuffer<float> *inbuf;
    RingBuffer<float> *outbuf;

    double *mag;
    double *phase;

    double *prevPhase;
    double *unwrappedPhase;

    size_t *freqPeak;

    float *accumulator;
    size_t accumulatorFill;
    float *windowAccumulator;

    float *fltbuf;
    double *dblbuf;

    size_t prevIncrement; // only used in RT mode

    size_t blockCount;
    size_t inCount;
    long inputSize; // set only after known (when data ended); -1 previously
    size_t outCount;

    bool draining;
    bool outputComplete;

    FFT *fft;
    std::map<size_t, FFT *> ffts;

    Resampler *resampler;
    float *resamplebuf;
    size_t resamplebufSize;

private:
    void construct(const std::set<size_t> &blockSizes,
                   size_t initialBlockSize, size_t outbufSize);
};        

}

#endif