// Copyright AStarship <https://astarship.net>.
#include "../Door.hpp"
#if _Seam >= _CrabsRelease
#if _Seam == _CrabsRelease
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

inline const CHA* Release(const CHA* args) {
#if _Seam == _CrabsRelease
  _ATestBegin;
  TDoor<> door;
  Slot slot;
#endif
  return _NilP;
}

}  //< namespace CRTest
