// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsCrabsH
#define _CrabsCrabsH
#include "Op.h"
#include "Operand.h"
#include "AType.hpp"
#if _Seam >= _CrabsOp
namespace _ {

/* A full-duplex Crabs EXP (Expression) interpreter.
Crabs' (crabs(s)) must be word-aligned in order to run correctly
so it's best to scan and word align the data types in the same sweep.

Crabs' are composed of one B-Input (TBIn) and one B-Output (TBOut) socket. The
App/Driver/User writes to the stop of the Tx socket and the driver reads from
the beginning. The user writes are synchronous and the driver reads are
asynchronous.

# Ring Buffer Streaming Diagram

@code
      |>---------------------- Ring Buffer ------------------------->|
      ________________________________________________________________
TBOut  |>-Buffer->|>-Sync User SScan->|>-Async Portal Rx-->|>-Buffer->|
      |__________|___________________|____________________|__________|
      ________________________________________________________________
TBIn   |>-Buffer->|>-Async Portal Tx->|>-Sync User Writes->|>-Buffer->|
      |__________|___________________|____________________|__________|
@endcode

Almost all block of memory in Script has something that grows up and another
that grows down.

## Stack Memory Layout

@code
0x0 +-------------------------+
 |  |      Crabs struct       |
 V  |-------------------------|
(+) |   Stack of Operand**    |
    |-------------------------|
    |          Slot           |
    |-------------------------|
    |         Buffer          |
    |-------------------------|
    |     Evaluated Result    |
0xN +-------------------------+
@endcode
*/
struct alignas(ACPUCacheLineSize) Crabs {

  enum {
    StackTotalMin  = 8, //< Size of the crabs stack.
    BufferTotalMin = 2, //< Min socket size.
  };

  ISC socket_bytes,     //< Offset to the TBOut slot.
      header_bytes,     //< The total size of the header in bytes.
      stack_count,      //< Stack Operand count.
      stack_total,      //< Max stack count.
      type,             //< Current type being scanned.
      num_states,       //< Number of states on the state stack.
      bytes_left,       //< Countdown counter for parsing POD types.
      params_left;      //< Height of header and cursors stacks.
  TMC epoch;            //< The Unix timestamp.
  IUA bout_state,       //< TBOut streaming state.
      bin_state,        //< Slot streaming state.
      last_bin_state,   //< Last TBIn state.
      last_byte;        //< Last byte read.
  CHC current_char;     //< Current Unicode CHA being scanned.
  IUB hash;             //< Packed BSQ hash.
  IUC timeout_us;       //< Timeout time in microseconds.
  ISD last_time;        //< Last time the Stack was scanned.
  IUD ctxt;             //< Current Plain Context Types map word.
  const Op* result;     //< Result of the EXR.
  const DTB* header,    //< Pointer to the header being verified.
      * header_start;   //< Start of the header being verified.
  Operand* operand,     //< Current Script Operand.
      * root;           //< Root-level scope Operand.
  Slot* args;           //< Arguments slot for running.
  Slot slot;            //< Slot for unpacking B-Sequences to.
};

Crabs* StdCrabs();

/*  */
inline ISC CrabsHeaderBytes(ISC stack_total);

/* Gets the crabs's socket. */
IUA* CrabsBuffer(Crabs* crabs);

/* Gets a pointer to the TBIn slot. */
TBIn<ISC>* CrabsBIn(Crabs* crabs);

/* Gets a pointer to the TBOut slot. */
TBOut<ISC>* CrabsBOut(Crabs* crabs);

/* Creates a Stack with equal sized rx and tx slots.
@param root The root-scope device.
@param unpacked_buffer The word-aligned expression socket.
@param unpacked_size   Size of the unpacked socket. */
Crabs* CrabsInit(IUW* socket, ISC buffer_size, ISC stack_count,
                            Operand* root, IUW* unpacked_buffer,
                            IUW unpacked_size, IUD ctxt);

/* Gets the base address of the device stack. */
inline Operand** CrabsStack(Crabs* crabs) {
  return TRef<Operand*>(&crabs->root);
}

/* Returns true if the Stack uses dynamic memory. */
// BOL CrabsIsDynamic (Crabs* crabs);

IUA* CrabsEndAddress(Crabs* crabs);

/* Resets this Stack to the new state. */
const Op* CrabsReset(Crabs* crabs);

/* Pushes the operand at the given index of the current device control onto the stack.
@return Returns nil upon success and a pointer to a CHA
upon failure. */
const Op* Push(Crabs* crabs, Operand* operand);

/* Attempts to pop an Star off the stack and returns a pointer to a CHA upon failure. */
const Op* Pop(Crabs* crabs);

/* Exits the current state. */
IUA CrabsExitState(Crabs* crabs);

/* Sets the new state onto the expression stack.
const Op* CrabsSetState (Crabs* crabs, BInState state); */

/* Saves the current bin_state and sets the bin_state to the new state. */
const Op* CrabsEnterState(Crabs* crabs, BInState state);

/* Streams a B-Output IUA. */
IUA CrabsStreamBOut(Crabs* crabs);

/* Scans the TBIn socket and marks the data as being ready to execute.
@param a The Stack to scan. */
const Op* CrabsScanBIn(Crabs* crabs);  // , Portal* io);

/* Returns true if the given Stack contains the given address. */
BOL CrabsContains(Crabs* crabs, void* address);

/* Pushes a header onto the scan stack.*/
const Op* CrabsScanHeader(Crabs* crabs, const DTB* header);

/* Gets the base address of the header stack. */
const ISC* CrabsHeaderStack(Crabs* crabs);

/* Closes the current crabs and cues it for execution. */
void CrabsClose(Crabs* crabs);

/* Cancels the current crabs. */
void CrabsCancel(Crabs* crabs);

/* Cancels the current crabs and writes zeros to the socket. */
void CrabsClear(Crabs* crabs);

/* Script Bell Op rings the bell of the given address. */
void CrabsRingBell(Crabs* crabs, const CHA* address = "");

/* Script Ack-back Op replies an ACK to a Bell Op. */
void CrabsAckBack(Crabs* crabs, const CHA* address = "");

/* Disconnects the expression. */
const Op* CrabsForceDisconnect(Crabs* crabs, ERC error);

/* Pops the args off the Crabs Args Stack. */
inline const Op* CrabsArgs(Crabs* crabs, const DTB* params, void** args);

/* Writes the result to the Crabs.
@param crabs The resulting expression.
@param op   The Operation with result B-Sequence header.
@param args Pointers to the B-Sequence args. */
inline const Op* CrabsResult(Crabs* crabs, const Op& op, void** args,
    IUD pc_ctx);
//    inline const Op* CrabsResult(Crabs * crabs, const Op & op, void** args,
//        IUD pc_ctx) {

/* Writes the result to the Crabs.
@return NIL upon  @todo something or other, I don't know. WTF is this?
@param crabs The resulting expression.
@param op   The Operation with result B-Sequence header.
@param args Pointers to the B-Sequence args. */
inline const Op* CrabsResult(Crabs* crabs, const Op* op, void** args,
    IUD pc_ctx);

//inline const Op* CrabsResult(Crabs* crabs, const Op* op, void** args,
//    IUD pc_ctx) {
//
/* Writes the result to the Crabs.
@param crabs The resulting expression.
@param op   The Operation with result B-Sequence header.
@param args Pointers to the B-Sequence args. */
inline const Op* CrabsResult(Crabs* crabs, const DTB* params, void** args,
    IUD pc_ctx);

/* Returns the Operand header or writes it to the Crabs.
@param crabs  The expression to write the Op header to.
@param header The Op header.
@return Returns the header if crabs is nil. */
const Op* CrabsQuery(Crabs* crabs, const Op& header, IUD pc_ctx);

/* Returns the Op header or writes it to the Crabs.
@param crabs The expression to write the Op header to.
@param op    The Op header.
@return Returns the header if crabs is nil. */
const Op* CrabsQuery(Crabs* crabs, const Op* op, IUD pc_ctx);

/* Creates a new promise context.
@param crabs The Crabs machine.
@param timestamp Unix epoch (TMC) for when promise was created.
@return Returns a pointer to the newly created promise Slot, or nil on error. */
Slot* CrabsCreatePromise(Crabs* crabs, TMC timestamp);

/* Resolves a promise with a value.
@param crabs The Crabs machine.
@param promise_slot The promise Slot to resolve.
@param value Pointer to the result value.
@return Returns nil upon success or an Error Op upon failure. */
const Op* CrabsResolvePromise(Crabs* crabs, Slot* promise_slot, void* value);

/* Rejects a promise with an error reason.
@param crabs The Crabs machine.
@param promise_slot The promise Slot to reject.
@param error Pointer to the error reason.
@return Returns nil upon success or an Error Op upon failure. */
const Op* CrabsRejectPromise(Crabs* crabs, Slot* promise_slot, void* error);

/* Attaches a fulfillment handler to a promise (.then()).
@param crabs The Crabs machine.
@param promise_slot The promise Slot.
@param onFulfilledKey The key for the fulfillment callback operand.
@return Returns nil upon success or an Error Op upon failure. */
const Op* CrabsThenPromise(Crabs* crabs, Slot* promise_slot, CHA* onFulfilledKey);

/* Attaches a rejection handler to a promise (.catch()).
@param crabs The Crabs machine.
@param promise_slot The promise Slot.
@param onRejectedKey The key for the rejection callback operand.
@return Returns nil upon success or an Error Op upon failure. */
const Op* CrabsCatchPromise(Crabs* crabs, Slot* promise_slot, CHA* onRejectedKey);

/* Reads a promise from a TBIn socket.
@param bin The TBIn socket to read from.
@return Returns the promise state word (0=pending, 1=fulfilled, 2=rejected). */
IUW CrabsReadPromiseState(TBIn<ISC>* bin);

/* Writes a promise result to a TBOut socket.
@param bout The TBOut socket to write to.
@param promise_slot The promise Slot containing the result.
@return Returns nil upon success or an Error Op upon failure. */
const Op* CrabsWritePromiseResult(TBOut<ISC>* bout, Slot* promise_slot);

}  //< namespace _

#endif  //< #if _Seam >= _CrabsCrabs
#endif  //< _CrabsCrabsH
