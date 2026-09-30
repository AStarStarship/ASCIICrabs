// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_DOOR_DECL
#define CRABS_DOOR_DECL
#include <_Config.h>
#if SEAM >= CRABS_ROOM
#include "Crabs.h"
#include "BIn.h"
#include "BigInt.h"
#include "Slot.h"
#include "Stack.hpp"
#include "String.hpp"
namespace _ {

/* A door that connects two Chinese rooms.
A door can lead to one or more Rooms. A Door that can
be locked, in which case all of the Rooms behind this door are locked
out. When a door is locked, no messages are processed or sent to or
from the door and its subsidiary agents.

Rooms can be connected by one or more Portal. An example of a
system with one Door and multiple Portals is a WiFi plus USB/Serial
connection. This is a commonly used configuration for debugging the system.

Multiple doors also might share the same Portal. An example of this is a
connection to multiple systems over a WiFi connection.

@code

+=============+
|    slots    |
|=============|
| Door struct |
+=============+

@endcode
*/
template<typename ISZ>
class TDoor : public Operand {
 public:
  enum {
    DoorCount = 1,          //< Initial (or static) Door count.
    SlotSizeDefault = 512,  //< Default.
    BytesMin = 128,         //< Min size of the door socket in bytes.
  };

  /* A door in a Chinese room. */
  TDoor(const CHA* roomName = NILP, IUW* socket = NILP,
        IUW bytes = BytesMin) {
    if (IsError(socket)) {
      if (bytes < BytesMin) {
        bytes = BytesMin;
      }
    } else {
      if (bytes < BytesMin) {
        // @todo insert error code here
        D_COUT("\nError: Door bytes < DoorBytesMin!");
        return;
      }
    }
    // tx.SetBoofer (adjacentDoor->Rx ()->EndAddress () + 1), aSlotSize);
    // rx = new SerialSlot (tx.EndAddress (), aSlot, aSlotSize,
    //  aTalkbackSize);
  }

  /* RAMFactory. */
  //virtual ~TDoor() {
  //  if (origin) delete[] origin;
  //}

  /* Gets the BOut at the given index. */
  BOut* GetSlot(ISZ index) { return slots_->Element(index); }

  /* Address the given crabs to the Door. */
  ISZ AddSlot(ISZ slot) { return TStackInsert<ISZ, ISZ, ISZ>(slots_, slot); }

  /* Attempts to find a Slot or Door with the given address. */
  BOL Contains(void* address) {
    return TStackContains<ISZ, ISZ, ISZ>(slots_, address);
  }

  /* Gets the Slot that contains the given address.
  @return Returns the doors_ stack count if the Door does not contain the given
  address. */
  ISZ FindSlot(void* address) {
    ISZ count = slots_->count;
    for (ISZ i = 0; i < count; ++i) {
      // Slot* slot = NILP; //< @todo fix me!

      if (TStackContains<ISZ, ISZ, ISZ>(slots_, address)) return i;
    }
    return count;
  }

  BIn* SlotNumber(ISZ index) {
    if (!slots_.InBounds(index)) return NILP;
    return slots_[index];
  }

  /* Executes all of the queued escape sequences.
  @return Nil upon success or an Error Op upon failure. */
  const Op* Exec(Crabs* crabs) {
    TStack<Slot, ISZ, ISZ>* slots = slots_;
    ISZ scan_total = scan_total_;
    for (ISZ i = 0; i < slots->count; ++i) {
      Slot* slot = slots->Element(i);
      BIn* bin = TPtr<BIn>(slot);
      for (ISZ count = scan_total; count > 0; --count) {
        ISN value = BInStreamByte(bin);
        if (value < 0) break;
        //const Op* result = crabs->SScan(value);
      }
    }
    return NILP;
  }

  /* Crabs operations. */
  virtual const Op* Star(CHC index, Crabs* crabs) {
    static const Op This = {
      "Door",
      OpFirst('A'),
      OpFirst(ASizeCodef(slots_->count)),
      "A door in a Chinese room with a bunch of slots in it where "
      "messages are passed.",
      '}',
      ';',
      ' ',
      false,
      NILP,
      NILP
    };
    if (index == '?') {
      return CrabsQuery(crabs, This, 0);
    }
    index -= ' ';
    if (ISZ(index) >= slots_->count) {
      //return DoorResult(this, Door::ErrorInvalidOp);
      return NILP;
    }
    return NILP;
  }

 private:
  ISN  bytes_,          //< Door size in bytes.
       scan_total_;     //< Max bytes to pull through the slot at once.
  IUW* begin_;          //< Pointer to dynamic socket.
  TStack<Slot, ISZ, ISZ>* slots_;  //< Slots in the door.

  BIn* OffsetToBIn(ISZ offset) {
    return TPtr<BIn>(IUW(this) + offset);
  }
};

/* Initializes a Door at the beginning of the given socket.
static Door* DoorInit (ISZ* socket, ISZ slot_size) {
  if (socket == NILP) return NILP;
  if (slot_size < kMinSlotSize) return NILP;
  Wall* wall = TPtr<Door>(socket);
  w->is_dynamic = 0;
  w->num_doors = 0;
  w->max_num_doors;
  w->door_one = NILP;
}*/

}  //< namespace _
#endif
#endif
