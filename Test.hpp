// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTestHPP
#define _CrabsTestHPP
#include "Test.h"
namespace _ {

template<typename T = BOL>
inline T TBRK(T value = 0, const CHR* message = _NilP) {
  if(!IsError(message)) {
    #if _Seam >= _CrabsCore
    RefOut() << " message:\"" << message << "\"\nvalue:" << value;
    #endif
  }
  return value;  //< Insert break-point here.
}

template<typename T>
inline BOL TTestNZ(T value) {
  if (value != 0) return true;
#if _Seam >= _CrabsCore
  RefOut() << "\n\nERROR: value:" << value << " is nil!";
#endif
  return false;
}

template<typename T>
BOL TTestEq(T a, T b) {
  if (a == b) return true;

#if _Seam >= _CrabsCore
  RefOut() << "\n\nERROR: Expecting: " << a << " > " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestGTEQ(T a, T b) {
  if (a >= b) return true;

#if _Seam >= _CrabsCore
  RefOut() << "\n\nERROR: Expecting: " << a << " >= " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestLT(T a, T b) {
  if (a < b) return true;

#if _Seam >= _CrabsCore
  RefOut() << "\n\nERROR: Expecting: " << a << " < " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestLTEQ
(T a, T b) {
  if (a <= b) return true;

#if _Seam >= _CrabsCore
  RefOut() << "\n\nERROR: Expecting: " << a << " <= " << b
           << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<TestCase... N>
const CHA* TTestTree(const CHA* args) {
  static TestCase nodes[sizeof...(N)] = {N...};
  return TestTree(args, nodes, sizeof...(N));
}

template<TestCase... N>
ISN TTestTree(ISN arg_count, CHA** args) {
  static TestCase tests[sizeof...(N)] = {N...};
  return SeamTreeTest(arg_count, args, tests, sizeof...(N));
}

}  //< namespace _
#endif  //< _CrabsTestHPP
