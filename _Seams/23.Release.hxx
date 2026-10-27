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
  // The release seam is the final sown test pass: it runs the complete
  // tree of every lower seam's checks in one pass.
  return CrabsTests(args);
#endif
  return NILP;
}

}  //< namespace CRTest
