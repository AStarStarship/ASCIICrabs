// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_LOOM
#include "../Loom.hpp"
//
#include "TestWords.h"
#if SEAM == CRABS_LOOM
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
using namespace ::_;
namespace CRTest {

template<typename CHS = CHR, typename CHT = CHD, typename ISZ = ISN, typename ISY = ISN>
void TestLoom() {
  D_COUT(Linef("\n\n---\n\n"));

  enum {
    Size = 256 * sizeof(CHS),
    Count = 16,
  };
  D_COUT("Testing ALoom<CHS" << CSizeCodef<CHS>() << ",IS" << CSizeCodef<ISZ>() <<
         "> with Size:" << Size << " and Count:" << Count);

  ALoom<CHS, CHT, ISZ, ISY, Size, TBOF<Size>> loom(Count);
#ifdef D_THIS
  D_COUT("\nPrinting empty loom:\n");
  loom.COut();
#endif

  enum {
    LengthMax = (Size << 2) - 1,
  };

  CHS element[LengthMax + 1] = {};
  CHS* str_end = &element[LengthMax];
  ISY i = 0;
  for (; i < Count; ++i) {
    TSPrint<CHS, CHT>(element, str_end, i);
    D_COUT("\nstr:" << element);
    A_AVOW(ISY(i), loom.Insert(element));
  }

  D_COUT("\n\nTesting Factory.Grow...\n");

  TSPrint<CHS, CHT>(element, str_end, Count);
  loom.Insert(element);
  IUW* boofer = loom.AJT().origin;
  auto dez_nutz = loom.Insert(element);
  A_AVOW(boofer, loom.AJT().origin);
  //i = Count + 1;
  A_AVOW(++i, dez_nutz);
  while (i < 96) {
    TSPrint<CHS, CHT>(element, str_end, i);
    A_AVOW(ISY(++i), loom.Insert(element));
  }
  
  TSPrint<CHS, CHT>(element, str_end, i);
  A_AVOW(++i, loom.Insert(element));
  
  D_COUT("\n\nAttempting to add a very large string...\n");

  for (i = 0; i < LengthMax; ++i) element[i] = '*';

  element[LengthMax] = 0;

  ISY index = loom.Insert(element);
#ifdef D_THIS
  loom.COut();
#endif
  A_AVOW(ISY(98), index);

#ifdef D_THIS
  D_COUT('\n');
  loom.COut();
  D_COUT('\n');
#endif

  D_COUT("\nTesting TLoomFind...\n");

  static const CHS a[] = {'A', '\0'}, b[] = {'B', '\0'}, c[] = {'C', '\0'},
                   d[] = {'D', '\0'}, abc[] = {'a', 'b', 'c', '\0'},
                   bac[] = {'b', 'a', 'c', '\0'}, cba[] = {'c', 'b', 'a', '\0'},
                   cab[] = {'c', 'a', 'b', '\0'},
                   test[] = {'t', 'e', 's', 't', '\0'};

  A_AVOW(ISY(99), loom.Insert(d));
  A_AVOW(ISY(99), loom.Find(d));

  A_AVOW(ISY(100), loom.Insert(c));
  A_AVOW(ISY( 99), loom.Find(d));
  A_AVOW(ISY(100), loom.Find(c));

  A_AVOW(ISY(101), loom.Insert(b));
  A_AVOW(ISY( 99), loom.Find(d));
  A_AVOW(ISY(100), loom.Find(c));
  A_AVOW(ISY(101), loom.Find(b));

  A_AVOW(ISY(102), loom.Insert(a));
  A_AVOW(ISY( 99), loom.Find(d));
  A_AVOW(ISY(100), loom.Find(c));
  A_AVOW(ISY(101), loom.Find(b));
  A_AVOW(ISY(102), loom.Find(a));

  A_AVOW(ISY(103), loom.Insert(abc));
  A_AVOW(ISY(103), loom.Find(abc));

  A_AVOW(ISY(104), loom.Insert(bac));
  A_AVOW(ISY(103), loom.Find(abc));
  A_AVOW(ISY(104), loom.Find(bac));

  A_AVOW(ISY(105), loom.Insert(cba));
  A_AVOW(ISY(103), loom.Find(abc));
  A_AVOW(ISY(104), loom.Find(bac));
  A_AVOW(ISY(105), loom.Find(cba));

  A_AVOW(ISY(106), loom.Insert(cab));
  A_AVOW(ISY(103), loom.Find(abc));
  A_AVOW(ISY(104), loom.Find(bac));
  A_AVOW(ISY(105), loom.Find(cba));
  A_AVOW(ISY(106), loom.Find(cab));

#ifdef D_THIS
  D_COUT('\n');
  loom.COut();
  D_COUT('\n');
#endif

  A_AVOW(ISY(-1), loom.Find(test));
}
}  //< namespace CRTest
#endif

namespace CRTest {
static const CHA* Loom(const CHA* args) {
#if SEAM >= CRABS_LOOM
  A_TEST_BEGIN;
  //TestLoom<CHA, CHA, ISB, ISB>();
  // 
  //TestLoom<CHA, CHC, ISB, ISB>();
  //TestLoom<CHA, CHC, ISC, ISC>();
  //TestLoom<CHA, CHC, ISB, ISA>();
  //TestLoom<CHA, CHC, ISC, ISB>();
  TestLoom<CHA, CHC, ISD, ISC>();
#if USING_STB == YES_0
  //TestLoom<CHB, CHC, ISB, ISB>();
  //TestLoom<CHB, CHC, ISC, ISC>();
  //TestLoom<CHB, CHC, ISB, ISA>();
  //TestLoom<CHB, CHC, ISC, ISB>();
  TestLoom<CHB, CHC, ISD, ISC>();
#endif
#if USING_STC == YES_0
  //TestLoom<CHC, CHC, ISC, ISC>();
  //TestLoom<CHC, CHC, ISB, ISA>();
  //TestLoom<CHC, CHC, ISC, ISB>();
  TestLoom<CHC, CHC, ISD, ISC>();
#endif
#endif
  return NILP;
}
}  //< namespace CRTest
