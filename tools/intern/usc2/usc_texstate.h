/*****************************************************************************
 * Name         : usc_texstate.h
 * Title        : Texture state macros used internally by USC and by DX
 * Created      : April 2005
 *
 * Copyright      : 2011 by Imagination Technologies Ltd.
 *                All rights reserved. No part of this software, either
 *                material or conceptual may be copied or distributed,
 *                transmitted, transcribed, stored in a retrieval system or
 *                translated into any human or computer language in any form
 *                by any means, electronic, mechanical, manual or otherwise,
 *                or disclosed to third parties without the express written
 *                permission of Imagination Technologies Ltd,
 *                Home Park Estate, Kings Langley, Hertfordshire,
 *                WD4 8LZ, U.K.
 *
 * Modifications :-
 * $Log: usc_texstate.h $
 *****************************************************************************/

#ifndef USC_TEXSTATE_H
#define USC_TEXSTATE_H

#include "sgxdefs.h"

/*
	Offset in the texture state constant buffer from the base of the state for each sampler of different kinds of state.
*/
#define UNIFLEX_TEXSTATE_HWSTATE_START						(0)
#define _UNIFLEX_TEXSTATE_HWSTATE(CHUNK, STATEWORD, HWSTATESIZE)	(UNIFLEX_TEXSTATE_HWSTATE_START + ((CHUNK) * (HWSTATESIZE)) + (STATEWORD))
#define _UNIFLEX_TEXSTATE_HWSTATE_END(HWSTATESIZE)			(UNIFLEX_TEXSTATE_HWSTATE_START + UF_MAX_CHUNKS_PER_TEXTURE * (HWSTATESIZE))
#define _UNIFLEX_TEXSTATE_STRIDE_START(HWSTATESIZE)			_UNIFLEX_TEXSTATE_HWSTATE_END(HWSTATESIZE)
#define _UNIFLEX_TEXSTATE_STRIDE(CHUNK, HWSTATESIZE)		(_UNIFLEX_TEXSTATE_STRIDE_START(HWSTATESIZE) + CHUNK)
#define _UNIFLEX_TEXSTATE_STRIDE_END(HWSTATESIZE)			(_UNIFLEX_TEXSTATE_STRIDE_START(HWSTATESIZE) + UF_MAX_CHUNKS_PER_TEXTURE)
#define _UNIFLEX_TEXSTATE_MAXARRAYINDEX(HWSTATESIZE)		_UNIFLEX_TEXSTATE_STRIDE_END(HWSTATESIZE)
#define _UNIFLEX_TEXSTATE_WIDTH(HWSTATESIZE)				(_UNIFLEX_TEXSTATE_MAXARRAYINDEX(HWSTATESIZE) + 1)
#define _UNIFLEX_TEXSTATE_HEIGHT(HWSTATESIZE)				(_UNIFLEX_TEXSTATE_WIDTH(HWSTATESIZE) + 1)
#define _UNIFLEX_TEXSTATE_DEPTH(HWSTATESIZE)				(_UNIFLEX_TEXSTATE_HEIGHT(HWSTATESIZE) + 1)

/*
	Size of the state for each sampler.
*/
#define _UNIFLEX_TEXSTATE_STATESIZE(HWSTATESIZE)			(_UNIFLEX_TEXSTATE_DEPTH(HWSTATESIZE) + 1)

#define UNIFLEX_TEXSTATE_HWSTATE(CHUNK, STATEWORD)			_UNIFLEX_TEXSTATE_HWSTATE(CHUNK, STATEWORD, EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_HWSTATE_END						_UNIFLEX_TEXSTATE_HWSTATE_END(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_STRIDE_START						_UNIFLEX_TEXSTATE_STRIDE_START(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_STRIDE(CHUNK)						_UNIFLEX_TEXSTATE_STRIDE(CHUNK, EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_STRIDE_END							_UNIFLEX_TEXSTATE_STRIDE_END(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_MAXARRAYINDEX						_UNIFLEX_TEXSTATE_MAXARRAYINDEX(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_WIDTH								_UNIFLEX_TEXSTATE_WIDTH(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_HEIGHT								_UNIFLEX_TEXSTATE_HEIGHT(EURASIA_TAG_TEXTURE_STATE_SIZE)
#define UNIFLEX_TEXSTATE_DEPTH								_UNIFLEX_TEXSTATE_DEPTH(EURASIA_TAG_TEXTURE_STATE_SIZE)

#define UNIFLEX_TEXSTATE_STATESIZE							_UNIFLEX_TEXSTATE_STATESIZE(EURASIA_TAG_TEXTURE_STATE_SIZE)

/*
	Offset from the overall start of the texture state to the start of the state for a particular sampler.
*/
#define _UNIFLEX_TEXSTATE_SAMPLERBASE(SAMP, HWSTATESIZE)	((SAMP) * _UNIFLEX_TEXSTATE_STATESIZE(HWSTATESIZE))
#define UNIFLEX_TEXSTATE_SAMPLERBASE(SAMP)					_UNIFLEX_TEXSTATE_SAMPLERBASE(SAMP, EURASIA_TAG_TEXTURE_STATE_SIZE)

#define _UNIFLEX_TEXSTATE_GETSAMPLER(OFF, HWSTATESIZE)		((OFF) / _UNIFLEX_TEXSTATE_STATESIZE(HWSTATESIZE))
#define UNIFLEX_TEXSTATE_GETSAMPLER(OFF)					_UNIFLEX_TEXSTATE_GETSAMPLER(OFF, EURASIA_TAG_TEXTURE_STATE_SIZE)

#define _UNIFLEX_TEXSTATE_GESTATETYPE(OFF, HWSTATESIZE)		((OFF) % _UNIFLEX_TEXSTATE_STATESIZE(HWSTATESIZE))
#define UNIFLEX_TEXSTATE_GETSTATETYPE(OFF)					_UNIFLEX_TEXSTATE_GETSTATETTYPE(OFF, EURASIA_TAG_TEXTURE_STATE_SIZE)

#endif /* USC_TEXSTATE_H */
