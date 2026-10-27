// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_ADDRESS_HPP
#define CRABS_ADDRESS_HPP
#include "Address.h"
#if SEAM >= CRABS_OP
namespace _ {

/* C++11 constexpr function for creating Crabs addresses headers with less
typing as well as ensuring there is only a single copy in ROM.
Template solves the problem of not being able using array constructors to
create data structures in ROM.

@code
TAddress<'A', 'B', 'C'> ()
@endcode
*/
template<const CHA... N>
const CHA* TAddress() {
  static const CHA path[sizeof...(N)] = {N...};
  static const CHA term_char = 0;
  return path;
}

}  //< namespace _
#endif
#endif  // CRABS_ADDRESS_HPP
