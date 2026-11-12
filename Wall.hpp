// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsWallHPP
#define _CrabsWallHPP
#include <_Config.h>
#if _Seam >= _CrabsDictionary
#include "AType.hpp"
// Op.hpp no longer needed here - Door.hpp provides forward declarations
// and all template structs are in AType.hpp
/* @defgroup Wall
Chinese Room Wall
A block of dynamic memory in a Chinese Room. Walls provide
heap space when the Floor (static memory) is exhausted.
A Wall is like a Room in structure, but always heap-resident.
Its root is a List: fastest index-based lookup + stack pushing,
no hashtable by default. Hashtables and Doors are added to the
root List when that helps.
@{ */

namespace _ {
  
enum {
  WallBytesMin = 512,  //< Min functional Wall size.
};

template<WAL_A>
TWall<WAL_P>* TWallInit(TWall<WAL_P>* wall, ISW bytes = WallBytesMin,
                        ISY total = 16) {
  wall->is_dynamic_ = true;
  bytes = bytes < WallBytesMin ? ISW(WallBytesMin) : bytes;
  ISW size_words = (bytes >> ACPUBytesLog2) + 3;
  IUW* socket = new IUW[size_words];
  LST* root = TRef<LST>(socket);
  root->bytes = ISZ(bytes);
  wall->list_ = TListInit<LST_P>(root, total);
  return wall;
}

/* Builds a Wall over a given preallocated block. */
template<WAL_A>
TWall<WAL_P>* TWallInit(TWall<WAL_P>* wall, IUW* socket, ISW bytes,
                        ISY total) {
  wall->is_dynamic_ = false;
  LST* root = TRef<LST>(socket);
  root->bytes = ISZ(bytes);
  wall->list_ = TListInit<LST_P>(root, total);
  return wall;
}

/* The Wall's root List: TListAlloc into it; TListValue to read back. */
template<WAL_A>
LST* TWallRoot(TWall<WAL_P>* wall) { return wall->list_; }

/* The Wall's size in bytes. */
template<WAL_A>
ISW TWallGetSizeBytes(TWall<WAL_P>* wall) { return wall->list_->bytes; }

/* Frees a Wall that bought its own block and leaves it empty. A Wall built
over a caller's socket is left alone: that memory is not the Wall's to give
back. */
template<WAL_A>
void TWallRelease(TWall<WAL_P>* wall) {
  if (wall == _NilP || !wall->is_dynamic_ || wall->list_ == _NilP) return;
  delete[] TRef<IUW>(wall->list_);
  wall->list_ = _NilP;
  wall->is_dynamic_ = false;
}

}  //< namespace _

/* @} */  // End of Wall group.
#endif  //< _Seam >= _CrabsDictionary
#endif  //< _CrabsWallHPP
