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

#ifdef __CUDACC__

#ifndef PLATFORM_CUDA
  #define PLATFORM_CUDA
#endif

/*!
 * @macro SEQAN_FUNC
 * @headerfile <seqan/platform.h>
 * @brief Prefix for functions
 *
 * @signature #define SEQAN_FUNC
 *
 * This macro can be placed in front of functions that should be used for code that is CUDA-compatible.  The macro
 * expands to <tt>inline</tt> for normal compilers and to <tt>inline __device__ __host__</tt> for CUDA compilers.  These
 * two keywords mark a function to be executed on the GPU as well as on the CPU.
 *
 * @section Example
 *
 * @code{.cpp}
 * SEQAN_FUNC void foo(int & x)
 * {
 *     // I can run on the CPU and on the GPU, yay!
 *     x = 10;
 * }
 * @endcode
 */

#define SEQAN_FUNC inline __host__ __device__
#define SEQAN_HOST_DEVICE __host__ __device__
#define SEQAN_HOST __host__
#define SEQAN_DEVICE __device__
#define SEQAN_GLOBAL __global__

#else

#define SEQAN_FUNC inline
#define SEQAN_HOST_DEVICE
#define SEQAN_HOST
#define SEQAN_DEVICE
#define SEQAN_GLOBAL

#endif
