// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsRandom
#include "../Random.h"
#if _Seam == _CrabsRandom
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

static const CHA* RNG(const CHA* args) {
#if _Seam >= _CrabsRandom
  _ATestBegin;

#if _Seam == _CrabsRandom
  enum { PrintCount = 256 };
#else
  enum { PrintCount = 1 };
#endif

  for (ISN i = 0; i < PrintCount; ++i)
    _DROut('\n' << i << ".) " << Random(TISMin<ISN>(), TISMax<ISN>()));

#endif
  return _NilP;
}
}  //< namespace CRTest
