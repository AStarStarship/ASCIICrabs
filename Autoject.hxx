// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsAutojectHXX
#define _CrabsAutojectHXX
#if _Seam >= _CrabsStack
#include "Autoject.h"
#include "Binary.hpp"
#if _Seam == _CrabsStack
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

IUW* ArrayFactoryStackA(IUW* origin, ISW total) {
  if (total <= 0) return _NilP;
  _DAssert (!origin);
  IUW* socket = new IUW[(total * sizeof(ISA)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapA(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackA(origin, bytes);
  delete[] origin;
  return _NilP;
}

IUW* ArrayFactoryStackB(IUW* origin, ISW total) {
  if (total <= 0) return _NilP;
  _DAssert(!origin);
  IUW* socket = new IUW[(total * sizeof(ISB)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapB(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackB(origin, bytes);
  delete[] origin;
  return _NilP;
}

IUW* ArrayFactoryStackC(IUW* origin, ISW total) {
  if (total <= 0) return _NilP;
  _DAssert(!origin);
  IUW* socket = new IUW[(total * sizeof(ISC)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapC(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackC(origin, bytes);
  delete[] origin;
  return _NilP;
}

IUW* ArrayFactoryStackD(IUW* origin, ISW total) {
  if (total <= 0) return _NilP;
  _DAssert(!origin);
  IUW* socket = new IUW[(total * sizeof(ISD)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapD(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackD(origin, bytes);
  delete[] origin;
  return _NilP;
}

IUW* ObjectFactoryStack(IUW* origin, ISW bytes) {
  if (bytes <= 0) return _NilP;
  _DAssert(!origin);
  bytes += (-bytes) & ACPUWordMask;  //< Word align up.
  IUW* socket = new IUW[bytes >> ACPUBytesLog2];
  return socket;
}

IUW* ObjectFactoryHeap(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ObjectFactoryStack(origin, bytes);
  delete[] origin;
  return _NilP;
}

}  //< namespace _
#endif  //< _Seam >= _CrabsStack
#endif  //< _CrabsAutojectHXX