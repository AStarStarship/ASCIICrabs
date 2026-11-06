// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsMirrorH
#define _CrabsMirrorH
#include <_Config.h>
#if _Seam >= _CrabsOp
#include "BIn.h"
#include "BOut.h"
#include "Op.h"
#include "AType.hpp"
namespace _ {

/* @ingroup Mirror
@brief A Mirror is a portal to this system for debugging purpsoes.
A Mirror is still a Door, it just has a diffeent Star function.
*/

const Op* MirrorResult(Mirror* mirror, AError error);

enum {
  MirrorHeaderSize = 4 * sizeof(ISC)  //< Size of a Mirror Header.
};

static const IUW MirrorSizeMax = ~((IUW)0);

/* Calculates the used ring socket space.
@param Start The origin of the data.
@param Stop  The stop of the data.
@param Size The size of the socket. */
ISC MirrorLength(CHA* origin, CHA* stop, ISC size);

/* Calculates the space left in the given ring socket.
@param Start The origin of the data.
@param Stop  The stop of the data.
@param Size  The size of the socket. */
ISC MirrorSpace(CHA* origin, CHA* stop, ISC size);

/* Checks if the given slot contains the specified address. */
void* MirrorContains(Mirror* mirror, void* address);

/* Clears the ring socket by writing zeros to it. */
void MirrorWipe(Mirror* mirror);

/* Copies a block from a ring-socket to the given destination. */
// CHA* MirrorWrite (Mirror* mirror, void* source, ISW size);

/* Copies a block from a ring-socket to the given destination. */
// CHA* MirrorRead (Mirror* mirror, void* source, ISW size);

/* Gets the args from the b-sequence. */
const Op* MirrorRead(Mirror* mirror, const ISC* params, void** args);

/* Posts a evaluated result to the b-sequence. */
// const Op* MirrorWrite (Mirror* mirror, const ISC* params,
//                                   void** args);

BOL MirrorIsReadable(Mirror* mirror);

/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
const Op* MirrorResult(Mirror* mirror, AError error, const DTB* header);
/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
const Op* MirrorResult(Mirror* mirror, AError error, const DTB* header);

/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
const Op* MirrorResult(Mirror* mirror, AError error, const DTB* header,
                       IUA offset);

/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
// const Op* MirrorResult (Mirror* mirror, Error error,
//                                   const DTB* header,
//                                   ISC offset);

/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
const Op* MirrorResult(Mirror* mirror, AError error, const DTB* header,
                       ISC offset, CHA* address);

/* Used to return an erroneous result from a B-Input.
@param bin     The source Mirror.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Operation Result. */
const Op* MirrorResult(Mirror* mirror, AError error, const DTB* header,
                       ISC offset, CHA* address);

}  //< namespace _
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsMirrorH
