// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_MATH_HPP
#define CRABS_MATH_HPP
#include <_Config.h>
#if SEAM >= CRABS_MATHCORE
#include "Stack.hpp"
#if SEAM == CRABS_MATHCORE
#include "_Debug.h"
#else
#include "_Release.h"
#endif

/* @defgroup MATHCORE Scalar math
@brief In-house scalar math functions (thin libm wrappers), FPD-typed.
@details MUSEWorld and other consumers must NOT #include <math.h>/<cmath>
directly; they call these `_::` functions so the C stdlib boundary stays inside
ASCIICrabs (implemented in Math.hxx). Names are CamelCase with the FPD type
prefix (Fpd*) per the Chimera+ style guide (functions are CamelCase, often with
a type prefix). The C <math.h> is an ASCIICrabs-internal detail only.
@see ./Math.hxx
*/

namespace _ {

/* Trigonometric. */
inline FPD FpdAsin(FPD x);
inline FPD FpdAcos(FPD x);
inline FPD FpdAtan(FPD x);
inline FPD FpdAtan2(FPD y, FPD x);
inline FPD FpdSin(FPD x);
inline FPD FpdCos(FPD x);
inline FPD FpdTan(FPD x);

/* Exponential / logarithmic. */
inline FPD FpdExp(FPD x);
inline FPD FpdLog(FPD x);
inline FPD FpdLog10(FPD x);
inline FPD FpdPow(FPD base, FPD exponent);
inline FPD FpdSqrt(FPD x);
inline FPD FpdErf(FPD x);

/* Rounding / comparison / classification. */
inline FPD FpdFabs(FPD x);
inline FPD FpdFloor(FPD x);
inline FPD FpdCeil(FPD x);
inline FPD FpdMin(FPD a, FPD b);
inline FPD FpdMax(FPD a, FPD b);
inline BOL FpdIsFinite(FPD x);
inline BOL FpdIsNaN(FPD x);
inline BOL FpdIsInf(FPD x);

}
#endif
#endif
