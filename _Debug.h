// Copyright AStarship <https://astarship.net>.
#include "_Undef.h"
//
#include "Test.h"
// @todo Add O_ macros for optional tests.
#define D_(code) code
#define R_(code)
#define D_THIS 1
#define A_TEST_BEGIN\
  ::_::StdOut() << Headingf("Testing ", __FUNCTION__, NILP, NILP, 80)
#define A_RUN_TEST(test_unit) \
  { const CHA* result = test_unit(); if(result) return result; }
#define D_RUN_TEST(test_unit) \
  { const CHA* = test_unit(); if(result) return result; }
#define R_RUN_TEST(test_unit)
#define D_COUT(item) ::_::StdOut() << item
#define D_COUT_NL ::_::StdOut().NL()
#define D_LINEF(item) ::_::StdOut() << Linef(item)
#define D_PAUSE(message) ::_::Pausef(message)
#define D_COUT_ERROR(message)\
  ::_::StdOut() << "\nERROR: " << message << '.');\
  ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define D_COUT_BSQ(item) ::_::TBSeqPrint<::_::COut>(::StdOut(), item)
#define D_COUT_BIN(item) ::_::TBInPrint<::_::COut>(::StdOut(), item)
#define D_COUT_BOUT(item) ::_::TBOutPrint<::_::COut>(::StdOut(), item)
#define D_COUT_OBJ(obj) obj.template PrintTo<::_::COut>(::StdOut())
#define D_COUT_FUNCTION ::_::COut("\n", __FUNCTION__)
#define D_COUT_FUNCTION_LINE \
  ::_::COut().NL();          \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__)
#define D_COUT_ARRAY(item) \
::_::TArrayPrint<::_::COut, ISZ>(::_::StdOut(), item)
#define D_COUT_ARRAY_LINES(item) \
  ::_::TArrayPrintLines<::_::COut, ISZ>(::_::StdOut(), item)
#define D_COUT_STACK(item) \
  ::_::TStackPrint<::_::COut, T, ISZ>(::_::StdOut(), item)
#define D_COUT_STRING(item) \
  ::_::TStringPrint<::_::COut, T, CHT, ISZ>(::_::StdOut(), item)
#define D_COUT_MATRIX(item) \
  ::_::TMatrixPrint<::_::COut, T, ISZ>(::_::StdOut(), item)
#define D_COUT_LIST(item) \
  ::_::TListPrint<::_::COut, ISZ, ISY, DT>(::StdOut(), item)
#define D_COUT_LOOM(item) \
  ::_::TLoomPrint<::_::COut, CHS, CHT, ISZ, ISY>(::StdOut(), item)
#define D_COUT_BOOK(item) \
  ::_::TBookPrint<::_::COut, CHS, CHT, ISZ, ISY, DT>(::StdOut(), item)
#define D_COUT_MAP(item) ::_::TMapPrint<::_::COut, D, ISZ>(::StdOut(), item)
#define D_COUT_DIC(item) \
  ::_::TDicPrint<::_::COut, CHS, CHT, ISZ, ISY, DT, HSH>(::StdOut(), item)
#define D_COUT_TABLE(item) \
  TTablePrint<::_::COut, CHS, CHT, ISZ, ISY, HSH>(::StdOut(), item)
#define D_ARRAY_SAVE(origin, end_or_size) \
  Socket socket_to_print(origin, end_or_size)
#define D_ARY_FILL(origin, end_or_size, c) \
  ::_::ArrayFill(cursor, end_or_size, c)
#define D_RAM_WIPE(origin, end_or_size) ::_::ArrayFill(origin, end_or_size)
#define D_ARY_WIPE(origin) ::_::TArrayFill<T, ISZ>(origin)
#define D_SCK_WIPE(origin) ::_::TStackFill<T, ISZ, ISY>(origin)
#define D_STR_WIPE(origin) ::_::TStackFill<T, ISZ, ISY>(origin)
#define D_OBJ_WIPE(origin, IS) ::_::TObjectFill<IS>(origin)
#define A_CHECK(condition) \
  if (!::_::Test(condition)) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define A_COMPARE(a, b)                              \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_ASSERT_INDEX(condition, index)                           \
  if (!::_::Test(condition)) {                                     \
    ::_::COut("\n\nERROR at index:0x").Hex(index) << ':' << index; \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__);               \
  }
#define A_ASSERT(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define D_ASSERT(condition) \
  if (!::_::Test(condition)) ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define R_ASSERT(condition)
#define A_ASSERT_PTR(ptr)                            \
  if (IsError(ptr))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define D_ASSERT_PTR(ptr)                            \
  if (IsError(ptr))  \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__)
#define R_ASSERT_PTR(ptr)
#define A_AVOW(a, b)                                 \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_AVOW(a, b)                                 \
  if (!::_::TestEq(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define R_AVOW(a, b)
#define A_DISAVOW(a, b)                             \
  if (!::_::TestNot(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define D_DISAVOW(a, b)                             \
  if (!::_::TestNot(a, b)) {                            \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define R_DISAVOW(a, b)
#define R_AVOW_INDEX(a, b, index)
#define D_AVOW_INDEX(a, b, index)                    \
  if (!::_::TestEq(a, b)) {                            \
    ::_::StdOut() << kSTRIndex << index;               \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define A_AVOW_INDEX(a, b, index)                    \
  if (!::_::TestEq(a, b)) {                            \
    ::_::StdOut() << "\n           Index:" << index;   \
    ::_::TestFail(__LINE__, __FUNCTION__, __FILE__); \
  }
#define A_FAIL() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define D_FAIL() ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define R_FAIL()
#define A_WARN(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define D_WARN(condition) \
  if (!condition) ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__)
#define R_WARN(condition)
#define A_RETURN {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return; }
#define D_RETURN {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return; }
#define R_RETURN(value)
#define D_RETURNT(type, error) { \
  ::_::TestWarn(__LINE__, __FUNCTION__, __FILE__); \
  ::_::StdOut() << "\n\nERROR " << error << ": " << \
    TAErrors<CHR, ISN>(error); \
  return type(error); }
#define D_RETURN_CPTR_ERROR(type, error){ \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::StdOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return const_cast<type*>(error);}
#define D_RETURN_TPTR_ERROR(type, error){ \
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::StdOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return reinterpret_cast<type*>(error);}
#define D_RETURN_PTR_ERROR(error) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); \
  ::_::StdOut() << "\n\nERROR " << ISW(error) << ": " << \
    TAErrors<CHR, ISW>(ISW(error)); \
  return error;}
#define A_RETURN_VALUE(value) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return value; }
#define D_RETURN_VALUE(value) {\
  ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__); return value; }
#define R_RETURN_VALUE(value)
#define D_CHECK_PTR_RETURN_CODE(ptr) \
  if(IUW(ptr) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ptr));\
    return ptr;\
  }
#define D_CHECK_PTR_TRETURN(type, ptr) \
  if(IUW(ptr) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ptr));\
    return type(IUW(ptr));\
  }
#define D_CHECK_PTR_TRETURN2(type, ptr1, ptr2) \
  if(IUW(ptr1) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr1) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ptr2));\
    return type(IUW(ptr1));\
  } \
  if(IUW(ptr2) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr2) << ": " <<\
      TAErrors<CHR, ISW>(ISW(ptr2));\
    return type(IUW(ptr2));\
  }
#define D_CHECK_CPTR_RETURN(type, ptr)\
  if(IUW(ptr) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(ptr));\
    return const_cast<type*>(ptr);\
  }
#define D_CHECK_TPTR_RETURN(type, ptr) \
  if(IUW(ptr) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << IUW(ptr) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(ptr));\
    return reinterpret_cast<type*>(IUW(ptr));\
  }
#define D_CHECK_PTR_RETURN_NIL(ptr) \
  if(IUW(ptr) < AErrorTotal) {\
    ::_::TestFunctionLine(__LINE__, __FUNCTION__, __FILE__);\
    ::_::StdOut() << "\n\nERROR " << reinterpret_cast<ISW>(ptr) << ": " <<\
      TAErrors<CHR, ISW>(reinterpret_cast<ISW>(IUW(ptr)));\
    return;\
  }


#if SEAM < CRABS_SCRIPT
#define CRABS_FAIL D_FAIL
#endif
