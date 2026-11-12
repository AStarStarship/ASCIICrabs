// Copyright AStarship <https://astarship.net>.

#if _Seam >= _CrabsString
#include "../String.hpp"
#if _Seam == _CrabsString
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

#if _Seam >= _CrabsString
template<typename T = CHR, typename CHT = CHD, typename ISZ = ISS, 
  typename ISY=ISZ>
void TestStringN() {
  _DROut(Linef("\n\n\n---\nTesting TString<CH") << CSizeCodef<T>() << ", IS" <<
         CSizeCodef<ISZ>() << ", IS" << CSizeCodef<ISY>() << ">\n");
  static const T Testing123[] = {'T', 'e', 's', 't', 'i', 'n',
                                  'g', ' ', '1', ',', ' ', '2',
                                  ',', ' ', '3', NIL};
  enum {
    Count = 256
  };

  AString<STR_P, 8> str;
  _DROut("\nPrinting to 0x" << Hexf(str.This()) << "\nstr.Total():" << 
         str.Total() << " str.Count():" << str.Count() << " str.Bytes():" <<
         str.CBytes() << "\nExpecting \"Testing 1, 2, 3\"");
  
  T cbof[Count];
  TSPrint<T>(cbof, cbof + Count, 10);
  _DROut("\n\nTest:\n" << cbof);

  str << "Testing ";
  for (ISC i = 1; i <= 3; ++i) str << i << ", ";
  _AAssert(str.Find(Testing123));
  for (ISC i = 3; i < Count; ++i) str << i << ", ";
  
  _DROut("\n\nResult:\"" << str.Begin() << "\"\n");
}

#endif

static const CHA* String(const CHA* args) {
#if _Seam >= _CrabsString
  _ATestBegin;
  //TestStringN<CHA, CHA, ISC, ISB>();
  //TestStringN<CHA, CHC, ISC, ISB>();
  TestStringN<CHB, CHC, ISC, ISB>();
  //vTestStringN<CHC, CHC, ISC, ISB>();
#endif
  return _NilP;
}
}  //< namespace CRTest
