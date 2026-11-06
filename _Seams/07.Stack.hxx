// Copyright AStarship <https://astarship.net>.
#include "../Stack.hpp"
#if _Seam >= _CrabsStack
#if _Seam == _CrabsStack
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif
namespace CRTest {
using namespace ::_;

#if _Seam >= _CrabsStack
template<typename T, typename ISZ, typename ISY=ISZ>
void TestStack(const CHA* args) {
  _DROut("Testing AStack<T" << CSizeCodef<T>() << ",IS" <<
         CSizeCodef<ISZ>() << ">...\n");

  AStack<T, ISZ, ISZ, 8> stack;

  _DROut(Linef("\n+---\n| AStack: total:") << stack.Total() << 
         " count:" << stack.Count() << " bytes:" << stack.CBytes() <<
         " size_words:" << stack.SizeWords());
  _DROutOBJ(stack);

  enum { TestCount = 32 };
  _DROut("\n\nPushing " << TestCount << " items on to the Stack...\n");
  T i;
  for (i = 0; i < TestCount; ++i) {
    _DROut("\n| " << i << ".) count:" << stack.Count() <<
           Linef("\n\n+---\n| ") << i << ".) ");
    auto result = stack.Push('0' + i);
    _DROut("\n| Result:" << result);
    _AAssert(result <= stack.Total());
    _DROutOBJ(stack);
    _DAvow(ISZ(i), result);
  }

  _DROut(Headingf("\n\nPopping items off the Stack... ") << "i:" << i << 'n');
  #if _Seam == _CrabsStack
  TStackPrint<::_::ROut, SCK_P>(RefOut(), stack.This());
  #endif
  for (i=i-1; i >= 0; --i) {
    _DROutOBJ(stack);
    auto value = stack.Pop();
    _AAvowIndex(i, value - '0', i);
    _DROut("\n| " << i << ".) Popping " << value << " count:" << stack.Count());
  }
  _DROut("\n\nEnding stack.Count(): " << stack.Count() << " i: " << 
         i << '\n');
  _DROutOBJ(stack);
  _AAssert(stack.Count() == 0);
}
#endif

static const CHA* Stack(const CHA* args) {
#if _Seam >= _CrabsStack
  _ATestBegin;
  _DROut(Headingf("TestStack"));

  // TestStack<ISC, ISA>(args); //< Illegal!
  TestStack<ISC, ISC>(args);
  TestStack<ISD, ISD>(args);
  TestStack<ISD, ISC>(args);
#endif
  return _NilP;
}
}  //< namespace CRTest
