// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsVarintHPP
#define _CrabsVarintHPP
#include <_Config.h>
#if _Seam >= _CrabsDictionary
namespace _ {

#if _CPUSize == _CPU16Bit
inline ISB UnpackISV(ISB value) {
  if (value < 0) {
    IUB result = 0x8000 return result | ~(value - 1);
  }
  return value;
}

inline ISB PackVSI(ISB value) {
  if (value < 0) {
    IUB result = 1 << 15;
    return result | ((~value + 1) << 1);
  }
  return value;
}
#else
inline ISC VSCUnpack(ISC value) {
  if (value < 0) {
    IUC result = 0x80000000;
    return result | ~(value - 1);
  }
  return value;
}

inline ISC VSCPack(ISC value) {
  if (value < 0) {
    ISC result = 0x80000000;
    return result | ((~value + 1) << 1);
  }
  return value;
}
#endif

inline ISD VSDUnpack(ISD value) {
  if (value < 0) {
    ISD result = 0x8000000000000000;
    return result | ~(value - 1);
  }
  return value;
}

inline ISD VSDPack(ISD value) {
  if (value < 0) {
    ISD result = 0x8000000000000000;
    return result | ((~value + 1) << 1);
  }
  return value;
}

template<typename IS = ISW>
inline IS TVarintPack(IS value) {
  if (value < 0) {
    IS result = IS(-1) >> 1;
    return result | ((~value + 1) << 1);
  }
  return value;
}

}  //< namespace _
#endif  //< _Seam >= _CrabsDictionary
#endif  //< _CrabsVarintHPP