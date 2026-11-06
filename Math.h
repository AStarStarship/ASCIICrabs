// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsMathH
#define _CrabsMathH
#include <_Config.h>
#if _Seam >= _CrabsMathCore
#if _Seam == _CrabsMathCore
#include "_Debug.h"
#else
#include "_Release.h"
#endif

/* @defgroup MATHCORE Scalar math
@brief In-house scalar math functions (thin libm wrappers), FPD-typed.
@details MUSEWorld and other consumers must NOT #include <math.h>/<cmath>
directly; they call these `_::` functions so the C stdlib boundary stays inside
ASCIICrabs (implemented in Math.hxx). Names are FPD-prefixed CamelCase, no
abbreviations: FPDLogTen, FPDAbsolute, FPDSquareRoot. The C <math.h> is an
ASCIICrabs-internal detail only.
@see ./Math.hxx
*/

namespace _ {

/* Trigonometric. */
inline FPD FPDArcsine(FPD x);
inline FPD FPDArcCosine(FPD x);
inline FPD FPDArcTangent(FPD x);
inline FPD FPDArcTangent2(FPD y, FPD x);
inline FPD FPDsin(FPD x);
inline FPD FPDCos(FPD x);
inline FPD FPDtan(FPD x);

/* Exponential / logarithmic. */
inline FPD FPDExponent(FPD x);
inline FPD FPDLog(FPD x);
inline FPD FPDLogTen(FPD x);
inline FPD FPDPower(FPD base, FPD exponent);
inline FPD FPDSquareRoot(FPD x);
inline FPD FPDErrorFunction(FPD x);

/* Rounding / comparison / classification. */
inline FPD FPDAbsolute(FPD x);
inline FPD FPDFloor(FPD x);
inline FPD FPDCeiling(FPD x);
inline FPD FPDMin(FPD a, FPD b);
inline FPD FPDMax(FPD a, FPD b);
inline BOL FPDIsFinite(FPD x);
inline BOL FPDIsNaN(FPD x);
inline BOL FPDIsInfinite(FPD x);

}
#endif
#endif  //< _CrabsMathH
