// Copyright AStarship <https://astarship.net>.
#include "_Undef.h"
//
#include "Test.h"
//
#define _D(code)
#define _R(code) code
#define _ATestBegin
#define _ARunTest(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define _DRunTest(test_unit)
#define _RRunTest(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define _DPause(message)
#define _DROut(item)
#define _DROutNL
#define _DLinef(item)
#define _DROutError(message)
#define _DROutBSQ(item)
#define _DROutBIn(item)
#define _DROutBOut(item)
#define _DROutOBJ(obj)
#define _DROutFunction
#define _DROutFunctionLine
#define _DROutArray(item)
#define _DROutArrayLines(item)
#define _DROutStack(item)
#define _DROutStrings(item)
#define _DROutList(item)
#define _DROutLoom(item)
#define _DROutMap(item)
#define _DROutBook(item)
#define _DROutDic(item)
#define _DROutTable(item)
#define _DROutMatrix(item)
#define _DArraySave(origin, stop)
#define _DArrayFillC(origin, end_or_size, c)
#define _DArrayFill(origin, end_or_size)
#define _DStackFill(origin)
#define _DStackFill(origin)
#define _DObjectFill(origin, obj_class)
#define _DAssertIndex(condition, index)
#define _AAssert(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _DAssert(condition)
#define _AAssertPointer(pointer)                            \
  if (IsError(pointer))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _DAssertPointer(pointer)
#define _RAssertPointer(pointer)                            \
  if (IsError(pointer))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define _AFail() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _RFail() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DFail()
#define _ACheck(condition)
#define _DCheck(condition, what)
#define _DCheckClose(a, b, eps, what)
#define _DCheckString(a, b, what)
#define _DCheckPointer(pointer, what)
#define _DCheckRange(a, lo, hi, what)
#define _RCheck(condition) \
  if (!::_::Test(condition)) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _ACompare(a, b)
#define _DCompare(a, b)
#define _RCompare(a, b)                           \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__) \
  }
#define _AAvow(a, b)                               \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DAvow(a, b)
#define _RAvow(a, b)                              \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define _RAvow(a, b)                              \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define _AAvow(a, b)                               \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DDisavow(a, b)
#define _RDisavow(a, b)                              \
  if (!::_::TestNot(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define _RDisavow(a, b)                              \
  if (!::_::TestNot(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define _AAvowIndex(a, b, index)                  \
  if (!::_::TestEq(a, b)) {                            \
    ::_::RefOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _RAvowIndex(a, b, index)                  \
  if (!::_::TestEq(a, b)) {                            \
    ::_::RefOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define _DAvowIndex(a, b, index)
#define _AWarn(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _DWarn(condition)
#define _RWarn(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define _RReturn(value) return;
#define _DReturn(value)
#define _RReturn(value) \
  { ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__) return; }
#define _DReturnT(type, error) return type(error)
#define _DReturnCPointerError(type, error) return const_cast<type*>(IUW(error))
#define _DReturnTPointerError(type, error) return reinterpret_cast<type*>(IUW(error))
#define _AReturnValue(value) return value;
#define _DReturnValue(value)
#define _RReturnValue(value) \
  { ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__) return; }
#define _RCheckPointerCode(pointer, error_code)\
  if (IUW(pointer) < IUW(AErrorTotal))\
    return error_code;
#define D_CHECK_PTR_RETURN(pointer)\
  if (IUW(pointer) < IUW(AErrorTotal))\
    return pointer;
#define _DCheckCPointerTReturn(type, pointer)\
  if (IUW(pointer) < IUW(AErrorTotal))\
    return const_cast<type*>(pointer);
#define _DCheckTPointerReturn(type, pointer)\
  if (IUW(pointer) < IUW(AErrorTotal))\
    return reinterpret_cast<type*>(pointer);
#define _DCheckPointerTReturn(type, pointer)\
  if (IUW(pointer) < IUW(AErrorTotal))\
    return type(IUW(pointer));
#define _DCheckPointerTReturn2(type, ref1, ref2)\
  if (IUW(ref1) < IUW(AErrorTotal))\
    return type(IUW(ref1));\
  if (IUW(ref2) < IUW(AErrorTotal))\
    return type(IUW(ref2));
#define _DCheckTPointerReturnNil(pointer)\
  if (IUW(pointer) < IUW(AErrorTotal)) return;

#if _Seam <= 0
#define _CrabsFail _DFail
#endif
