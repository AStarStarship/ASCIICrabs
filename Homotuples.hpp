// Copyright AStarship <https://astarship.net>.
//
// Homotuples.hpp — template implementation for Homotuples.h.
//
// The THomoT<T, N> generic storage form. Non-template named types
// (ISC2, FPD3, ISB4, ...) live in Homotuples.h (declarations only).

#pragma once
#ifndef _CrabsHomotuplesHPP
#define _CrabsHomotuplesHPP
#include "Homotuples.h"

namespace _ {

/* THomoT<T, N>: generic VHT storage. N is 1, 2, 3, or 4. */
template <typename T, ISC N>
struct THomoT {
  T v[N];
  THomoT() { for (ISC i = 0; i < N; ++i) v[i] = T(); }
  T& operator[](ISC i) { return v[i]; }
  const T& operator[](ISC i) const { return v[i]; }
  ISC Count() const { return N; }
};

}  // namespace _
#endif  //< _CrabsHomotuplesHPP
