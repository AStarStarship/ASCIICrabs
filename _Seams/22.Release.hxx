// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_RELEASE
#if SEAM == CRABS_RELEASE
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {

inline const CHA* Release(const CHA* args) {
#if SEAM == CRABS_RELEASE
#endif
  return NILP;
}

}  //< namespace CRTest
