// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsCore
#include "../Uniprinter.hpp"
#if _Seam == _CrabsCore
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

namespace AlignmentTest {
static ISW checks = 0, failures = 0;
static void Check(BOL pass, const CHA* name) {
  ++checks;
  if (!pass) { ++failures; if (failures <= 12) { _DROut("FAIL: " << name); _DROutNL; } }
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
#if _Seam >= _CrabsCore
  _ATestBegin;
  
  _DROut(Headingf("Testing ArgsToString"));
  
  CHA arg_string[48] = "C:\\Windows\0Foo\0\0Bar    \0\0\0   420    \0";
  _DROut("arg_string:\"");
  CHA* cursor = arg_string;
  for (ISN i = 0; i < 35; ++i) _DROut(*cursor++);
  _DROut('\"');

  CHA* test_args[] = {arg_string, arg_string + 11, arg_string + 16,
                      arg_string + 26};
  const ISC ArgCount = 4;

  _DROut("\nArguments:\n");
  for (ISC i = 0; i < ArgCount; ++i) {
    CHA* arg = test_args[i];
    if (arg) {
      _DROut("\n" << i << ".)\"" << arg << "\" delta:" << arg - arg_string);
    } else {
      _DROut("\nNil arg.");
    }
  }
  _DROut("\n\nRunning ArgsToString... ");

  const CHA* arg_result = ArgsToString(ArgCount, test_args);
  _DAssert(!IsError(arg_result));

  _DROut("Result:\"" << test_args[1] << '\"');
  _DROut(Headingf("Testing CHA* SPrint (CHA*,CHA*,CHC);"));
  enum {
    STAEdgeCondition2 = 0x7f,
    STAEdgeCondition3 = 0x7ff,
    STAEdgeCondition4 = 0xffff,
    STACount = 8
  };
  CHA str1[STACount] = {};
  CHA* str1_cursor = SPrint(str1, STACount, CHC(0));
  _AAssert(str1_cursor);
  CHC char_found = 0;
  const CHA* str1_result = SScan(str1, char_found);
  _DAssertIndex(str1_result, ISC(0));
  _AAvowIndex(CHC(0), char_found, 0);

  for (CHC ch4_expected = STAEdgeCondition4; ch4_expected < (1 << 21);
       ch4_expected += 64) {
    str1_cursor = SPrint(str1, STACount, ch4_expected);
    _DAssertIndex(str1_cursor, IUC(ch4_expected));
    const CHA* str1_result = SScan(str1, char_found);
    _DAssertIndex(str1_result, IUC(ch4_expected));
    //_AAvowIndex(ch4_expected, char_found, ISC(ch4_expected));
  }

  _DROut(Headingf("Testing Testing CHB* SPrint (CHB*,CHB*,CHC)..."));
  enum {
    STRBCount = 4,
  };
  CHB str2[STRBCount] = {};
  CHB* str2_cursor = str2;
  str2_cursor = SPrint(str2, ISW(STRBCount) - 1, CHC(0));
  _DROut(Hexf(str2) << TRef<CHA>(str2 + 3) - 1);
  _AAssert(str2_cursor);
  const CHB* str2_result = SScan(str2, char_found);
  _DAssertIndex(str2_result, ISC(0));
  _AAvowIndex(CHC(0), char_found, 0);

  for (CHC ch4_expected = 1; ch4_expected < (1 << 10); ++ch4_expected) {
    str2_cursor = SPrint(str2, 3, ch4_expected);
    _DAssertIndex(str2_cursor, IUC(ch4_expected));
    const CHB* str2_result = SScan(str2, char_found);
    _DAssertIndex(str2_result, IUC(ch4_expected));
    _AAvowIndex(ch4_expected, char_found, ISC(ch4_expected));
  }
  
  _DROut(Headingf("Testing Hex functions"));
  for (ISC i = 0; i < 16; ++i) {
    ISC value = HexToByte(HexNibbleToLowerCase(i));
    TestEq(i, value);
    // _DROut("\n    " << i << ".) " >> value);
    value = HexToByte(HexNibbleToUpperCase(i));
    // _DROut(" Result:" << value);
    TestEq(i, value);
  }

  for (ISC i = 0; i < 256; ++i) {
    IUB c = HexByteToLowerCase(i);
    // _DROut('\n' << i << ".) Expecting:" << Hexf(i)
    //             << "        HexByteToLowerCase:" << CHA(c) << CHA(c >> 8));
    ISC value = HexToByte(c);
    // _DROut("        HexToByte:" << value);
    _AAvow(i, value);
    value = HexToByte(HexByteToUpperCase(i));
    // _DROut(" Result:" << value);
    TestEq(i, value);
  }

#endif
  return 0;
}

}  //< namespace CRTest
