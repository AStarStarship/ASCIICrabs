// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_CORE
#include "../Uniprinter.hpp"
#if SEAM == CRABS_CORE
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
using namespace ::_;
namespace CRTest {

namespace AlignmentTest {
static ISW checks = 0, failures = 0;
static void Check(BOL pass, const CHA* name) {
  ++checks;
  if (!pass) { ++failures; if (failures <= 12) D_COUT("FAIL: " << name << D_COUT_NL); }
}

template<typename I>
static void Integers() {
  for (ISC mask = 0; mask <= 7; mask = mask * 2 + 1) {
    const ISC alignment = mask + 1;
    for (ISC input = 0; input <= 64; ++input) {
      const I value = I(input), m = I(mask);
      const I down = I(input - input % alignment);
      const I up = I(input + (input % alignment ? alignment - input % alignment : 0));
      Check(AlignDown(value, m) == down, "runtime align down");
      Check(CAlignDown(value, m) == down, "constexpr align down");
      Check(AlignUp(value, m) == up && CAlignUp(value, m) == up, "align up parity");
      Check(TAlignUp(value, m) == up && TAlignDownI(value, m) == down, "template mask parity");
    }
  }
}

static void Pointers() {
  alignas(IUW) CHA bytes[96] = {};
  for (ISW offset = 0; offset < 32; ++offset) {
    CHA* p = bytes + offset;
    const CHA* cp = p;
    for (IUW mask = 0; mask <= 7; mask = mask * 2 + 1) {
      const IUW address = IUW(p), alignment = mask + 1;
      const IUW down = address - address % alignment;
      const IUW up = address + (address % alignment ? alignment - address % alignment : 0);
      Check(IUW(AlignDown(p, mask)) == down, "mutable pointer mask is not an offset");
      Check(IUW(AlignDown(cp, mask)) == down, "const pointer mask is not an offset");
      Check(IUW(AlignUp(cp, mask)) == up, "pointer align up");
      Check(IUW(TPtrUp<CHA>(p, ISW(mask))) == up &&
            IUW(TPtrUp<CHA>(cp, ISW(mask))) == up, "typed pointer up");
      Check(IUW(TPtrDown<CHA>(p, 0, ISW(mask))) == down, "explicit pointer offset/mask");
    }
    Check(AlignUpC(p) == ISC((sizeof(ISC) - IUW(p) % sizeof(ISC)) % sizeof(ISC)),
          "AlignUpC uses ISC width, not word width");
  }
}

static void Extremes() {
  volatile ISC low_c = ISC(IUC(1) << 31);
  volatile ISD low_d = ISD(IUD(1) << 63);
  volatile IUC high_c = IUC(1) << 31;
  volatile IUD high_d = IUD(1) << 63;
  Check(AlignUp(ISC(low_c), ISC(7)) == low_c, "ISC minimum aligns without signed negation");
  Check(CAlignUp(ISD(low_d), ISD(7)) == low_d, "ISD minimum aligns without signed negation");
  Check(AlignUp(IUC(high_c), IUC(7)) == high_c, "high IUC stays unsigned");
  Check(TAlignUp(IUD(high_d), IUD(7)) == high_d, "high IUD stays unsigned");
}
}  //< namespace AlignmentTest

inline const CHA* Core(const CHA* args) {
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  A_TEST_BEGIN;
  
  D_COUT(Headingf("Testing ArgsToString"));
  
  CHA arg_string[48] = "C:\\Windows\0Foo\0\0Bar    \0\0\0   420    \0";
  D_COUT("arg_string:\"");
  CHA* cursor = arg_string;
  for (ISN i = 0; i < 35; ++i) D_COUT(*cursor++);
  D_COUT('\"');

  CHA* test_args[] = {arg_string, arg_string + 11, arg_string + 16,
                      arg_string + 26};
  const ISC ArgCount = 4;

  D_COUT("\nArguments:\n");
  for (ISC i = 0; i < ArgCount; ++i) {
    CHA* arg = test_args[i];
    if (arg) {
      D_COUT("\n" << i << ".)\"" << arg << "\" delta:" << arg - arg_string);
    } else {
      D_COUT("\nNil arg.");
    }
  }
  D_COUT("\n\nRunning ArgsToString... ");

  const CHA* arg_result = ArgsToString(ArgCount, test_args);
  D_ASSERT(!IsError(arg_result));

  D_COUT("Result:\"" << test_args[1] << '\"');
  D_COUT(Headingf("Testing CHA* SPrint (CHA*,CHA*,CHC);"));
  enum {
    STAEdgeCondition2 = 0x7f,
    STAEdgeCondition3 = 0x7ff,
    STAEdgeCondition4 = 0xffff,
    STACount = 8
  };
  CHA str1[STACount] = {};
  CHA* str1_cursor = SPrint(str1, STACount, CHC(0));
  A_ASSERT(str1_cursor);
  CHC char_found = 0;
  const CHA* str1_result = SScan(str1, char_found);
  D_ASSERT_INDEX(str1_result, ISC(0));
  A_AVOW_INDEX(CHC(0), char_found, 0);

  for (CHC ch4_expected = STAEdgeCondition4; ch4_expected < (1 << 21);
       ch4_expected += 64) {
    str1_cursor = SPrint(str1, STACount, ch4_expected);
    D_ASSERT_INDEX(str1_cursor, IUC(ch4_expected));
    const CHA* str1_result = SScan(str1, char_found);
    D_ASSERT_INDEX(str1_result, IUC(ch4_expected));
    //A_AVOW_INDEX(ch4_expected, char_found, ISC(ch4_expected));
  }

#if USING_STB == YES_0
  D_COUT(Headingf("Testing Testing CHB* SPrint (CHB*,CHB*,CHC)..."));
  enum {
    STRBCount = 4,
  };
  CHB str2[STRBCount] = {};
  CHB* str2_cursor = str2;
  str2_cursor = SPrint(str2, ISW(STRBCount) - 1, CHC(0));
  D_COUT(Hexf(str2) << TPtr<CHA>(str2 + 3) - 1);
  A_ASSERT(str2_cursor);
  const CHB* str2_result = SScan(str2, char_found);
  D_ASSERT_INDEX(str2_result, ISC(0));
  A_AVOW_INDEX(CHC(0), char_found, 0);

  for (CHC ch4_expected = 1; ch4_expected < (1 << 10); ++ch4_expected) {
    str2_cursor = SPrint(str2, 3, ch4_expected);
    D_ASSERT_INDEX(str2_cursor, IUC(ch4_expected));
    const CHB* str2_result = SScan(str2, char_found);
    D_ASSERT_INDEX(str2_result, IUC(ch4_expected));
    A_AVOW_INDEX(ch4_expected, char_found, ISC(ch4_expected));
  }
  
  D_COUT(Headingf("Testing Hex functions"));
  for (ISC i = 0; i < 16; ++i) {
    ISC value = HexToByte(HexNibbleToLowerCase(i));
    TestEq(i, value);
    // D_COUT("\n    " << i << ".) " >> value);
    value = HexToByte(HexNibbleToUpperCase(i));
    // D_COUT(" Result:" << value);
    TestEq(i, value);
  }

  for (ISC i = 0; i < 256; ++i) {
    IUB c = HexByteToLowerCase(i);
    // D_COUT('\n' << i << ".) Expecting:" << Hexf(i)
    //             << "        HexByteToLowerCase:" << CHA(c) << CHA(c >> 8));
    ISC value = HexToByte(c);
    // D_COUT("        HexToByte:" << value);
    A_AVOW(i, value);
    value = HexToByte(HexByteToUpperCase(i));
    // D_COUT(" Result:" << value);
    TestEq(i, value);
  }
#endif

#endif
  return 0;
}

}  //< namespace CRTest
