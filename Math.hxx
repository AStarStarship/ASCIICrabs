// Copyright AStarship <https://astarship.net>.
#include "Math.h"
#if SEAM >= CRABS_MATHCORE

/* The scalar math is a thin, FPD-typed boundary over the hardware libm. The
   C <math.h> is an ASCIICrabs-internal detail (same class of internal C use as
   File.hxx's <string.h>/<stdlib.h>); consumers see only the _::Fpd* wrappers.
   The bodies call the unqualified libm functions (resolved at global scope via
   the <math.h> include above the namespace block). */
#include <math.h>

namespace _ {

inline FPD FpdAsin(FPD x)    { return asin(x); }
inline FPD FpdAcos(FPD x)    { return acos(x); }
inline FPD FpdAtan(FPD x)    { return atan(x); }
inline FPD FpdAtan2(FPD y, FPD x) { return atan2(y, x); }
inline FPD FpdSin(FPD x)     { return sin(x); }
inline FPD FpdCos(FPD x)     { return cos(x); }
inline FPD FpdTan(FPD x)     { return tan(x); }

inline FPD FpdExp(FPD x)     { return exp(x); }
inline FPD FpdLog(FPD x)     { return log(x); }
inline FPD FpdLog10(FPD x)   { return log10(x); }
inline FPD FpdPow(FPD base, FPD exponent) { return pow(base, exponent); }
inline FPD FpdSqrt(FPD x)    { return sqrt(x); }
inline FPD FpdErf(FPD x)     { return erf(x); }

inline FPD FpdFabs(FPD x)    { return fabs(x); }
inline FPD FpdFloor(FPD x)   { return floor(x); }
inline FPD FpdCeil(FPD x)    { return ceil(x); }
inline FPD FpdMin(FPD a, FPD b) { return fmin(a, b); }
inline FPD FpdMax(FPD a, FPD b) { return fmax(a, b); }
inline BOL FpdIsFinite(FPD x) { return isfinite(x) != 0; }
inline BOL FpdIsNaN(FPD x)    { return isnan(x) != 0; }
inline BOL FpdIsInf(FPD x)    { return isinf(x) != 0; }

}
#endif
