// Copyright AStarship <https://astarship.net>.
#include "../Door.hpp"
#if SEAM >= CRABS_SCRIPT
#if SEAM == CRABS_SCRIPT
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
using namespace ::_;
namespace CRTest {

static const CHA* CrabsScript(const CHA* args) {
#if SEAM == CRABS_SCRIPT
  A_TEST_BEGIN;
  ADoor door;
  Slot slot;
#endif
  return NILP;
}
}  //< namespace CRTest
