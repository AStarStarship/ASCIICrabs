// Copyright AStarship <https://astarship.net>.

#if SEAM >= CRABS_STRING
#include "../String.hpp"
#if SEAM == CRABS_STRING
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif

using namespace ::_;
namespace CRTest {
#if SEAM >= CRABS_STRING
template<typename T = CHR, typename CHT = CHD, typename ISZ = ISR, 
  typename ISY=ISZ>
void TestStringN() {
  D_COUT(Linef("\n\n\n---\nTesting TString<CH") << CSizeCodef<T>() << ", IS" <<
         CSizeCodef<ISZ>() << ", IS" << CSizeCodef<ISY>() << ">\n");
  static const T Testing123[] = {'T', 'e', 's', 't', 'i', 'n',
                                  'g', ' ', '1', ',', ' ', '2',
                                  ',', ' ', '3', NIL};
  enum {
    Count = 256
  };

  AString<STR_P, 8> str;
  D_COUT("\nPrinting to 0x" << Hexf(str.This()) << "\nstr.Total():" << 
         str.Total() << " str.Count():" << str.Count() << " str.Bytes():" <<
         str.CBytes() << "\nExpecting \"Testing 1, 2, 3\"");
  
  T cbof[Count];
  TSPrint<T>(cbof, cbof + Count, 10);
  D_COUT("\n\nTest:\n" << cbof);

  str << "Testing ";
  for (ISC i = 1; i <= 3; ++i) str << i << ", ";
  A_ASSERT(str.Find(Testing123));
  for (ISC i = 3; i < Count; ++i) str << i << ", ";
  
  D_COUT("\n\nResult:\"" << str.Begin() << "\"\n");
}

#endif

static const CHA* String(const CHA* args) {
#if SEAM >= CRABS_STRING
  A_TEST_BEGIN;

  //TestStringN<CHA, CHA, ISC, ISB>();
  //TestStringN<CHA, CHC, ISC, ISB>();
#if USING_STB == YES_0
  TestStringN<CHB, CHC, ISC, ISB>();
#endif
#if USING_STC == YES_0
  //vTestStringN<CHC, CHC, ISC, ISB>();
#endif

#endif
  return NILP;
}
}  //< namespace CRTest
