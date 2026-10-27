// Copyright AStarship <https://astarship.net>.
#include "pch.h"
#include <_Config.h>
#include "_Tests.hxx"
using namespace ::_;

ISN main(ISN arg_count, CHA** args) {
  const CHA* argss = ArgsToString(arg_count, args);
#if SEAM == SEAM_N
  return SeamResult(CRTest::Release(argss));
#else
  return TTestTree<CrabsTests>(arg_count, args);
#endif
}
