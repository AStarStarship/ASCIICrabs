// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsErrorH
#define _CrabsErrorH
#include <_Config.h>
#if _Seam >= _CrabsCore
namespace _ {

// @see _ConfigHeader.hxx for enum of error codes.

/* A CHA[32][32] containing the ASCIIError error strings.
Example:
const CHA* error_string = STRErrors()[error_code * 32]; */
const CHA* ASCIIErrorSTA();

/* Returns one of the STRErrors() ASCIIError Strings. */
const CHA* ASCIIErrorSTA(ISW error);

BOL AIsError(const void* result);

}  //< namespace _
#endif
#endif  //< _CrabsErrorH
