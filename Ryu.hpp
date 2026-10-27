// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_RYU_HPP
#define CRABS_RYU_HPP
#include "Ryu.h"
#if SEAM >= CRABS_FTOS
#include "Binary.hpp"  //< ToFloat/ToUnsigned bit casts.
namespace _ {

/* Capacity-checked Ryu shortest float print.
The Ryu kernel writes a CHA buffer; this template widens the characters
into the socket and nil-terminates.
@warning The Ryu kernel itself is NOT checked, this wrapper is.
@return Nil upon socket overflow and a pointer to the nil-terminated CHS
upon success.
@param value The FPC to convert.
@param socket The beginning of the socket.
@param stop The stop address of the socket. */
template <class CHS = CHR>
CHS* TRyuF2S(FPC value, CHS* socket, CHS* stop) {
  if (stop - socket < RyuFloatStringLengthMax) return NILP;
  CHA buffer[RyuFloatStringLengthMax];
  const ISN length = RyuF2S_N(value, buffer);
  for (ISN i = 0; i < length; ++i) socket[i] = CHS(buffer[i]);
  socket[length] = 0;
  return socket + length;
}

template <class CHS = CHR>
CHS* TRyuF2S(FPC value, CHS* socket, ISW size) {
  return TRyuF2S<CHS>(value, socket, socket + size);
}

/* Capacity-checked Ryu shortest double print.
The Ryu kernel writes a CHA buffer; this template widens the characters
into the socket and nil-terminates.
@warning The Ryu kernel itself is NOT checked, this wrapper is.
@return Nil upon socket overflow and a pointer to the nil-terminated CHS
upon success.
@param value The FPD to convert.
@param socket The beginning of the socket.
@param stop The stop address of the socket. */
template <class CHS = CHR>
CHS* TRyuD2S(FPD value, CHS* socket, CHS* stop) {
  if (stop - socket < RyuDoubleStringLengthMax) return NILP;
  CHA buffer[RyuDoubleStringLengthMax];
  const ISN length = RyuD2S_N(value, buffer);
  for (ISN i = 0; i < length; ++i) socket[i] = CHS(buffer[i]);
  socket[length] = 0;
  return socket + length;
}

template <class CHS = CHR>
CHS* TRyuD2S(FPD value, CHS* socket, ISW size) {
  return TRyuD2S<CHS>(value, socket, socket + size);
}

}  //< namespace _
#endif
#endif
