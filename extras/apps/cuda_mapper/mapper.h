// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2013 NVIDIA Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of NVIDIA Corporation nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Enrico Siragusa <enrico.siragusa@fu-berlin.de>
// ==========================================================================

#ifndef SEQAN_EXTRAS_CUDAMAPPER_MAPPER_H_
#define SEQAN_EXTRAS_CUDAMAPPER_MAPPER_H_

// ============================================================================
// Prerequisites
// ============================================================================

#include "index.h"

using namespace seqan;

// ============================================================================
// Tags
// ============================================================================

struct CPU_;
typedef Tag<CPU_>     CPU;

// ============================================================================
// Classes
// ============================================================================

// ----------------------------------------------------------------------------
// Class HitsCounter
// ----------------------------------------------------------------------------

template <typename TSpec = void>
struct HitsCounter
{
//    typedef typename Delegated<TFinder>::Type   TDelegated;

    unsigned long hits;

    HitsCounter() :
        hits(0)
    {}

    template <typename TFinder>
    SEQAN_FUNC void
    operator() (TFinder const & /* finder */)
    {
//        SEQAN_OMP_PRAGMA(atomic)
//        hits += countOccurrences(finder._pool[omp_get_thread_num()]._textIt);
    }
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function mapReads()                                                    [CPU]
// ----------------------------------------------------------------------------

template <typename TIndex, typename TReadSeqs>
inline void
mapReads(TIndex & index, TReadSeqs & readSeqs, CPU const & /* tag */)
{
//    typedef Finder2<TIndex, TReadSeqs, Multiple<FinderSTree> >  TFinder;
//
//    // Instantiate a multiple finder.
//    TFinder finder(index);
//
//    // Count hits.
//    HitsCounter<> counter;
//    find(finder, readSeqs, counter);
//    std::cout << "Hits count:\t\t\t" << counter.hits << std::endl;
}


#endif  // #ifndef SEQAN_EXTRAS_CUDAMAPPER_MAPPER_H_
