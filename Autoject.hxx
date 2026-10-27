// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_AUTOJECT_HXX
#define CRABS_AUTOJECT_HXX
#if SEAM >= CRABS_STACK
#include "Autoject.h"
#include "Binary.hpp"
#if SEAM == CRABS_STACK
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

IUW* ArrayFactoryStackA(IUW* origin, ISW total) {
  if (total <= 0) return NILP;
  D_ASSERT (!origin);
  IUW* socket = new IUW[(total * sizeof(ISA)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapA(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackA(origin, bytes);
  delete[] origin;
  return NILP;
}

IUW* ArrayFactoryStackB(IUW* origin, ISW total) {
  if (total <= 0) return NILP;
  D_ASSERT(!origin);
  IUW* socket = new IUW[(total * sizeof(ISB)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapB(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackB(origin, bytes);
  delete[] origin;
  return NILP;
}

IUW* ArrayFactoryStackC(IUW* origin, ISW total) {
  if (total <= 0) return NILP;
  D_ASSERT(!origin);
  IUW* socket = new IUW[(total * sizeof(ISC)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapC(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackC(origin, bytes);
  delete[] origin;
  return NILP;
}

IUW* ArrayFactoryStackD(IUW* origin, ISW total) {
  if (total <= 0) return NILP;
  D_ASSERT(!origin);
  IUW* socket = new IUW[(total * sizeof(ISD)) >> ACPUBytesLog2];
  return socket;
}

IUW* ArrayFactoryHeapD(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ArrayFactoryStackD(origin, bytes);
  delete[] origin;
  return NILP;
}

IUW* ObjectFactoryStack(IUW* origin, ISW bytes) {
  if (bytes <= 0) return NILP;
  D_ASSERT(!origin);
  bytes += (-bytes) & ACPUWordMask;  //< Word align up.
  IUW* socket = new IUW[bytes >> ACPUBytesLog2];
  return socket;
}

IUW* ObjectFactoryHeap(IUW* origin, ISW bytes) {
  if (IsError(origin)) return ObjectFactoryStack(origin, bytes);
  delete[] origin;
  return NILP;
}

}  //< namespace _
#endif  // SEAM >= CRABS_STACK
#endif  // CRABS_AUTOJECT_HXX