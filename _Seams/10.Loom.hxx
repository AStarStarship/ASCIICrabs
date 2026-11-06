// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsLoom
#include "../Loom.hpp"
//
#include "TestWords.h"
#if _Seam == _CrabsLoom
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
namespace CRTest {
using namespace ::_;

template<typename CHS = CHR, typename CHT = CHD, typename ISZ = ISN, typename ISY = ISN>
void TestLoom() {
  _DROut(Linef("\n\n---\n\n"));

  enum {
    Size = 256 * sizeof(CHS),
    Count = 16,
  };
  _DROut("Testing ALoom<CHS" << CSizeCodef<CHS>() << ",IS" << CSizeCodef<ISZ>() <<
         "> with Size:" << Size << " and Count:" << Count);

  ALoom<CHS, CHT, ISZ, ISY, Size, TBOF<Size>> loom(Count);
#ifdef _DThis
  _DROut("\nPrinting empty loom:\n");
  loom.ROut();
#endif

  enum {
    LengthMax = (Size << 2) - 1,
  };

  CHS element[LengthMax + 1] = {};
  CHS* str_end = &element[LengthMax];
  ISY i = 0;
  for (; i < Count; ++i) {
    TSPrint<CHS, CHT>(element, str_end, i);
    _DROut("\nstr:" << element);
    _AAvow(ISY(i), loom.Insert(element));
  }

  _DROut("\n\nTesting Factory.Grow...\n");

  TSPrint<CHS, CHT>(element, str_end, Count);
  loom.Insert(element);
  IUW* buffer = loom.AJT().origin;
  auto dez_nutz = loom.Insert(element);
  _AAvow(buffer, loom.AJT().origin);
  //i = Count + 1;
  _AAvow(++i, dez_nutz);
  while (i < 96) {
    TSPrint<CHS, CHT>(element, str_end, i);
    _AAvow(ISY(++i), loom.Insert(element));
  }
  
  TSPrint<CHS, CHT>(element, str_end, i);
  _AAvow(++i, loom.Insert(element));
  
  _DROut("\n\nAttempting to add a very large string...\n");

  for (i = 0; i < LengthMax; ++i) element[i] = '*';

  element[LengthMax] = 0;

  ISY index = loom.Insert(element);
#ifdef _DThis
  loom.ROut();
#endif
  _AAvow(ISY(98), index);

#ifdef _DThis
  _DROut('\n');
  loom.ROut();
  _DROut('\n');
#endif

  _DROut("\nTesting TLoomFind...\n");

  static const CHS a[] = {'A', '\0'}, b[] = {'B', '\0'}, c[] = {'C', '\0'},
                   d[] = {'D', '\0'}, abc[] = {'a', 'b', 'c', '\0'},
                   bac[] = {'b', 'a', 'c', '\0'}, cba[] = {'c', 'b', 'a', '\0'},
                   cab[] = {'c', 'a', 'b', '\0'},
                   test[] = {'t', 'e', 's', 't', '\0'};

  _AAvow(ISY(99), loom.Insert(d));
  _AAvow(ISY(99), loom.Find(d));

  _AAvow(ISY(100), loom.Insert(c));
  _AAvow(ISY( 99), loom.Find(d));
  _AAvow(ISY(100), loom.Find(c));

  _AAvow(ISY(101), loom.Insert(b));
  _AAvow(ISY( 99), loom.Find(d));
  _AAvow(ISY(100), loom.Find(c));
  _AAvow(ISY(101), loom.Find(b));

  _AAvow(ISY(102), loom.Insert(a));
  _AAvow(ISY( 99), loom.Find(d));
  _AAvow(ISY(100), loom.Find(c));
  _AAvow(ISY(101), loom.Find(b));
  _AAvow(ISY(102), loom.Find(a));

  _AAvow(ISY(103), loom.Insert(abc));
  _AAvow(ISY(103), loom.Find(abc));

  _AAvow(ISY(104), loom.Insert(bac));
  _AAvow(ISY(103), loom.Find(abc));
  _AAvow(ISY(104), loom.Find(bac));

  _AAvow(ISY(105), loom.Insert(cba));
  _AAvow(ISY(103), loom.Find(abc));
  _AAvow(ISY(104), loom.Find(bac));
  _AAvow(ISY(105), loom.Find(cba));

  _AAvow(ISY(106), loom.Insert(cab));
  _AAvow(ISY(103), loom.Find(abc));
  _AAvow(ISY(104), loom.Find(bac));
  _AAvow(ISY(105), loom.Find(cba));
  _AAvow(ISY(106), loom.Find(cab));

#ifdef _DThis
  _DROut('\n');
  loom.ROut();
  _DROut('\n');
#endif

  _AAvow(ISY(-1), loom.Find(test));
}
}  //< namespace CRTest
#endif

namespace CRTest {
static const CHA* Loom(const CHA* args) {
#if _Seam >= _CrabsLoom
  _ATestBegin;
  //TestLoom<CHA, CHA, ISB, ISB>();
  // 
  //TestLoom<CHA, CHC, ISB, ISB>();
  //TestLoom<CHA, CHC, ISC, ISC>();
  //TestLoom<CHA, CHC, ISB, ISA>();
  //TestLoom<CHA, CHC, ISC, ISB>();
  TestLoom<CHA, CHC, ISD, ISC>();

  //TestLoom<CHB, CHC, ISB, ISB>();
  //TestLoom<CHB, CHC, ISC, ISC>();
  //TestLoom<CHB, CHC, ISB, ISA>();
  //TestLoom<CHB, CHC, ISC, ISB>();
  TestLoom<CHB, CHC, ISD, ISC>();

  //TestLoom<CHC, CHC, ISC, ISC>();
  //TestLoom<CHC, CHC, ISB, ISA>();
  //TestLoom<CHC, CHC, ISC, ISB>();
  TestLoom<CHC, CHC, ISD, ISC>();

#endif
  return _NilP;
}
}  //< namespace CRTest
