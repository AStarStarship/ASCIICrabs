// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_PROMISE
#include "../Promise.h"
#if SEAM == CRABS_PROMISE
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
using namespace ::_;
namespace CRTest {

static const CHA* Promise(const CHA* args) {
#if SEAM == CRABS_PROMISE
  A_TEST_BEGIN;
  
  enum {
    BooferBytes = 512,
    BooferWords = BooferBytes / sizeof(IUW),
    StackTotal = 8,
  };
  
  D_COUT("\nTesting Promise API...\n");
  
  IUW boofer[BooferWords],
      unpackedExpr[BooferWords];
  IUD pcCtx = 0;
  
  StarTest starTest;
  Crabs* crabs = CrabsInit(boofer, BooferBytes, StackTotal, &starTest,
                           unpackedExpr, BooferBytes, pcCtx);
  
  TMC now = crabs->epoch;
  Slot* promiseSlot = CrabsCreatePromise(crabs, now);
  
  if (!promiseSlot) {
    D_COUT("Error: Failed to create promise slot");
    return TPtr<const CHA>(AErrorNil);
  }
  
  D_COUT("\nPromise created at timestamp:" << now);
  D_COUT("Promise state (should be 0=pending):" << PromiseState(promiseSlot));
  A_AVOW(IUW(PromisePending), PromiseState(promiseSlot));
  
  IUW resultValue = 42;
  const Op* op = CrabsResolvePromise(crabs, promiseSlot, &resultValue);
  if (op) {
    D_COUT("Error: Failed to resolve promise");
    return NILP; //<  return op; //< ???
  }
  
  D_COUT("\nPromise resolved with value:" << resultValue);
  D_COUT("Promise state (should be 1=fulfilled):" << PromiseState(promiseSlot));
  A_AVOW(IUW(PromiseFulfilled), PromiseState(promiseSlot));
  
  TMC resolutionTime = PromiseTimestamp(promiseSlot);
  D_COUT("Resolution time:" << resolutionTime);
  
  Slot* promiseSlot2 = CrabsCreatePromise(crabs, now + 1000);
  IUB errorValue = 123;
  op = CrabsRejectPromise(crabs, promiseSlot2, &errorValue);
  if (op) {
    D_COUT("Error: Failed to reject promise");
    return NILP; //<  return op; //< ???
  }
  
  D_COUT("\nPromise rejected with error:" << errorValue);
  D_COUT("Promise state (should be 2=rejected):" << PromiseState(promiseSlot2));
  A_AVOW(IUW(PromiseRejected), PromiseState(promiseSlot2));
  
  CHA onFulfilledKey[] = "onFulfill";
  op = CrabsThenPromise(crabs, promiseSlot, onFulfilledKey);
  if (op) {
    D_COUT("Error: Failed to attach then handler");
    return NILP; //<  return op; //< ???
  }
  D_COUT("\n.then() handler attached");
  
  CHA onRejectedKey[] = "onReject";
  op = CrabsCatchPromise(crabs, promiseSlot2, onRejectedKey);
  if (op) {
    D_COUT("Error: Failed to attach catch handler");
    return NILP; //<  return op; //< ???
  }
  D_COUT(".catch() handler attached");
  
  D_COUT("\n| Promise tests completed successfully\n");
#endif
  return NILP;
}

}  //< namespace CRTest
