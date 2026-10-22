// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_FTOS
#include "../Random.h"
#if SEAM == CRABS_FTOS
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif

using namespace ::_;
namespace CRTest {
const CHA* FtoS(const CHA* args) {
#if SEAM >= CRABS_FTOS
  A_TEST_BEGIN;

#if SEAM == CRABS_FTOS
  enum { TestCount = 1 << 20 };
#else
  enum { TestCount = 1 << 15 };
#endif
  /*
  enum { Size = 31 };
  CHA socket[Size + 1];

  IUD value;
  FPD dbl_expected, dbl_found;

  D_COUT("\n\nTesting Float Ceiling<Float, IU> (Float)...\n");

  for (ISC i = 0; i < TestCount; ++i) {
    do {
      value = IUDRandom();
      dbl_expected = static_cast<FPD>(value);
    } while (!IsFinite(dbl_expected));
    dbl_found = ceil(dbl_expected);
    dbl_expected = Ceiling(dbl_expected);
    Test(dbl_expected, dbl_found);
  }

  D_COUT(
      "\n\nTesting const CHS* TSScan<CHS, CHT> (const CHS*, const CHS*, FPC&) "
      "functions...\n");

  for (ISC i = 0; i < TestCount; ++i) {
    do {
      value = IUDRandom();
      dbl_expected = static_cast<FPD>(value);
    } while (!IsFinite(dbl_expected));
    TSPrintTo(socket, dbl_expected);
    A_ASSERT(SScan(socket, dbl_found));
    A_AVOW(dbl_expected, dbl_found);
  }

  D_COUT("\n\nTesting CHS* TSPrint<CHS, CHT> (CHS*, CHS*, Float)...\n");

  for (ISC i = 0; i < TestCount; ++i) {
    do {
      value = IUDRandom();
      dbl_expected = static_cast<FPD>(value);
    } while (!IsFinite(dbl_expected));
    Print(socket, socket + Size, dbl_expected);
    ISC r = sscanf_s(socket, "%lf", &dbl_found);
    A_ASSERT(r);
    A_AVOW(dbl_expected, dbl_found);
  }
  TDecimalFloat<>::PrintDebugInfo();
  */
#endif
  return NILP;
}
}  //< namespace CRTest
