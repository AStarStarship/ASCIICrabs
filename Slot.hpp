// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsSlotHPP
#define _CrabsSlotHPP
#include "Slot.h"
#if _Seam >= _CrabsOp
namespace _ {

/* @defgroup Slot
@brief Slot utilities for ring sockets.
*/

/* Calculates the used ring socket space.
@param Start The origin of the data (read head).
@param Stop  The stop of the data (write head).
@param Size  The size of the socket. */
template<typename T>
inline T TSlotLength(IUA* origin, IUA* stop, ISW size) {
  T delta = T(stop - origin);
  if (delta < 0) delta += T(size);  //< Wrapped: write head is before read head.
  return delta;
}

/* Calculates the space left in the given ring socket.
@param Start The origin of the data.
@param Stop  The stop of the data.
@param Size  The size of the socket. */
template<typename T>
inline T TSlotSpace(IUA* start, IUA* stop, ISW size) {
  T delta = T(stop - start);
  if (start < stop)
    return T(size - delta);
  return T(start + size - stop);
}
// @} Slot

}  //< namespace _
#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsSlotHPP
