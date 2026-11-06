// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsPromiseH
#define _CrabsPromiseH
#include <_Config.h>
#include "AType.hpp"
#if _Seam >= _CrabsOp
namespace _ {

struct Crabs;
struct Op;
struct Slot;

// @module Promise
enum {
  PromisePending = 0,
  PromiseFulfilled = 1,
  PromiseRejected = 2
};
IUW PromiseState(Slot* slot);
BOL PromiseIsPending(Slot* slot);
BOL PromiseIsFulfilled(Slot* slot);
BOL PromiseIsRejected(Slot* slot);
TMC PromiseTimestamp(Slot* slot);
const Op* PromiseSetResult(Slot* slot, IUW new_state, void* result, const DTB* header);
const Op* PromiseTriggerFulfillment(Crabs* crabs, Slot* promise_slot);
const Op* PromiseTriggerRejection(Crabs* crabs, Slot* promise_slot);
// @endmodule

}  //< namespace _

#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsPromiseH
