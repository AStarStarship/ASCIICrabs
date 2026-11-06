// Copyright AStarship <https://astarship.net>.
#include "../Door.hpp"
#if _Seam >= _CrabsScript
#if _Seam == _CrabsScript
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

static const CHA* CrabsScript(const CHA* args) {
#if _Seam == _CrabsScript
  _ATestBegin;
  TDoor<> door;
  Slot slot;
#endif
  return _NilP;
}
}  //< namespace CRTest
