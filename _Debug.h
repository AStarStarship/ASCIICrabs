// Copyright AStarship <https://astarship.net>.
#include "_Undef.h"
//
#include "Test.h"
// @todo Add O_ macros for optional tests.
#define _D(code) code
#define _R(code)
#define _DThis 1
#define _ATestBegin\
  ::_::RefOut() << Headingf("Testing ", __FUNCTION__, _NilP, _NilP, 80)
#define _ARunTest(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define _DRunTest(test_unit) \
  { const CHA* = test_unit(); if(result) return result; }
#define _RRunTest(test_unit)
#define _DROut(item) ::_::RefOut() << item
#define _DROutNL ::_::RefOut().NL()
#define _DLinef(item) ::_::RefOut() << Linef(item)
#define _DPause(message) ::_::Pausef(message)
#define _DROutError(message)\
  ::_::RefOut() << "\nERROR: " << message << '.';\
  ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DROutBSQ(item) ::_::TBSeqPrint<::_::ROut>(::RefOut(), item)
#define _DROutBIn(item) ::_::TBInPrint<::_::ROut>(::RefOut(), item)
#define _DROutBOut(item) ::_::TBOutPrint<::_::ROut>(::RefOut(), item)
#define _DROutOBJ(obj) obj.template PrintTo<::_::ROut>(::RefOut())
#define _DROutFunction ::_::ROut("\n", __FUNCTION__)
#define _DROutFunctionLine \
  ::_::ROut().NL();          \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__)
#define _DROutArray(item) \
::_::TArrayPrint<::_::ROut, ISZ>(::_::RefOut(), item)
#define _DROutArrayLines(item) \
  ::_::TArrayPrintLines<::_::ROut, ISZ>(::_::RefOut(), item)
#define _DROutStack(item) \
  ::_::TStackPrint<::_::ROut, T, ISZ>(::_::RefOut(), item)
#define _DROutStrings(item) \
  ::_::TStringPrint<::_::ROut, T, CHT, ISZ>(::_::RefOut(), item)
#define _DROutMatrix(item) \
  ::_::TMatrixPrint<::_::ROut, T, ISZ>(::_::RefOut(), item)
#define _DROutList(item) \
  ::_::TListPrint<::_::ROut, ISZ, ISY, DT>(::RefOut(), item)
#define _DROutLoom(item) \
  ::_::TLoomPrint<::_::ROut, CHS, CHT, ISZ, ISY>(::RefOut(), item)
#define _DROutBook(item) \
  ::_::TBookPrint<::_::ROut, CHS, CHT, ISZ, ISY, DT>(::RefOut(), item)
#define _DROutMap(item) ::_::TMapPrint<::_::ROut, D, ISZ>(::RefOut(), item)
#define _DROutDic(item) \
  ::_::TDicPrint<::_::ROut, CHS, CHT, ISZ, ISY, DT, HSH>(::RefOut(), item)
#define _DROutTable(item) \
  TTablePrint<::_::ROut, CHS, CHT, ISZ, ISY, HSH>(::RefOut(), item)
#define _DArraySave(origin, end_or_size) \
  Socket socket_to_print(origin, end_or_size)
#define _DArrayFillC(origin, end_or_size, c) \
  ::_::ArrayFill(cursor, end_or_size, c)
#define _DArrayFill(origin, end_or_size) ::_::ArrayFill(origin, end_or_size)
#define _DStackFill(origin) ::_::TStackFill<T, ISZ, ISY>(origin)
#define _DObjectFill(origin, ObjType) ::_::TObjectFill<ObjType>(origin)
#define _ACheck(condition) \
  if (!::_::Test(condition)) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DCheck(condition, what) \
  if (::_::Test(condition)) { ::_::RefOut() << "PASS " << what << '\n'; } \
  else { ::_::RefOut() << "FAIL " << what << '\n'; \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); }
#define _DCheckClose(a, b, eps, what) \
  do { FPC _d = (FPC)(a) - (FPC)(b); if (_d < 0) _d = -_d; \
    if (::_::Test(_d < (FPC)(eps))) { ::_::RefOut() << "PASS " << what << '\n'; } \
    else { ::_::RefOut() << "FAIL " << what << " (" << (a) << " vs " << (b) << ")\n"; \
      ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); } } while (0)
#define _DCheckString(a, b, what) \
  if (::_::TSEquals(a, b) != _NilP) { ::_::RefOut() << "PASS " << what << '\n'; } \
  else { ::_::RefOut() << "FAIL " << what << " (\"" << (a) << "\" vs \"" << (b) << "\")\n"; \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); }
#define _DCheckPointer(pointer, what) \
  if (!IsError(pointer)) { ::_::RefOut() << "PASS " << what << '\n'; } \
  else { ::_::RefOut() << "FAIL " << what << " (nil/error pointer)\n"; \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); }
#define _DCheckRange(a, lo, hi, what) \
  do { FPC _v = (FPC)(a); \
    if (::_::Test(_v >= (FPC)(lo) && _v <= (FPC)(hi))) { ::_::RefOut() << "PASS " << what << '\n'; } \
    else { ::_::RefOut() << "FAIL " << what << " (" << (a) << " not in [" << (lo) << "," << (hi) << "])\n"; \
      ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); } } while (0)
#define _ACompare(a, b)                              \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DAssertIndex(condition, index)                           \
  if (!::_::Test(condition)) {                                     \
    ::_::ROut("\n\nERROR at index:0x").Hex(index) << ':' << index; \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__);               \
  }
#define _AAssert(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _DAssert(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _RAssert(condition)
#define _AAssertPointer(pointer)                            \
  if (IsError(pointer))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _DAssertPointer(pointer)                            \
  if (IsError(pointer))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _RAssertPointer(pointer)
#define _AAvow(a, b)                                 \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DAvow(a, b)                                 \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _RAvow(a, b)
#define _ADisavow(a, b)                             \
  if (!::_::TestNot(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DDisavow(a, b)                             \
  if (!::_::TestNot(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _RDisavow(a, b)
#define _RAvowIndex(a, b, index)
#define _DAvowIndex(a, b, index)                    \
  if (!::_::TestEq(a, b)) {                            \
    ::_::RefOut() << STRIndex << index;               \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _AAvowIndex(a, b, index)                    \
  if (!::_::TestEq(a, b)) {                            \
    ::_::RefOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _AFail() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DFail() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _RFail()
#define _AWarn(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DWarn(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _RWarn(condition)
#define _RReturn {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return; }
#define _DReturn {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return; }
#define _RReturn(value)
#define _DReturnT(type, error) { \
  ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); \
  ::_::RefOut() << "\n\nERROR " << error << ": " << \
    TAErrors<CHR, ISN>(error); \
  return type(error); }
#define _DReturnCPointerError(type, error){ \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::RefOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return const_cast<type*>(error);}
#define _DReturnTPointerError(type, error){ \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::RefOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return reinterpret_cast<type*>(error);}
#define _DReturnPointerError(error) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::RefOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return error;}
#define _AReturnValue(value) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return value; }
#define _DReturnValue(value) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return value; }
#define _RReturnValue(value)
#define _RCheckPointerCode(pointer) \
  if(IUW(pointer) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(pointer) << ": " <<\
      TAErrors<CHR, ISW>(ISW(pointer));\
    return pointer;\
  }
#define _DCheckPointerTReturn(type, pointer) \
  if(IUW(pointer) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(pointer) << ": " <<\
      TAErrors<CHR, ISW>(ISW(pointer));\
    return type(IUW(pointer));\
  }
#define _DCheckPointerTReturn2(type, ref11, ref12) \
  if(IUW(ref11) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(ref11) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ref12));\
    return type(IUW(ref11));\
  } \
  if(IUW(ref12) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(ref12) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ref12));\
    return type(IUW(ref12));\
  }
#define _DCheckCPointerTReturn(type, pointer)\
  if(IUW(pointer) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(pointer) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(pointer));\
    return const_cast<type*>(pointer);\
  }
#define _DCheckTPointerReturn(type, pointer) \
  if(IUW(pointer) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << IUW(pointer) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(pointer));\
    return reinterpret_cast<type*>(IUW(pointer));\
  }
#define _DCheckTPointerReturnNil(pointer) \
  if(IUW(pointer) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::RefOut() << "\n\nERROR " << reinterpret_cast<ISW>(pointer) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(IUW(pointer)));\
    return;\
  }


#if _Seam < _CrabsScript
#define _CrabsFail _DFail
#endif
