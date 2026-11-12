// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsPuffH
#define _CrabsPuffH
#include <_Config.h>
#if _Seam >= _CrabsIToS
namespace _ {

/* Lookup table of ASCII CHS pairs for 00, 01, ..., 99. */
const IUB* BinaryLUTDecimals();

enum {
  UI2DigitTotal = 5,
  UI4DigitTotal = 9,
  UI8DigitTotal = 20,
};

const IUD* Pow10_UI8();
IUD Pow10(IUD index);

const IUC* Pow10_UI4();
IUC Pow10(IUC index);

const IUB* Pow10_UI2();
IUB Pow10(IUB index);

/* Finds the string length of the given value. */
ISC STRLength(IUB value);
ISC STRLength(ISB value);
ISC STRLength(IUC value);
ISC STRLength(ISC value);
ISC STRLength(IUD value);
ISC STRLength(ISD value);

#if _Seam >= _CrabsFToS

/* IEEE754 Powers of 10 integral portions LUT. */
const void* Binary32Pow10IntegralPortions();

/* IEEE754 Powers of 10 integral portions LUT. */
const void* Binary64Pow10IntegralPortions();

/* Lookup table of the IEEE754 integral portion of pre-computed powers of 10.
 */
const void* BinaryPow10Exponents();

/* Gets the maximum number_ of digits required to represent a FPC as in
ASCII. */
ISC FPCDigitsMax();

/* Gets the maximum number_ of digits required to represent a FPD as in
ASCII. */
ISC FPDDigitsMax();

#endif
}  //< namespace _
#endif
#endif  //< _CrabsPuffH
