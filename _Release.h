// Copyright AStarship <https://astarship.net>.
#include "_Undef.h"
//
#include "Test.h"
//
#define D_(code)
#define R_(code) code
#define A_TEST_BEGIN
#define A_RUN_TEST(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define D_RUN_TEST(test_unit)
#define R_RUN_TEST(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define D_PAUSE(message)
#define D_COUT(item)
#define D_COUT_NL
#define D_LINEF(item)
#define D_COUT_ERROR(message)
#define D_COUT_BSQ(item)
#define D_COUT_BIN(item)
#define D_COUT_BOUT(item)
#define D_COUT_OBJ(obj)
#define D_COUT_FUNCTION
#define D_COUT_FUNCTION_LINE
#define D_COUT_ARRAY(item)
#define D_COUT_ARRAY_LINES(item)
#define D_COUT_STACK(item)
#define D_COUT_STRING(item)
#define D_COUT_LIST(item)
#define D_COUT_LOOM(item)
#define D_COUT_MAP(item)
#define D_COUT_BOOK(item)
#define D_COUT_DIC(item)
#define D_COUT_TABLE(item)
#define D_COUT_MATRIX(item)
#define D_ARRAY_SAVE(origin, stop)
#define D_ARY_FILL(origin, end_or_size, c)
#define D_RAM_WIPE(origin, end_or_size)
#define D_ARY_WIPE(origin)
#define D_SCK_WIPE(origin)
#define D_STR_WIPE(origin)
#define D_OBJ_WIPE(origin, obj_class)
#define D_ASSERT_INDEX(condition, index)
#define A_ASSERT(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define D_ASSERT(condition)
#define A_ASSERT_PTR(ptr)                            \
  if (IsError(ptr))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define D_ASSERT_PTR(ptr)
#define R_ASSERT_PTR(ptr)                            \
  if (IsError(ptr))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define A_FAIL() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define R_FAIL() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define D_FAIL()
#define A_CHECK(condition)
#define D_CHECK(condition)
#define R_CHECK(condition) \
  if (!::_::Test(condition)) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define A_COMPARE(a, b)
#define D_COMPARE(a, b)
#define R_COMPARE(a, b)                           \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__) \
  }
#define A_AVOW(a, b)                               \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_AVOW(a, b)
#define R_AVOW(a, b)                              \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define R_AVOW(a, b)                              \
  if (!::_::TestEq(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define A_AVOW(a, b)                               \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_DISAVOW(a, b)
#define R_DISAVOW(a, b)                              \
  if (!::_::TestNot(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define R_DISAVOW(a, b)                              \
  if (!::_::TestNot(a, b)) {                           \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__) \
  }
#define A_AVOW_INDEX(a, b, index)                  \
  if (!::_::TestEq(a, b)) {                            \
    ::_::StdOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define R_AVOW_INDEX(a, b, index)                  \
  if (!::_::TestEq(a, b)) {                            \
    ::_::StdOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_AVOW_INDEX(a, b, index)
#define A_WARN(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define D_WARN(condition)
#define R_WARN(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define A_RETURN(value) return;
#define D_RETURN(value)
#define R_RETURN(value) \
  { ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__) return; }
#define D_RETURNT(type, error) return type(error)
#define D_RETURN_CPTR_ERROR(type, error) return const_cast<type*>(IUW(error))
#define D_RETURN_TPTR_ERROR(type, error) return reinterpret_cast<type*>(IUW(error))
#define A_RETURN_VALUE(value) return value;
#define D_RETURN_VALUE(value)
#define R_RETURN_VALUE(value) \
  { ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__) return; }
#define D_CHECK_PTR_RETURN_CODE(ptr, error_code)\
  if (IUW(ptr) < IUW(AErrorTotal))\
    return error_code;
#define D_CHECK_PTR_RETURN(ptr)\
  if (IUW(ptr) < IUW(AErrorTotal))\
    return ptr;
#define D_CHECK_CPTR_RETURN(type, ptr)\
  if (IUW(ptr) < IUW(AErrorTotal))\
    return const_cast<type*>(ptr);
#define D_CHECK_TPTR_RETURN(type, ptr)\
  if (IUW(ptr) < IUW(AErrorTotal))\
    return reinterpret_cast<type*>(ptr);
#define D_CHECK_PTR_TRETURN(type, ptr)\
  if (IUW(ptr) < IUW(AErrorTotal))\
    return type(IUW(ptr));
#define D_CHECK_PTR_TRETURN2(type, ptr1, ptr2)\
  if (IUW(ptr1) < IUW(AErrorTotal))\
    return type(IUW(ptr1));\
  if (IUW(ptr2) < IUW(AErrorTotal))\
    return type(IUW(ptr2));
#define D_CHECK_PTR_RETURN_NIL(ptr)\
  if (IUW(ptr) < IUW(AErrorTotal)) return;

#if SEAM <= 0
#define CRABS_FAIL D_FAIL
#endif
