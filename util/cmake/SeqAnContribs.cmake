# ============================================================================
#                  SeqAn - The Library for Sequence Analysis
# ============================================================================
# Copyright (c) 2006-2012, Knut Reinert, FU Berlin
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Knut Reinert or the FU Berlin nor the names of
#       its contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
# ============================================================================
# Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
# ============================================================================
# This CMake file is included by the root CMakeLists.txt to look for
# installed SeqAn contribs on the WIN32 platform.
# ============================================================================

if (WIN32)
    # The contrib version we look for.
	set (_SEQAN_CONTRIB_VERSION D20111031)
	set (_SEQAN_CONTRIB_DIR "seqan-contrib-${_SEQAN_CONTRIB_VERSION}")

	# Try to figure out where the user installed the contrib.  We expect
	# it to be either in C:\, or one of the Program Files dirs.
	#
	# First, look into Program Files on 64 bit.
	if (DEFINED ENV{ProgramW6432})
		if (IS_DIRECTORY "$ENV{ProgramW6432}/${_SEQAN_CONTRIB_DIR}-x64")
			set (SEQAN_CONTRIB_BASE "$ENV{ProgramW6432}/${_SEQAN_CONTRIB_DIR}-x64")
		endif (IS_DIRECTORY "$ENV{ProgramW6432}/${_SEQAN_CONTRIB_DIR}-x64")
	endif (DEFINED ENV{ProgramW6432})
	# Try out Program Files for 32bit Windows.
	if (NOT DEFINED SEQAN_CONTRIB_BASE)
		if (IS_DIRECTORY "$ENV{ProgramFiles}/${_SEQAN_CONTRIB_DIR}-x86")
			set (SEQAN_CONTRIB_BASE "$ENV{ProgramFiles}/${_SEQAN_CONTRIB_DIR}-x86")
		endif (IS_DIRECTORY "$ENV{ProgramFiles}/${_SEQAN_CONTRIB_DIR}-x86")
	endif (NOT DEFINED SEQAN_CONTRIB_BASE)
	# Try out on C:/.
	if (NOT DEFINED SEQAN_CONTRIB_BASE)
		if (IS_DIRECTORY "C:/${_SEQAN_CONTRIB_DIR}-x86")
			set (SEQAN_CONTRIB_BASE "C:/${_SEQAN_CONTRIB_DIR}-x86")
		elseif (IS_DIRECTORY "C:/${_SEQAN_CONTRIB_DIR}-x64")
			set (SEQAN_CONTRIB_BASE "C:/${_SEQAN_CONTRIB_DIR}-x64")
		endif (IS_DIRECTORY "C:/${_SEQAN_CONTRIB_DIR}-x86")
	endif (NOT DEFINED SEQAN_CONTRIB_BASE)

	# Debug help.
    #if (NOT DEFINED SEQAN_CONTRIB_BASE)
    #	message("SEQAN_CONTRIB_BASE is undefined!")
    #else (NOT DEFINED SEQAN_CONTRIB_BASE)
    #	message("SEQAN_CONTRIB_BASE is ${SEQAN_CONTRIB_BASE}")
    #endif (NOT DEFINED SEQAN_CONTRIB_BASE)

	# Try to figure out the generator.
	if (IS_DIRECTORY ${SEQAN_CONTRIB_BASE})
		if (CMAKE_GENERATOR MATCHES "^Visual Studio .*")
			string (REGEX REPLACE "^Visual Studio ([0-9]+).*$" "\\1" SEQAN_CONTRIB_VARIANT ${CMAKE_GENERATOR})
			set (SEQAN_CONTRIB_VARIANT "vs${SEQAN_CONTRIB_VARIANT}")
		elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
			set (SEQAN_CONTRIB_VARIANT mingw)
		endif (CMAKE_GENERATOR MATCHES "^Visual Studio .*")

        #message(STATUS "SEQAN_CONTRIB_BASE    is ${SEQAN_CONTRIB_BASE}")
        #message(STATUS "SEQAN_CONTRIB_VARIANT is ${SEQAN_CONTRIB_VARIANT}")

		# Compose contrib path.
		set(SEQAN_CONTRIB_PATH "${SEQAN_CONTRIB_BASE}/${SEQAN_CONTRIB_VARIANT}")

		# Extend CMAKE_FIND_ROOT_PATH.
		if (IS_DIRECTORY ${SEQAN_CONTRIB_PATH})
			set (CMAKE_FIND_ROOT_PATH ${SEQAN_CONTRIB_PATH} ${CMAKE_FIND_ROOT_PATH})
		endif (IS_DIRECTORY ${SEQAN_CONTRIB_PATH})
	endif (IS_DIRECTORY ${SEQAN_CONTRIB_BASE})

    message(STATUS "CMAKE_FIND_ROOT_PATH is \"${CMAKE_FIND_ROOT_PATH}\".")
endif (WIN32)
