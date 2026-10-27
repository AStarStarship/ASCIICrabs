// Copyright AStarship <https://astarship.net>.
#include "Test.hpp"
#include "COut.h"
#include "Stringf.hpp"
namespace _ {

inline BOL BRK(BOL value, const CHR* message) {
  return TBRK<BOL>(value, message);
}
inline BOL* BRK(BOL* value, const CHR* message) {
  TBRK<BOL>(*value, message);
  return value;
}
inline const BOL* BRK(const BOL* value, const CHR* message) {
  TBRK<BOL>(*value, message);
  return value;
}

inline IUA BRK(IUA value, const CHR* message) {
  return TBRK<IUA>(value, message);
}
inline IUA* BRK(IUA* value, const CHR* message) {
  TBRK<IUA>(*value, message);
  return value;
}
inline const IUA* BRK(const IUA* value, const CHR* message) {
  TBRK<IUA>(*value, message);
  return value;
}

inline ISA BRK(ISA value, const CHR* message) {
  return TBRK<ISA>(value, message);
}
inline ISA* BRK(ISA* value, const CHR* message) {
  TBRK<ISA>(*value, message);
  return value;
}
inline const ISA* BRK(const ISA* value, const CHR* message) {
  TBRK<ISA>(*value, message);
  return value;
}

inline IUB BRK(IUB value, const CHR* message) {
  return TBRK<IUB>(value, message);
}
inline IUB* BRK(IUB* value, const CHR* message) {
  TBRK<IUB>(*value, message);
  return value;
}
inline const IUB* BRK(const IUB* value, const CHR* message) {
  TBRK<IUB>(*value, message);
  return value;
}

inline ISB BRK(ISB value, const CHR* message) {
  return TBRK<ISB>(value, message);
}
inline ISB* BRK(ISB* value, const CHR* message) {
  TBRK<ISB>(*value, message);
  return value;
}
inline const ISB* BRK(const ISB* value, const CHR* message) {
  TBRK<ISB>(*value, message);
  return value;
}

inline IUC BRK(IUC value, const CHR* message) {
  return TBRK<IUC>(value, message);
}
inline IUC* BRK(IUC* value, const CHR* message) {
  TBRK<IUC>(*value, message);
  return value;
}
inline const IUC* BRK(const IUC* value, const CHR* message) {
  TBRK<IUC>(*value, message);
  return value;
}

inline ISC BRK(ISC value, const CHR* message) {
  return TBRK<ISC>(value, message);
}
inline ISC* BRK(ISC* value, const CHR* message) {
  TBRK<ISC>(*value, message);
  return value;
}
inline const ISC* BRK(const ISC* value, const CHR* message) {
  TBRK<ISC>(*value, message);
  return value;
}

inline IUD BRK(IUD value, const CHR* message) {
  return TBRK<IUD>(value, message);
}
inline IUD* BRK(IUD* value, const CHR* message) {
  TBRK<IUD>(*value, message);
  return value;
}
inline const IUD* BRK(const IUD* value, const CHR* message) {
  TBRK<IUD>(*value, message);
  return value;
}

inline ISD BRK(ISD value, const CHR* message) {
  return TBRK<ISD>(value, message);
}
inline ISD* BRK(ISD* value, const CHR* message) {
  TBRK<ISD>(*value, message);
  return value;
}
inline const ISD* BRK(const ISD* value, const CHR* message) {
  TBRK<ISD>(*value, message);
  return value;
}

//inline IUE BreakPoint(IUE value, const CHR* message) {
//  return TBreakPoint<IUE>(value, message);
//}
//inline IUE* BRK(IUE* value, const CHR* message) {
//  TBRK<IUE>(*value, message);
//  return value;
//}
//inline const IUE* BRK(const IUE* value, const CHR* message) {
//  TBRK<IUE>(*value, message);
//  return value;
//}
//
//inline ISE BreakPoint(ISE value, const CHR* message) {
//  return TBreakPoint<ISE>(value, message);
//}
//
//inline ISE* BRK(ISE* value, const CHR* message) {
//  TBRK<ISE>(*value, message);
//  return value;
//}
//inline const ISE* BRK(const ISE* value, const CHR* message) {
//  TBRK<ISE>(*value, message);
//  return value;
//}

inline FPC BRK(FPC value, const CHR* message) {
  return TBRK<FPC>(value, message);
}
inline FPC* BRK(FPC* value, const CHR* message) {
  TBRK<FPC>(*value, message);
  return value;
}
inline const FPC* BRK(const FPC* value, const CHR* message) {
  TBRK<FPC>(*value, message);
  return value;
}

inline FPD BRK(FPD value, const CHR* message) {
  return TBRK<FPD>(value, message);
}
inline FPD* BRK(FPD* value, const CHR* message) {
  TBRK<FPD>(*value, message);
  return value;
}
inline const FPD* BRK(const FPD* value, const CHR* message) {
  TBRK<FPD>(*value, message);
  return value;
}

inline CHA BRK(CHA value, const CHR* message) {
  return TBRK<CHA>(value, message);
}
inline CHA* BRK(CHA* value, const CHR* message) {
  return TBRK<CHA*>(value, message);
}
inline const CHA* BRK(const CHA* value, const CHR* message) {
  return TBRK<const CHA*>(value, message);
}

inline CHB BRK(CHB value, const CHR* message) {
  return TBRK<CHB>(value, message);
}
inline CHB* BRK(CHB* value, const CHR* message) {
  return TBRK<CHB*>(value, message);
}
inline const CHB* BRK(const CHB* value, const CHR* message) {
  return TBRK<const CHB*>(value, message);
}

inline CHC BRK(CHC value, const CHR* message) {
  return TBRK<CHC>(value, message);
}
inline CHC* BRK(CHC* value, const CHR* message) {
  return TBRK<CHC*>(value, message);
}
inline const CHC* BRK(const CHC* value, const CHR* message) {
  return TBRK<const CHC*>(value, message);
}

BOL TestWarnFail(ISN line, const CHA* function, const CHA* file) {
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nFAILURE Function:" << function << " at line:" << line <<
    "\n       " << " in \"" << file << '\"';
#endif
  return false;
}

BOL TestFail(ISN line, const CHA* function, const CHA* file) {
  BOL result = TestWarnFail(line, function, file);
  return TBRK<>();
}

void TestFunctionLine(ISN line, const CHA* function, const CHA* file) {
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n        Function:" << function
           << "\n            Line:" << line << " in \"" << file << '\"';
#endif
}

BOL TestWarn(ISN line, const CHA* function, const CHA* file) {
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nWARNING Function:" << function << " at line:" << line <<
       "\n       " << " in \"" << file << '\"';
#endif
  return true;
}

const CHA* ArgsToString(ISN arg_count, CHA** args) {
  if (IsError(args) || arg_count <= 1) return "";
  if (arg_count == 2) return args[1];
  CHA* origin = args[1],
     * stop = args[arg_count - 1] - 1;
  while (stop > origin) {
    CHA c = *stop;
    if (!c) *stop = ' ';
    --stop;
  }
  return origin;
}

const CHA* TestTree(const CHA* args, TestCase* tests, ISN count) {
  if (IsError(tests)) return NILP;
  for (ISN i = 0; i < count; ++i) {
    TestCase test = tests[i];
    if (IsError(test)) {
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
      COut("\nError: seam node ").Print(i) << " is missing!";
#endif
      return "";
    }
    const CHA* error = test(args);
    if (error) return error;
  }
  return NILP;
}

ISN SeamResult(const CHA* result) {
  if (result) {
    #if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
    COut("\n\nError in seam ").Print(result);
    #endif
    return APP_EXIT_FAILURE;
  }
  #if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nUnit tests completed successfully! (:-)+=<\n");
  #endif
  return APP_EXIT_SUCCESS;
}

ISN SeamTreeTest(ISN arg_count, CHA** args, TestCase* tests, ISN test_count) {
  return SeamResult(TestTree(ArgsToString(arg_count, args), tests, test_count));
}

BOL Test(BOL condition) { return condition; }

BOL TestEQ(ISA a, ISA b) { return a >= b; }
BOL TestEQ(ISB a, ISB b) { return a >= b; }
BOL TestEQ(ISC a, ISC b) { return a >= b; }
BOL TestEQ(ISD a, ISD b) { return a >= b; }

BOL TestGEQ(ISA a, ISA b) { return a >= b; }
BOL TestGEQ(ISB a, ISB b) { return a >= b; }
BOL TestGEQ(ISC a, ISC b) { return a >= b; }
BOL TestGEQ(ISD a, ISD b) { return a >= b; }

BOL TestLEQ(IUA a, IUA b) { return TTestLTEQ<IUA>(a, b); }
BOL TestLEQ(ISA a, ISA b) { return TTestLTEQ<ISA>(a, b); }
BOL TestLEQ(IUB a, IUB b) { return TTestLTEQ<IUB>(a, b); }
BOL TestLEQ(ISB a, ISB b) { return TTestLTEQ<ISB>(a, b); }
BOL TestLEQ(IUC a, IUC b) { return TTestLTEQ<IUC>(a, b); }
BOL TestLEQ(ISC a, ISC b) { return TTestLTEQ<ISC>(a, b); }
BOL TestLEQ(IUD a, IUD b) { return TTestLTEQ<IUD>(a, b); }
BOL TestLEQ(ISD a, ISD b) { return TTestLTEQ<ISD>(a, b); }
BOL TestLEQ(FPC a, FPC b) { return TTestLTEQ<FPC>(a, b); }

static const CHA STRErrorExpecting[] = "\n\nERROR: Expecting:0x\0";
static const CHA STRFound[] = "\n           Found:0x\0";
//static const CHA STRErrorNil[] = "\nERROR: value was nil!\0";

BOL TestEq(const CHA* a, const CHA* b) {
  ISN difference = TSCompare<CHA>(a, b);
  if (!difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}

#if USING_STB == YES_0
BOL TestEq(const CHB* a, const CHB* b) {
  ISN difference = TSCompare<CHB>(a, b);
  if (!difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}
#endif

#if USING_STC == YES_0
BOL TestEq(const CHC* a, const CHC* b) {
  ISN difference = TSCompare<CHC>(a, b);
  if (!difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}
#endif

BOL TestEq(CHA a, CHA b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(CHB a, CHB b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(CHC a, CHC b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(const void* a, const void* b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a) << STRFound << Hexf(b);
#endif
  return false;
}

BOL TestEq(IUA a, IUA b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(ISA a, ISA b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(IUB a, IUB b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(ISB a, ISB b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(IUC a, IUC b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(ISC a, ISC b) {
  if (a == b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(IUD a, IUD b) {
  if (a == b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestEq(ISD a, ISD b) {
  if (a == b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

#if USING_FPC == YES_0
BOL TestEq(FPC a, FPC b) {
  if (a == b) return true;
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
  return false;
}
#endif
#if USING_FPD == YES_0
BOL TestEq(FPD a, FPD b) {
  if (a == b) return true;
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
  return false;
}
#endif



BOL TestNot(const CHA* a, const CHA* b) {
  ISN difference = TSCompare<CHA>(a, b);
  if (difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}

#if USING_STB == YES_0
BOL TestNot(const CHB* a, const CHB* b) {
  ISN difference = TSCompare<CHB>(a, b);
  if (difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}
#endif
#if USING_STC == YES_0
BOL TestNot(const CHC* a, const CHC* b) {
  ISN difference = TSCompare<CHC>(a, b);
  if (difference) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut("\n\nERROR: Expecting:\"").Print(a)
      << "\"\n           Found:\"" << b << "\"\n      Difference:\0"
      << difference;
#endif
  return false;
}
#endif
BOL TestNot(CHA a, CHA b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(CHB a, CHB b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(CHC a, CHC b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(const void* a, const void* b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a) << STRFound << Hexf(b);
#endif
  return false;
}

BOL TestNot(IUA a, IUA b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(ISA a, ISA b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(IUB a, IUB b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(ISB a, ISB b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(IUC a, IUC b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(ISC a, ISC b) {
  if (a != b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(IUD a, IUD b) {
  if (a != b) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

BOL TestNot(ISD a, ISD b) {
  if (a != b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
#endif
  return false;
}

#if USING_FPC == YES_0
BOL TestNot(FPC a, FPC b) {
  if (a != b) return true;
  COut(STRErrorExpecting).Hex(a) << ':' << a << STRFound << Hexf(b) << ':' 
                                 << b;
  return false;
}
#endif
#if USING_FPD == YES_0
BOL TestNot(FPD a, FPD b) {
  if (a != b) return true;
  COut(STRErrorExpecting).Hex(a)
      << ':' << a << STRFound << Hexf(b) << ':' << b;
  return false;
}
#endif

inline BOL Test(const void* value) { return ISW(value) >= AErrorTotal; }

inline BOL Test(IUA value) { return TTestNZ<IUA>(value); }

inline BOL Test(ISA value) { return TTestNZ<ISA>(value); }

inline BOL Test(IUB value) { return TTestNZ<IUB>(value); }

inline BOL Test(ISB value) { return TTestNZ<ISB>(value); }

inline BOL Test(IUC value) { return TTestNZ<IUC>(value); }

inline BOL Test(ISC value) { return TTestNZ<ISC>(value); }

inline BOL Test(IUD value) { return TTestNZ<IUD>(value); }

inline BOL Test(ISD value) { return TTestNZ<ISD>(value); }

inline BOL Test(FPC value) { return TTestNZ<FPC>(value); }

inline BOL Test(FPD value) { return TTestNZ<FPD>(value); }

}  //< namespace _
