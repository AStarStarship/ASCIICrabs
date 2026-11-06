// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsTable
#include "../Table.hpp"
//
#include "./TestWords.h"
#if _Seam == _CrabsTable
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
namespace CRTest {
using namespace ::_;

template<TBL_A>
void TestTable() {
  _DROut(Linef("\n\n\n\n\n\n+---\nTesting ATable<IS") 
    << ",CH" << CSizef<CHS>() << ",CH" << CSizef<CHT>() 
    << " IS" << CSizef<ISZ>() << ",IU" << CSizef<HSH>() 
    << ">\n" << Linef("+---\n\n"));
  ISY total_init = 13;
  ATable<TBL_P> table(total_init);
  const TBL* This = table.This();
  const ISZ size_bytes = table.CBytes();

  static const CHS a[] = {'A', '\0'}, b[] = {'B', '\0'}, c[] = {'C', '\0'},
                   d[] = {'D', '\0'}, e[] = { 'E', '\0' },
                   abc[] = {'a', 'b', 'c', '\0'},
                   bac[] = {'b', 'a', 'c', '\0'},
                   cba[] = {'c', 'b', 'a', '\0'},
                   cab[] = {'c', 'a', 'b', '\0'},
                   def[] = {'d', 'e', 'f', '\0'},
                   fed[] = {'f', 'e', 'd', '\0'},
                   efd[] = {'e', 'f', 'd', '\0'},
                   dfe[] = {'d', 'f', 'e', '\0'},
                   test[] = {'t', 'e', 's', 't', '\0'};

  _AAvow(ISY(0), table.Append(a));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(0), table.Find(a));

  _AAvow(ISY(1), table.Append(b));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(0), table.Find(a));
  _AAvow(ISY(1), table.Find(b));

  _AAvow(ISY(2), table.Append(c));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(0), table.Find(a));
  _AAvow(ISY(1), table.Find(b));
  _AAvow(ISY(2), table.Find(c));

  _AAvow(ISY(3), table.Append(d));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(0), table.Find(a));
  _AAvow(ISY(1), table.Find(b));
  _AAvow(ISY(2), table.Find(c));
  _AAvow(ISY(3), table.Find(d));

  _AAvow(ISY(0), table.Append(a));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(1), table.Append(b));
  _AAvow(ISY(2), table.Append(c));
  _AAvow(ISY(3), table.Append(d));

  _AAvow(ISY(4), table.Append(abc));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(4), table.Find(abc));

  _AAvow(ISY(5), table.Append(bac));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(4), table.Find(abc));
  _AAvow(ISY(5), table.Find(bac));

  _AAvow(ISY(6), table.Append(cba));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(4), table.Find(abc));
  _AAvow(ISY(5), table.Find(bac));
  _AAvow(ISY(6), table.Find(cba));

  _AAvow(ISY(7), table.Append(e));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(4), table.Find(abc));
  _AAvow(ISY(5), table.Find(bac));
  _AAvow(ISY(6), table.Find(cba));
  _AAvow(ISY(7), table.Find(e));

  _AAvow(ISY(8), table.Append(cab));
  _AAssert(TTableHashesVerify<TBL_P>(This));
  _AAvow(ISY(4), table.Find(abc));
  _AAvow(ISY(5), table.Find(bac));
  _AAvow(ISY(6), table.Find(cba));
  _AAvow(ISY(7), table.Find(e));
  _AAvow(ISY(8), table.Find(cab));

  _AAvow(ISY(9 ), table.Append(def));
  _AAvow(ISY(10), table.Append(fed));
  _AAvow(ISY(11), table.Append(efd));
  _AAvow(ISY(12), table.Append(dfe));
  _AAvow(ISY(9 ), table.Find(def));
  _AAvow(ISY(10), table.Find(fed));
  _AAvow(ISY(11), table.Find(efd));
  _AAvow(ISY(12), table.Find(dfe));
  _DROutOBJ(table);
  _AAvow(CAInvalidIndex<ISY>(), table.Find(test));

  const CHS* test_words = TTestWords<CHS>::Words(),
    * word_cursor = test_words;
  ISZ word_step = TTestWords<CHS>::CharsMax;

  ISY i = total_init;
  _DROut("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  _DROut("\n\nStep 8: Testing Factory.Grow...\n" << Linef("---") << '\n');
  _AAvow(ISY(i++), table.Append(word_cursor += word_step));
  //table.ROut();
  _DROut("\n\nStep 8b: Testing POD types...\n" << Linef("---\n"));
  for (; i < ISY(TTestWords<CHS>::Total) - 8;) {
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
    _AAvow(ISY(i++), table.Append(word_cursor += word_step));
  }

  _DROut("\n\nStep 8c Adding large string...:\n" << Linef("---\n"));
  enum { LargeStringLength = 255 };
  CHS large_string[LargeStringLength] = { 0 };
  CHS* cursor = large_string;
  for (ISN i = 0; i < LargeStringLength - 1; ++i) *cursor++ = '*';
  *cursor = 0;
  ISY index = table.Append(large_string);
  auto dez_nutz = table.This();
  _DROutTable(dez_nutz);
}
}  //< namespace CRTest
#endif

namespace CRTest {
static const CHA* Table(const CHA* args) {
#if _Seam >= _CrabsTable
  _ATestBegin;

  TestTable<CHA, CHC, ISC, ISB, IUC>();
  TestTable<CHA, CHC, ISD, ISC, IUD>();

  TestTable<CHB, CHC, ISC, ISB, IUC>();
  TestTable<CHB, CHC, ISD, ISC, IUD>();

  TestTable<CHC, CHC, ISC, ISB, IUC>();
  TestTable<CHC, CHC, ISD, ISC, IUD>();

#endif
  return _NilP;
}
}  //< namespace CRTest
