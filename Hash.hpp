// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_HASH_HEADER_IMPL
#define CRABS_HASH_HEADER_IMPL
#include <_Config.h>
#if SEAM >= CRABS_TABLE

namespace _ {
/* Returns the highest signed prime that can fit in type IS.
@return 0 if the sizeof (IS) is not 1, 2, 4, or 8.  */
template<typename IS>
inline IS PrimeMaxSigned() {
  IS prime = (sizeof(IS) == 1) ? 127
           : (sizeof(IS) == 2) ? 32767
           : (sizeof(IS) == 4) ? 2147483647
           : (sizeof(IS) == 8) ? 9223372036854775783
                               : 0;
  return prime;
}

/* Returns the highest signed prime that can fit in type IU.
@return 0 if the sizeof (IU) is not 1, 2, 4, or 8. */
template<typename HSH>
constexpr HSH CPrimeMaxUnigned() {
  HSH prime = sizeof(HSH) == 1   ? 251
            : sizeof(HSH) == 2 ? 65535
            : sizeof(HSH) == 4 ? 4294967291
            : sizeof(HSH) == 8 ? 18446744073709551557ULL
                                 : 0;
  return prime;
}

template<typename HSH, typename CHS = CHR>
inline HSH THashPrime(CHS value, HSH hash) {
  return hash + hash * (HSH)value;
}

template<typename HSH = IUN, typename CHS = CHR>
inline HSH THashPrime(const CHS* element) {
  CHS c = (CHS)*element++;
  HSH hash = CPrimeMaxUnigned<HSH>();
  while (c) {
    hash = THashPrime<HSH, CHS>(c, hash);
    c = *element++;
  }
  return hash;
}

template<typename HSH = IUN, typename ISZ = ISR, typename CHS, typename CHY>
inline CHS* THashPrimePrint_NC(CHS* start, CHS* stop, const CHY* item, HSH& hash, 
    ISZ& length) {
  CHY c = *item++;
  hash = CPrimeMaxUnigned<HSH>();
  while (c) {
    ++length;
    hash = THashPrime<HSH, CHY>(c, hash);
    start = SPrint(start, stop, c);
    if (IsError(start)) 
      return start;
    c = *item++;
  }
  *start = 0;
  return start;
}
template<typename HSH = IUN, typename ISZ = ISR, typename CHS, typename CHY>
inline CHS* THashPrimePrint(CHS* start, CHS* stop, const CHY* item, HSH& hash,
  ISZ& length) {
  D_CHECK_PTR_RETURN(start);
  if (start >= stop) 
    D_RETURN_TPTR_ERROR(CHS, AErrorInvalidBoofer);
  D_CHECK_CPTR_RETURN(CHS, item);
  THashPrimePrint<HSH, ISZ, CHS, CHY>(start, stop, item, hash, length);
}

inline IUB HashIUB(IUB value, IUB hash) {
  IUB prime = CPrimeMaxUnigned<IUB>();
  hash = ((value & 0xff) * prime) + hash;
  hash = ((value >> 8) * prime) + hash;
  return hash;
}

inline IUB HashIUBC(IUC value, IUB hash) {
  IUB prime = CPrimeMaxUnigned<IUB>();
  hash = ((value & 0xff) * prime) + hash;
  hash = (((value >> 8) & 0xff) * prime) + hash;
  hash = (((value >> 16) & 0xff) * prime) + hash;
  hash = (((value >> 24) & 0xff) * prime) + hash;
  return hash;
}

inline IUB HashIUBD(IUD value, IUB hash) {
  IUB prime = CPrimeMaxUnigned<IUB>();
  hash = ((value & 0xff) * prime) + hash;
  hash = (((value >> 8) & 0xff) * prime) + hash;
  hash = (((value >> 16) & 0xff) * prime) + hash;
  hash = (((value >> 24) & 0xff) * prime) + hash;
  hash = (((value >> 32) & 0xff) * prime) + hash;
  hash = (((value >> 40) & 0xff) * prime) + hash;
  hash = (((value >> 48) & 0xff) * prime) + hash;
  hash = (((value >> 56) & 0xff) * prime) + hash;
  return hash;
}

/* Finds the first hash between start up to stop that is not sorted assending.
@return Positive index of the first unsorted hash if one exists, else -1. */
template<typename ISY = ISR, typename HSH = IUW>
ISY THashFindFirstUnsorted(const HSH* start, const HSH* stop) {
  HSH hash = *start++;
  ISY i = 0;
  while (start < stop) {
    HSH hash_next = *start++;
    if (hash > hash_next) return i;
    i++;
  }
  return -1;
}

}  // namespace _
#endif
#endif
