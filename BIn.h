// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBInH
#define _CrabsBInH
#include <_Config.h>
#include "AType.hpp"
#if _Seam >= _CrabsOp
namespace _ {

/* @defgroup TBIn B-Input Slot
B-Input ring socket: the consumer side of a Chinese Room Door slot.
Reads type-tagged messages from a shared lock-free SPSC ring.
The struct lives in AType.hpp (TBin template).
@ingroup Slot
@{ */

/* List of TBIn States.
@see SCRIPT Specification RFC. */
typedef enum BInStates {
  BInStateAddress = 0,    //< State  0: Scanning address.
  BInStatePackedArgs,     //< State  1: Scanning arguments.
  BInStatePackedUTF8,     //< State  2: Scanning UTF-8.
  BInStatePackedUTF16,    //< State  3: Scanning UTF-16.
  BInStatePackedUTF32,    //< State  4: Scanning UTF-32.
  BInStatePackedVarint,   //< State  5: Scanning varint.
  BInStatePackedObj,      //< State  6: 8-bit OB1 state.
  BInStateVerifyingHash,  //< State  7: Verifying the 32-bit hash.
  BInStateHandlingError,  //< State  8: Handling an error state.
  BInStateDisconnected,   //< State  9: Disconnected state.
  BInStateAck,            //< State 10: Awaiting connection ACK.
  BInStateLocked,         //< State 11: Locked state.
  BInStatePackedPOD,      //< State 12: Scanning plain-old-data.
  BInStateInvalid         //< Number of TBIn States.
} BInState;

/* Get's the B-Input's socket. */
inline const IUA* BInOrigin(const TBIn<ISC>* bin);
inline IUA* BInOrigin(TBIn<ISC>* bin);

inline IUA* BInEnd(TBIn<ISC>* bin);

/* Calculates the used ring socket space.
@param Start The origin of the data.
@param Stop  The stop of the data.
@param Size  The size of the socket. */
inline ISW SlotLength(IUA* origin, IUA* stop, IUW size);

/* Calculates the space left in the given ring socket.
@param Start The origin of the data.
@param Stop  The stop of the data.
@param Size  The size of the socket. */
inline ISW SlotSpace(IUA* origin, IUA* stop, IUW size);

/* Gets the rx socket length. */
inline ISN BInSpace(TBIn<ISC>* bin);

inline ISN BinBufferLength(TBIn<ISC>* bin);

/* Pulls the keyboard input into the ring socket. */
// void BInKeyboard()

/* Initializes the TBIn struct to an empty socket. */
TBIn<ISC>* BInInit(TBIn<ISC>* socket, ISN size);

/* Gets the stop address of the rx socket. */
IUA* BInEnd(TBIn<ISC>* bin);

/* Returns true if the TBIn socket contains any data.
    @warning Function does not do any error checking for speed. */
BOL BInIsReadable(TBIn<ISC>* bin);

/* Scans a message with the given params to the given TBIn.
The data in the TBIn is word-aligned, unlike the Slot. It also
doesn't have a hash with an escape sequence.

@param rx     The TBIn socket.
@param params The parameters.
@param args   The arguments.
@return       Returns 0 upon success and an ErrorList ticket number upon
              failure. */
const Op* BInRead(TBIn<ISC>* bin, const DTB* params, void** args, IUD 
                             pc_ctx);

}  //< namespace _

/* @} */  // End of TBIn group.
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsBInH
