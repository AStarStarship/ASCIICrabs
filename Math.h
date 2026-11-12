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
abbreviations: LogTen, Absolute, SquareRoot. The C <math.h> is an
ASCIICrabs-internal detail only.
@see ./Math.hxx
*/

namespace _ {

/* Trigonometric. */
inline FPD Arcsine(FPD x);
inline FPD ArcCosine(FPD x);
inline FPD ArcTangent(FPD x);
inline FPD ArcTangent2(FPD y, FPD x);
inline FPD Sine(FPD x);
inline FPD Cosine(FPD x);
inline FPD Tan(FPD x);

/* Exponential / logarithmic. */
inline FPD eToThe(FPD x);
inline FPD Log_e(FPD x);
inline FPD LogTen(FPD x);
inline FPD Pow(FPD base, FPD exponent);
inline FPD SquareRoot(FPD x);
inline FPD ErrorFunction(FPD x);

/* Rounding / comparison / classification. */
inline FPD Absolute(FPD x);
inline FPD Floor(FPD x);
inline FPD Ceiling(FPD x);
inline FPD Remainder(FPD a, FPD b);
inline FPD Min(FPD a, FPD b);
inline FPD Max(FPD a, FPD b);
inline BOL IsFinite(FPD x);
inline BOL IsNaN(FPD x);
inline BOL IsInfinite(FPD x);

/* FPC overloads: the same surface at float32.
@details Vector and raster code works in FPC. Without these every call takes
the FPD kernel and narrows the result back, which promotes the argument to
double and trips -Wdouble-promotion at every call site. Each overload
delegates to its FPD kernel, so there is exactly one implementation of the
maths and the FPC result is the float nearest the FPD one.
Beware mixed arguments: Min(1.0F, 2.0) is ambiguous between the two overloads
by design. Cast the arguments to one type at the call site. */
inline FPC Arcsine(FPC x);
inline FPC ArcCosine(FPC x);
inline FPC ArcTangent(FPC x);
inline FPC ArcTangent2(FPC y, FPC x);
inline FPC Sine(FPC x);
inline FPC Cosine(FPC x);
inline FPC Tan(FPC x);
inline FPC eToThe(FPC x);
inline FPC Log_e(FPC x);
inline FPC LogTen(FPC x);
inline FPC Pow(FPC base, FPC exponent);
inline FPC SquareRoot(FPC x);
inline FPC ErrorFunction(FPC x);
inline FPC Absolute(FPC x);
inline FPC Floor(FPC x);
inline FPC Ceiling(FPC x);
inline FPC Remainder(FPC a, FPC b);
inline FPC Min(FPC a, FPC b);
inline FPC Max(FPC a, FPC b);
inline BOL IsFinite(FPC x);
inline BOL IsNaN(FPC x);
inline BOL IsInfinite(FPC x);

}
#endif
#endif  //< _CrabsMathH
