// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_BOOK
#include "../Book.hpp"
#include "TestWords.h"
#if SEAM == CRABS_BOOK
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
using namespace ::_;
namespace CRTest {

template<typename ISZ = ISN, typename ISY = ISM, typename CHS = CHR, 
  typename CHT = CHD, typename DT  = DTB>
void TestBook() {
  D_COUT(Linef("\n\n---\n\n"));

  enum {
    CBytes = 512 * sizeof(CHS)
  };

  D_COUT("\n\nTesting ABook<IS" << CSizeCodef<ISZ>() << ",IS" << CSizeCodef<ISY>() << 
         ",CH" << CSizeCodef<CHS>() << ",DT" << CSizeCodef<DT>() << 
         "> sizeof:" << sizeof(TBook<BOK_P>) << " with Bytes : " << CBytes);

  ABook<BOK_P, CBytes> book;

  D_COUT("\n\nAfter TBookInit book.Bytes():" << book.Bytes() << 
         " book.Count():" << book.Count() << 
         " book.Total():" << book.Total());
  D_COUT("\nPrinting empty book:\n");
  D_COUT_BOOK(book.This());

  ISZ test_word_count = book.Total();
  D_COUT("\nPopulating " << test_word_count << " test words...");

  const CHS * test_words  = TTestWords<CHS>::Words(),
            * word_cursor = test_words;
  ISZ word_step = TTestWords<CHS>::CharsMax;

  ISY i = 0;
  D_COUT("\n\nStep 1: Fill up the dictionary...\n" << Linef("---") << '\n');
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISA('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUA('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISB('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUB('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISC('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUC('!' + i)));
  A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISD('!' + i)));
  //book.COut();

  D_COUT("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  D_COUT("\n\nStep 2: Testing Factory.Grow...\n" << Linef("---") << '\n');
  while (i < ISY(TTestWords<CHS>::Total - 8)) {
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISA('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUA('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISB('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUB('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISC('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUC('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, ISD('!' + i)));
    A_AVOW(ISY(++i), book.Insert(word_cursor += word_step, IUD('!' + i)));
  }
  //book.COut();

  D_COUT("\n\nStep 3 Adding large string...:\n" << Linef("---\n"));
  CHS large_string[CBytes] = {0};
  CHS* cursor = large_string;
  for (ISN i = 0; i < CBytes - 1; ++i) *cursor++ = '*';
  *cursor = 0;
  ISZ index = book.Insert(large_string, 1);
  auto dez_nutz = book.This();
  D_COUT_BOOK(dez_nutz);
}
}  //< namespace CRTest
#undef TPARAMS
#endif

namespace CRTest {
const CHA* Book(const CHA* args) {
#if SEAM >= CRABS_BOOK
  A_TEST_BEGIN;
#if USING_STA == YES_0
  //TestBook<ISB, ISA, CHA, CHC>();
  TestBook<ISC, ISB, CHA, CHC>();
  TestBook<ISD, ISC, CHA, CHC>();
#endif
#if USING_STB == YES_0
  //TestBook<ISB, ISA, CHB, CHC>();
  TestBook<ISC, ISB, CHB, CHC>();
  TestBook<ISD, ISC, CHB, CHC>();
#endif
#if USING_STC == YES_0
  //TestBook<ISB, ISA, CHC, CHC>();
  TestBook<ISC, ISB, CHC, CHC>();
  TestBook<ISD, ISC, CHC, CHC>();
#endif
#endif
  return NILP;
}
}  //< namespace CRTest
