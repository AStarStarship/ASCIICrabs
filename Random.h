// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsRandomH
#define _CrabsRandomH
#include <_Config.h>
// std headers go in the global namespace, never inside `namespace _` —
// the std headers reference ::std / ::__builtin symbols unqualified, and
// nesting them in `_` makes the compiler resolve `std` to `_::std`.
#include <random>
#include <cstdlib>
#include <cmath>
namespace _ {

/* Gets a random generator device seed. */
IUC RandomSeed();

/* Re-seeds the static mersenne twister engine. */
void RandomizeSeed();

/* Gets a random 8-bit/1-byte unsigned integer. */
IUA Random(IUA min, IUA max);

/* Gets a random 8-bit/1-byte signed integer. */
ISA Random(ISA min, ISA max);

/* Gets a random 16-bit/2-byte unsigned integer. */
IUB Random(IUB min, IUB max);

/* Gets a random 16-bit/2-byte signed integer. */
ISB Random(ISB min, ISB max);

/* Gets a random 32-bit/4-byte unsigned integer. */
IUC Random(IUC min, IUC max);

/* Gets a random 32-bit/4-byte unsigned integer. */
ISC Random(ISC min, ISC max);

/* Gets a random 64-bit/8-byte unsigned integer. */
IUD Random(IUD min, IUD max);

/* Gets a random 64-bit/8-byte signed integer. */
ISD Random(ISD min, ISD max);

/* Gets a random 8-bit/1-IUA unsigned integer. */
IUA IUARandom();

/* Gets a random 8-bit/1-IUA signed integer. */
ISA ISARandom();

/* Gets a random 16-bit/2-IUA unsigned integer. */
IUB IUBRandom();

/* Gets a random 16-bit/2-IUA signed integer. */
ISB ISBRandom();

/* Gets a random 32-bit/4-IUA unsigned integer. */
IUC IUCRandom();

/* Gets a random 32-bit/4-IUA signed integer. */
ISC ISCRandom();

/* Gets a random 64-bit/8-IUA unsigned integer. */
IUD IUDRandom();

/* Gets a random 64-bit/8-IUA signed integer. */
ISD ISDRandom();

/* Gets a random 8-bit/1-IUA unsigned integer. */
void Random(IUA& result);

/* Gets a random 8-bit/1-IUA signed integer. */
void Random(ISA& result);

/* Gets a random 16-bit/2-IUA unsigned integer. */
void Random(IUB& result);

/* Gets a random 16-bit/2-IUA signed integer. */
void Random(ISB& result);

/* Gets a random 32-bit/4-IUA unsigned integer. */
void Random(IUC& result);

/* Gets a random 32-bit/4-IUA signed integer. */
void Random(ISC& result);

/* Gets a random 64-bit/8-IUA unsigned integer. */
void Random(IUD& result);

/* Gets a random 64-bit/8-IUA signed integer. */
void Random(ISD& result);

/* Generates a random 64-bit floating point number 0 <= x <= 1. */
void RandomProbability(FPC& result);

/* Generates a random 64-bit floating point number 0 <= x <= 1. */
void RandomProbability(FPD& result);

/* Generates a random 64-bit floating point number -1 <= x <= 1. */
void RandomProbabilitySigned(FPC& result);

/* Generates a random 64-bit floating point number -1 <= x <= 1. */
void RandomProbabilitySigned(FPD& result);

/* Generates a random 32-bit integer with the given number of bits. */
void RandomBits(ISA& result, ISA bit_count);

/* Generates a random 32-bit integer with the given number of bits. */
void RandomBits(ISB& result, ISB bit_count);

/* Generates a random 32-bit integer with the given number of bits. */
void RandomBits(ISC& result, ISC bit_count);

/* Generates a random 64-bit integer with the given number of bits. */
void RandomBits(ISD& result, ISD bit_count);

}  //< namespace _
#endif  //< _CrabsRandomH
