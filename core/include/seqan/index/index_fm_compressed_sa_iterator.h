// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2013, Knut Reinert, FU Berlin
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
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Jochen Singer <jochen.singer@fu-berlin.de>
// ==========================================================================

#ifndef INDEX_FM_COMPRESSED_SA_ITERATOR_H_
#define INDEX_FM_COMPRESSED_SA_ITERATOR_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

template <typename TText, typename TSpec>
struct CompressedSA;

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction Iterator
// ----------------------------------------------------------------------------

template <typename TText, typename TSpec>
struct Iterator<CompressedSA<TText, TSpec> const, Standard>
{
    typedef Iter<CompressedSA<TText, TSpec> const, PositionIterator> Type;
};

template <typename TText, typename TSpec>
struct Iterator<CompressedSA<TText, TSpec>, Standard>
{
    typedef Iter<CompressedSA<TText, TSpec>, PositionIterator> Type;
};

template <typename TText, typename TSpec>
struct Iterator<CompressedSA<TText, TSpec>, Rooted>:
    Iterator<CompressedSA<TText, TSpec>, Standard>{};

template <typename TText, typename TSpec>
struct Iterator<CompressedSA<TText, TSpec> const, Rooted>:
    Iterator<CompressedSA<TText, TSpec> const, Standard>{};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function begin()
// ----------------------------------------------------------------------------

///.Function.begin.param.object.type:Class.CompressedSA
template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec>, Standard>::Type
begin(CompressedSA<TText, TSpec> & compressedSA, Standard const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec>, Standard>::Type(compressedSA, 0);
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec> const, Standard>::Type
begin(CompressedSA<TText, TSpec> const & compressedSA, Standard const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec> const, Standard>::Type(compressedSA, 0);
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec>, Rooted>::Type
begin(CompressedSA<TText, TSpec> & compressedSA, Rooted const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec>, Rooted>::Type(compressedSA, 0);
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec> const, Rooted>::Type
begin(CompressedSA<TText, TSpec> const & compressedSA, Rooted const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec> const, Rooted>::Type(compressedSA, 0);
}

// ----------------------------------------------------------------------------
// Function end()
// ----------------------------------------------------------------------------

///.Function.end.param.object.type:Class.CompressedSA
template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec>, Rooted>::Type
end(CompressedSA<TText, TSpec> & compressedSA, Rooted const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec>, Rooted>::Type(compressedSA, length(compressedSA));
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec> const, Rooted>::Type
end(CompressedSA<TText, TSpec> const & compressedSA, Rooted/* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec> const, Rooted>::Type(compressedSA, length(compressedSA));
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec>, Standard>::Type
end(CompressedSA<TText, TSpec> & compressedSA, Standard const & /* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec>, Standard>::Type(compressedSA, length(compressedSA));
}

template <typename TText, typename TSpec>
inline typename Iterator<CompressedSA<TText, TSpec> const, Standard>::Type
end(CompressedSA<TText, TSpec> const & compressedSA, Standard/* dummy */)
{
    return typename Iterator<CompressedSA<TText, TSpec> const, Standard>::Type(compressedSA, length(compressedSA));
}

}
#endif // INDEX_FM_COMPRESSED_SA_ITERATOR_H_
