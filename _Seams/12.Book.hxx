// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsBook
#include "../Book.hpp"
#include "TestWords.h"
#if _Seam == _CrabsBook
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
namespace CRTest {
using namespace ::_;

template<typename ISZ = ISN, typename ISY = ISM, typename CHS = CHR, 
  typename CHT = CHD, typename DT  = DTB>
void TestBook() {
  _DROut(Linef("\n\n---\n\n"));

  enum {
    CBytes = 512 * sizeof(CHS)
  };

  _DROut("\n\nTesting ABook<IS" << CSizeCodef<ISZ>() << ",IS" << CSizeCodef<ISY>() << 
         ",CH" << CSizeCodef<CHS>() << ",DT" << CSizeCodef<DT>() << 
         "> sizeof:" << sizeof(TBook<BOK_P>) << " with Bytes : " << CBytes);

  ABook<BOK_P, CBytes> book;

  _DROut("\n\nAfter TBookInit book.Bytes():" << book.Bytes() << 
         " book.Count():" << book.Count() << 
         " book.Total():" << book.Total());
  _DROut("\nPrinting empty book:\n");
  _DROutBook(book.This());

  ISZ test_word_count = book.Total();
  _DROut("\nPopulating " << test_word_count << " test words...");

  const CHS * test_words  = TTestWords<CHS>::Words(),
            * word_cursor = test_words;
  ISZ word_step = TTestWords<CHS>::CharsMax;

  ISY i = 0;
  _DROut("\n\nStep 1: Fill up the dictionary...\n" << Linef("---") << '\n');
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISA('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUA('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISB('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUB('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISC('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUC('!' + i)));
  _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISD('!' + i)));
  //book.ROut();

  _DROut("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  _DROut("\n\nStep 2: Testing Factory.Grow...\n" << Linef("---") << '\n');
  while (i < ISY(TTestWords<CHS>::Total - 8)) {
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISA('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUA('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISB('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUB('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISC('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUC('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, ISD('!' + i)));
    _AAvow(ISY(++i), book.Insert(word_cursor += word_step, IUD('!' + i)));
  }
  //book.ROut();

  _DROut("\n\nStep 3 Adding large string...:\n" << Linef("---\n"));
  CHS large_string[CBytes] = {0};
  CHS* cursor = large_string;
  for (ISN i = 0; i < CBytes - 1; ++i) *cursor++ = '*';
  *cursor = 0;
  ISZ index = book.Insert(large_string, 1);
  auto dez_nutz = book.This();
  _DROutBook(dez_nutz);
}
}  //< namespace CRTest
#undef TPARAMS
#endif

namespace CRTest {
const CHA* Book(const CHA* args) {
#if _Seam >= _CrabsBook
  _ATestBegin;

  //TestBook<ISB, ISA, CHA, CHC>();
  TestBook<ISC, ISB, CHA, CHC>();
  TestBook<ISD, ISC, CHA, CHC>();

  //TestBook<ISB, ISA, CHB, CHC>();
  TestBook<ISC, ISB, CHB, CHC>();
  TestBook<ISD, ISC, CHB, CHC>();

  //TestBook<ISB, ISA, CHC, CHC>();
  TestBook<ISC, ISB, CHC, CHC>();
  TestBook<ISD, ISC, CHC, CHC>();

#endif
  return _NilP;
}
}  //< namespace CRTest
