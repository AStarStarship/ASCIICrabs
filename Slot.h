// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsSlotH
#define _CrabsSlotH
#include <_Config.h>
#if _Seam >= _CrabsOp
#include "AType.h"
#include "AType.hpp"
namespace _ {

// @defgroup Slot

/* Initializes the ring socket with the given socket origin and size.
@param socket Pointer to the beginning of the ring socket.
@param size   The size of the ring socket in bytes. */
Slot* SlotInit(IUW* socket, ISW size);

/* Initializes the slot from the TBIn. */
Slot* SlotInit(TBIn<ISC>* bin);

/* Initializes the slot from the TBOut. */
Slot* SlotInit(TBOut<ISC>* bout);

/* Sets the ring socket to the given socket origin and size.
@param slot   The slot to set.
@param socket Pointer to the beginning of the ring socket.
@param size   The size of the ring socket in bytes. */
inline BOL SlotSet(Slot* slot, IUW* socket, IUW size);

/* Checks if this slot contains the given address.
@return Returns inputted address if this Slot contains the given address
and nil else wise. */
void* SlotContains(Slot* slot, void* address);

/* Clears the socket without zeroing it out. */
inline void SlotClear(Slot* slot);

/* Zeros out the Slot. */
void SlotWipe(Slot* slot);

/* Checks if there is space in the socket.
@return True if the socket has space. */
BOL SlotIsWritable(Slot* slot);

/* Checks if there is data in the socket.
@return True if the socket has data. */
BOL SlotIsReadable(Slot* slot);

/* Reads the given Operation input parameters from the slot to the args.
@param slot   The slot to read from.
@param params The param types.
@param args   The args array of pointers to write to.
@return Nil upon success and an Error Operation upon failure. */
const Op* SlotRead(Slot* slot, const DTB* params, void** args);

/* Reads the given Operation input parameters from the slot to the args.
@param slot The slot to read from.
@param op   The Operation to get the in from.
@param args The args array of pointers to write to.
@return Nil upon success and an Error Operation upon failure. */
const Op* SlotRead(Slot* slot, const Op& op, void** args);

/* Writes the given Operation output parameters from the slot to the args.
@param slot   The slot to write to.
@param params The param types.
@param args   The args array of pointers to write to.
@return Nil upon success and an Error Operation upon failure. */
const Op* SlotWrite(Slot* slot, const DTB* params, void** args);

/* Writes the given Operation output parameters from the slot to the args.
@param slot The slot to write to.
@param op   The Operation to get the out from.
@param args The args array of pointers to write to.
@return Nil upon success and an Error Operation upon failure. */
const Op* SlotWrite(Slot* slot, const Op& op, void** args);

/* Writes a single message to the slot socket.
@param slot    The slot to write to.
@param message The message to send.
@return Nil upon success and an Error Operation upon failure. */
const Op* SlotWrite(Slot* slot, const IUA* message);

/* Copies the contents of the target slot into the slot. */
const Op* SlotWrite(Slot* slot, Slot& target);

// @} Slot

}  //< namespace _
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsSlotH
