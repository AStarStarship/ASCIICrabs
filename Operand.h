// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsOperandH
#define _CrabsOperandH
#include "Slot.h"
#if _Seam >= _CrabsOp
namespace _ {

struct Crabs;

/* Interface for an abstract A*B Op operand.
A Script Operand is an object that is being operated on.
Let A be a set of states in a Chinese Room state machine.
Let B be a set of bytes in a set of Abstract Serial Ports.

@code
+----------- Result: The set of finite states
| +--------- Op
| | +------- Set of finite state machine states
| | | +----- Star Op
| | | | +--- Operands (input values)
| | | | |
v v v v v
A = A * B
    |<->|   Op
|<----->|   Evaluation
@endcode

When a device is selected by a Slot, the caller can then call functions
of that device. The Slot has a stack of devices that it then pushes the
Star* on top of. This object is now selected, and agents can now call
functions of this object via the Data Controller (DC).

@see Console for example of how to use Operand interface.
*/

struct Operand {
  /* Crabs Operations.
  @param index The index of the expression.
  @param crabs The Crabs to read and write from.
  @return      Returns nil upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const Op* Star(CHC index, Crabs* crabs) = 0;
};

/* Returns the name of the given Operand. */
const CHA* OperandName(Operand* op);

/* Gets the number_ of ops in the given expressions. */
IUW OperandCount(Operand* op);

/* Returns the index of the given Op using the given slot.
@param key_begin Beginning of the key slot socket.
@param key_end   End of the key slot socket.
@return A value printable Unicode CHA or invalid if the Operand doesn't
Contain the given key. */
CHC OperandIndex(Operand* operand, CHA* key_begin, CHA* key_end);

#if _CrabsUsingText
/* Queries the given Operand Op Header.
@param crabs The expression to write the query to. Set to nil to return
              op.
@param op   The Op header.
@return Returns nil upon success and an error Op upon failure.
const Op* OperandQuery (Crabs* crabs, const Op* op);*/

/* Queries the operand for the given ??? @todo fix me */
Slot& OperandQuery(Operand* root, const CHA* address, Slot& key);
#endif

}  //< namespace _
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsOperandH
