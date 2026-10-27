// Copyright AStarship <https://astarship.net>.
//
// Ryu: the fastest known IEEE-754 float-to-shortest-decimal-string
// conversion. Ported from Ulf Adams' C implementation (ulfjack/ryu,
// dual-licensed Apache-2.0 / Boost-1.0, see ~/3P/ryu/LICENSE-Apache2 and
// ~/3P/ryu/LICENSE-Boost). The C algorithm is preserved verbatim; only the
// types were mapped onto the ASCII Data Types.
#pragma once
#ifndef CRABS_RYU_H
#define CRABS_RYU_H
#include <_Config.h>
#if SEAM >= CRABS_FTOS
namespace _ {

/* Maximum Ryu output length including the nil character.
Scientific notation: [sign] digits [. digits] E [sign] exponent digits. */
enum {
  RyuFloatStringLengthMax = 16,  //< FPC: '-' + 9 + '.' + 'E' + '-' + 2 + nil.
  RyuDoubleStringLengthMax = 25  //< FPD: '-' + 17 + '.' + 'E' + '-' + 3 + nil.
};

/* Converts an IEEE-754 32-bit float to its shortest round-trippable decimal
string in scientific notation.
@warning This function DOES NOT do any error checking!
@param value The FPC to convert (NaN and +/-Infinity supported).
@param result The socket to write into, at least
RyuFloatStringLengthMax characters long.
@return The character count, the nil terminator is NOT written. */
ISN RyuF2S_N(FPC value, CHA* result);

/* Converts an IEEE-754 32-bit float to its shortest round-trippable decimal
string in scientific notation, nil-terminated.
@warning This function DOES NOT do any error checking!
@param value The FPC to convert (NaN and +/-Infinity supported).
@param result The socket to write into, at least
RyuFloatStringLengthMax characters long.
@return A pointer to the nil terminator (one past the last character
written); the string begins at @p result. */
CHA* RyuF2S(FPC value, CHA* result);

/* Converts an IEEE-754 64-bit double to its shortest round-trippable
decimal string in scientific notation.
@warning This function DOES NOT do any error checking!
@param value The FPD to convert (NaN and +/-Infinity supported).
@param result The socket to write into, at least
RyuDoubleStringLengthMax characters long.
@return The character count, the nil terminator is NOT written. */
ISN RyuD2S_N(FPD value, CHA* result);

/* Converts an IEEE-754 64-bit double to its shortest round-trippable decimal
string in scientific notation, nil-terminated.
@warning This function DOES NOT do any error checking!
@param value The FPD to convert (NaN and +/-Infinity supported).
@param result The socket to write into, at least
RyuDoubleStringLengthMax characters long.
@return A pointer to the nil terminator (one past the last character
written); the string begins at @p result. */
CHA* RyuD2S(FPD value, CHA* result);

}  //< namespace _
#endif
#endif
