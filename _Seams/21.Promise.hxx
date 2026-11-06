// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsPromise
#include "../Promise.h"
#if _Seam == _CrabsPromise
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

static const CHA* Promise(const CHA* args) {
#if _Seam == _CrabsPromise
  _ATestBegin;
  
  enum {
    BufferBytes = 512,
    BufferWords = BufferBytes / sizeof(IUW),
    StackTotal = 8,
  };
  
  _DROut("\nTesting Promise API...\n");
  
  IUW buffer[BufferWords],
      unpackedExpr[BufferWords];
  IUD pcCtx = 0;
  
  StarTest starTest;
  Crabs* crabs = CrabsInit(buffer, BufferBytes, StackTotal, &starTest,
                           unpackedExpr, BufferBytes, pcCtx);
  
  TMC now = crabs->epoch;
  Slot* promiseSlot = CrabsCreatePromise(crabs, now);
  
  if (!promiseSlot) {
    _DROut("Error: Failed to create promise slot");
    return TRef<const CHA>(AErrorNil);
  }
  
  _DROut("\nPromise created at timestamp:" << now);
  _DROut("Promise state (should be 0=pending):" << PromiseState(promiseSlot));
  _AAvow(IUW(PromisePending), PromiseState(promiseSlot));
  
  IUW resultValue = 42;
  const Op* op = CrabsResolvePromise(crabs, promiseSlot, &resultValue);
  if (op) {
    _DROut("Error: Failed to resolve promise");
    return _NilP; //<  return op; //< ???
  }
  
  _DROut("\nPromise resolved with value:" << resultValue);
  _DROut("Promise state (should be 1=fulfilled):" << PromiseState(promiseSlot));
  _AAvow(IUW(PromiseFulfilled), PromiseState(promiseSlot));
  
  TMC resolutionTime = PromiseTimestamp(promiseSlot);
  _DROut("Resolution time:" << resolutionTime);
  
  Slot* promiseSlot2 = CrabsCreatePromise(crabs, now + 1000);
  IUB errorValue = 123;
  op = CrabsRejectPromise(crabs, promiseSlot2, &errorValue);
  if (op) {
    _DROut("Error: Failed to reject promise");
    return _NilP; //<  return op; //< ???
  }
  
  _DROut("\nPromise rejected with error:" << errorValue);
  _DROut("Promise state (should be 2=rejected):" << PromiseState(promiseSlot2));
  _AAvow(IUW(PromiseRejected), PromiseState(promiseSlot2));
  
  CHA onFulfilledKey[] = "onFulfill";
  op = CrabsThenPromise(crabs, promiseSlot, onFulfilledKey);
  if (op) {
    _DROut("Error: Failed to attach then handler");
    return _NilP; //<  return op; //< ???
  }
  _DROut("\n.then() handler attached");
  
  CHA onRejectedKey[] = "onReject";
  op = CrabsCatchPromise(crabs, promiseSlot2, onRejectedKey);
  if (op) {
    _DROut("Error: Failed to attach catch handler");
    return _NilP; //<  return op; //< ???
  }
  _DROut(".catch() handler attached");
  
  _DROut("\n| Promise tests completed successfully\n");
#endif
  return _NilP;
}

}  //< namespace CRTest
