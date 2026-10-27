// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TEST_HPP
#define CRABS_TEST_HPP
#include "Test.h"
namespace _ {

template<typename T = BOL>
inline T TBRK(T value = 0, const CHR* message = nullptr) {
  if(!IsError(message)) {
    #if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
    StdOut() << " message:\"" << message << "\"\nvalue:" << value;
    #endif
  }
  return value;  //< Insert break-point here.
}

template<typename T>
inline BOL TTestNZ(T value) {
  if (value != 0) return true;
#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nERROR: value:" << value << " is nil!";
#endif
  return false;
}

template<typename T>
BOL TTestEq(T a, T b) {
  if (a == b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nERROR: Expecting: " << a << " > " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestGTEQ(T a, T b) {
  if (a >= b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nERROR: Expecting: " << a << " >= " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestLT(T a, T b) {
  if (a < b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nERROR: Expecting: " << a << " < " << b
    << "\n      Difference:\0" << (b - a);
#endif
  return false;
}

template<typename T>
BOL TTestLTEQ
(T a, T b) {
  if (a <= b) return true;

#if SEAM >= CRABS_CORE && USING_CONSOLE == YES_0
  StdOut() << "\n\nERROR: Expecting: " << a << " <= " << b
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
#endif
