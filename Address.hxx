// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsAddressHXX
#define _CrabsAddressHXX
#include "Address.hpp"
#if _Seam >= _CrabsOp
namespace _ {

/* Returns a pointer to static variable for writing the address { a, \0 } to.
 */
inline IUB PackAddress(IUA a) { return ((1 << 8) | a); }

/* Returns a pointer to static variable for writing the address 
{ a, b, \0, \0 } to. */
inline IUC PackAddress(IUC a, IUC b) { return ((1 << 16) | a | (b << 8)); }

/* Returns a pointer to static variable for writing the address { a, b, c, \0 }
to. */
inline IUC PackAddress(IUC a, IUC b, IUC c) {
  return ((1 << 24) | a | (b << 8) | (c << 16));
}

/* Returns a pointer to static variable for writing the address { a, b, c, d,
e, f, g, \0 } to. */
inline IUD PackAddress(IUD a, IUD b, IUD c, IUD d) {
  IUD one = 1;
  return (one << 32 | a | (b << 8) | (c << 16) | (d << 24));
}

/* Returns a pointer to static variable for writing the address { a, b, c, d,
e, f, g, \0 } to. */
inline IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e) {
  IUD one = 1;
  return (one << 40 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32));
}

/* Returns a pointer to static variable for writing the address
{ a, b, c, d, e, f, g, \0 } to. */
inline IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e, IUD f) {
  IUD one = 1;
  return (one << 48 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
          (f << 40));
}

/* Returns a pointer to static variable for writing the address
{ a, b, c, d, e, f, g, \0 } to. */
inline IUD PackAddress(IUD a, IUD b, IUD c, IUD d, IUD e, IUD f, IUD g) {
  IUD one = 1;
  return (one << 56 | a | (b << 8) | (c << 16) | (d << 24) | (e << 32) |
          (f << 40) | (g << 48));
}

}  //< namespace _
#endif

#endif  //< _CrabsAddressHXX
