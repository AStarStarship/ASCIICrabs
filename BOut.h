// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBOutH
#define _CrabsBOutH
#define CRABS_BOUT_DECL
#include <_Config.h>
#include "AType.hpp"
#if _Seam >= _CrabsOp
namespace _ {

/* @defgroup TBOut B-Output Slot
B-Output ring socket: the producer side of a Chinese Room Door slot.
Writes type-tagged messages into a shared lock-free SPSC ring.
The struct lives in AType.hpp (TBOut template).
@ingroup Slot
@{ */

/* List of the Finite TBOut States. */
typedef enum BOutStates {
  BOutStateDisconnected = 0,  //< TBOut State 0: TBOut locked.
  BOutStateWriting,           //< TBOut State 1: Most common state.
  BOutStateWritingBSeq,       //< TBOut State 2: Writing _BSQ.
} BOutState;

/* Used to return an erroneous result from a B-Output.
@param bout       The source TBOut.
@param error      The error type.
@param header     The B-Sequence Header.
@param bsq_error  The offset to the type in error in the B-Sequence.
@param error_byte The address of the IUA in error.
@return Returns a Static Error Op Result. */
inline const Op* BOutError(TBOut<ISC>* bout, ERC error, const DTB* header,
  ISN bsq_error, IUA* error_byte);

/* Gets a a CHA for printing out the bout_state. */
const CHA** BOutStateStrings();

/* Initializes the B-Output socket with the given socket size. */
TBOut<ISC>* BOutInit(TBOut<ISC>* socket, ISN size);

/* Calculates the space left in the given ring socket.
    @param bout The B-Output socket. */
ISN BOutSpace(TBOut<ISC>* bout);

/* Gets the B-Output. */
ISN BOutBufferLength(TBOut<ISC>* bout);

/* Gets the stop address of the tx socket. */
IUA* BOutEndAddress(TBOut<ISC>* bout);

/* Streams a B-Output IUA.
    @param bout A B-Output abstract IUA stream. */
ISN BOutStreamByte(TBOut<ISC>* bout);

/* The first byte in the B-Output's socket.*/
IUA* BOutBuffer(TBOut<ISC>* bout);

/* Gets the begin of the socket. */
inline const CHA* BOutBegin(const TBOut<ISC>* bout);
inline CHA* BOutBegin(TBOut<ISC>* bout);

/* Used to return an erroneous result from a B-Output.
@param error The error type.
@return Returns a Static Error Op Result. */
inline const Op* BOutError(TBOut<ISC>* bout, ERC error);

/* Used to return an erroneous result from a B-Output.
@param bout    The source TBOut.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Op Result. */
inline const Op* BOutError(TBOut<ISC>* bout, ERC error, const DTB* header);

/* Used to return an erroneous result from a B-Output.
@param bout    The source TBOut.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Op Result. */
inline const Op* BOutError(TBOut<ISC>* bout, ERC error, const DTB* header,
  ISN offset);

/* Writes a message with the given params to the given B-Output slot.
@param bout   The B-Output socket to write to.
@param params The escape sequence to write.
@param args   The array of pointers to the stuff to write.
@param pc_ctx The context mapping for the write operation. */
const Op* BOutWrite(TBOut<ISC>* bout, const DTB* params, void** args, 
  IUD pc_ctx);

/* Sends a connection message to the given address. */
const Op* BOutConnect(TBOut<ISC>* bout, const CHA* address, IUD pc_ctx);

/* Sends a connection message to the given address. */
void BOutRingBell(TBOut<ISC>* bout, const CHA* address);

/* Sends a connection acknowledgment to the given address. */
void BOutAckBack(TBOut<ISC>* bout, const CHA* address);

/* Sends a connection termination message to the given address. */
void BOutTerminate(TBOut<ISC>* bout, const CHA* address);

/* Sends a connection reset message to the given address. */
void BOutReset(TBOut<ISC>* bout, const CHA* address);

/* Links a TBOut (producer) and a TBIn (consumer) so they share one ring.
This is the single-producer/single-consumer lock-free pipe: the TBOut's
write-head (bout->stop) and the TBIn's read-head (bin->origin) become the two
independent volatile counters over one shared byte region. The TBIn's
write-head mirror (bin->stop) and the TBOut's read-head mirror (bout->origin)
alias the producer/consumer heads respectively so each side sees the other's
progress. After calling this, BOutWrite(bout, ...) into the producer and
BInRead(bin, ...) from the consumer exchange messages through the shared ring.
@param bin   The consumer TBIn (Room B's input).
@param bout  The producer TBOut (Room A's output).
@param buffer The shared byte region both structs will use (must be at least
              SlotBytesMin bytes). This is the memory that, in a real
              interprocess pipe, would live in shared memory.
@return Returns the consumer TBIn upon success.
@see TBIn.h, TBOut.h, Slot.h (the Chinese Room Slot/Door/pipe). */
TBIn<ISC>* BInAttach(TBIn<ISC>* bin, TBOut<ISC>* bout, IUA* buffer, ISN buffer_bytes);

}  //< namespace _

/* @} */  // End of TBOut group.
#endif
#endif  //< _CrabsBOutH