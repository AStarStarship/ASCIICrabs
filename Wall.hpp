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
Each Wall contains a stack of Door pointers. The Wall is
just a memory block - the CPU doesn't care if it's on the
stack or heap; it's a contiguous address range.
@{ */

namespace _ {
  
enum {
  WallBytesMin = 512,  //< Min functional Wall size.
};

template<WAL_A>
TWall<WAL_P>* TWallInit(TWall<WAL_P>* wall, ISW bytes = WallBytesMin) {
  wall->is_dynamic_ = true;
  bytes = bytes < WallBytesMin ? ISC(WallBytesMin) : bytes;
  ISW size_words = (bytes >> ACPUBytesLog2) + 3;
  IUW *socket = new IUW[size_words];
  wall->doors_ = TStackInit<TDoor<ISZ>, ISZ, ISY>(socket, bytes);
  return wall;
}

/* Constructs a wall from the given block of words. */
template<WAL_A>
TWall<WAL_P>* TWallInit(TWall<WAL_P>* wall, IUW* socket, ISW bytes) {
  wall->is_dynamic_ = false;
  TStackInit(TRef<TStack<TDoor<ISZ>, ISZ, ISY>>(socket), bytes);
  return wall;
}

/* Gets the size of the wall in bytes. */
template<WAL_A>
ISW TWallGetSizeBytes(TWall<WAL_P>* wall) {
  return wall->doors_.GetSizeBytes();
}
/* Gets a pointer to the array of pointers to Door(). */
template<WAL_A>
TStack<TDoor<ISZ>, ISZ, ISY >* TWallDoors(TWall<WAL_P>* wall) {
  return &wall->doors_;
}

/* Gets the Door from the Door at the given index. */
template<WAL_A>
TDoor<ISZ> TWallGetDoor(TWall<WAL_P>* wall, ISY index) { return 0; }

/* Adds a Door to the slot.
@return Returns nil if the Door is full and a pointer to the Door in the
        socket upon success. */
template<WAL_A>
ISC TWallOpenDoor(TWall<WAL_P>* wall, TDoor<ISZ>* door) { return 0; }

/* Deletes the Door from the Door at the given index. */
template<WAL_A>
BOL TWallCloseDoor(TWall<WAL_P>* wall, ISY index) { return false; }

}  //< namespace _

/* @} */  // End of Wall group.
#endif  //< _Seam >= _CrabsDictionary
#endif  //< _CrabsWallHPP
