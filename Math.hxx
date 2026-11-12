// Copyright AStarship <https://astarship.net>.
//
// Math.hxx — self-contained scalar math. No <math.h>, no libm.
//
// Every function is implemented with arithmetic: Newton-Raphson for
// roots, Taylor series for transcendental functions, bit manipulation
// for classification. This is the entire math dependency of the system.
#pragma once
#ifndef _CrabsMathHXX
#define _CrabsMathHXX
#include "Math.h"
#if _Seam >= _CrabsMathCore

namespace _ {

/* ── Classification (bit manipulation, no libm) ─────────────────── */

/* Classification: IsNaN/IsInfinite/IsFinite are inlined in Math.h. */

/* ── Basic (arithmetic) ──────────────────────────────────────────── */

inline FPD Absolute(FPD x)   { return x < 0 ? -x : x; }
inline FPD Floor(FPD x)      { FPD i = FPD(ISN(x)); return i > x ? i - 1.0 : i; }
inline FPD Ceiling(FPD x)    { FPD i = FPD(ISN(x)); return i < x ? i + 1.0 : i; }
inline FPD Min(FPD a, FPD b) { return a < b ? a : b; }

/* fmod: the part of a left over after whole multiples of b, keeping the sign
of a. Truncating a/b toward zero is exact while the quotient still fits an
integer; past that the multiples of b are lost in the mantissa anyway, so the
result is 0. Used by the TrueType hinting and the glyph rasterizer. */
inline FPD Remainder(FPD a, FPD b) {
  if (b == 0.0) return a / b;
  FPD q = a / b;
  if (q > 9.0e15 || q < -9.0e15) return 0.0;
  return a - FPD(ISN(q)) * b;
}
inline FPD Max(FPD a, FPD b) { return a > b ? a : b; }

inline BOL IsFinite(FPD x)   { return !IsNaN(x) && !IsInfinite(x); }
inline BOL IsNaN(FPD x)      { return x != x; }
inline BOL IsInfinite(FPD x) { return x == x && x * x > 1e300; }

/* ── Newton-Raphson: x^(1/n) ────────────────────────────────────── */

/* x^(1/n) via Newton: g(x) = x^n - a, g'(x) = n*x^(n-1).
   Converges quadratically. 16 iterations gives full double precision. */
template <ISN N>
inline FPD RootN(FPD a) {
  if (a < 0) return a / a;
  if (a == 0) return 0.0;
  FPD x = a > 1.0 ? a : 1.0;
  for (ISN i = 0; i < 16; ++i) {
    FPD xn = x;
    for (ISN k = 0; k < N - 1; ++k) xn *= x;
    x = x - (xn - a) / (FPD(N) * (xn / x));
  }
  return x;
}

inline FPD SquareRoot(FPD x) {
  if (x < 0) return x / x;
  if (x == 0) return 0.0;
  FPD r = x > 1.0 ? x : 1.0;
  for (ISN i = 0; i < 16; ++i) r = 0.5 * (r + x / r);
  return r;
}

inline FPD SquareRoot(ISC& x) {
  if (x < 0) return FPD(x) / FPD(x);  // NaN sentinel
  if (x == 0) return 0.0;
  FPD a = FPD(x);
  FPD r = a > 1.0 ? a : 1.0;
  for (ISN i = 0; i < 16; ++i) r = 0.5 * (r + a / r);
  return r;
}

/* ── Range reduction + Taylor series ────────────────────────────── */

/* Reduce x to [-pi/4, pi/4] for sine/cosine.
   Returns the reduced angle; *k = number of quarter-turns. */
inline FPD ReduceTrig(FPD x, ISN* k) {
  constexpr FPD PiOver4 = 0.7853981633974483;
  constexpr FPD InversePiOver4 = 1.2732395447351627;
  *k = ISN(x * InversePiOver4 + (x >= 0 ? 0.5 : -0.5));
  return x - FPD(*k) * PiOver4;
}

/* sin(x) via Taylor after range reduction.
   sin(x) = x - x^3/6 + x^5/120 - x^7/5040 + x^9/362880
   Range reduction: x = r + k*pi/4, r in [-pi/4, pi/4].
   Universal identity: sin(r + k*pi/4) = s*SineQuarter[k] + c*CosineQuarter[k]
   where SineQuarter/CosineQuarter are sin/cos of k*pi/4 for k mod 8. */
constexpr FPD SineQuarter[8] = {
  0.0, 0.7071067811865476, 1.0, 0.7071067811865476,
  0.0, -0.7071067811865476, -1.0, -0.7071067811865476
};
constexpr FPD CosineQuarter[8] = {
  1.0, 0.7071067811865476, 0.0, -0.7071067811865476,
  -1.0, -0.7071067811865476, 0.0, 0.7071067811865476
};
inline FPD Sine(FPD x) {
  if (IsNaN(x)) return x;
  if (IsInfinite(x)) return x / x;
  ISN k;
  FPD r = ReduceTrig(x, &k);
  FPD r2 = r * r;
  FPD s = r * (1.0 + r2 * (-1.0/6.0 + r2 * (1.0/120.0 + r2 * (-1.0/5040.0 + r2 / 362880.0))));
  FPD c = 1.0 + r2 * (-0.5 + r2 * (1.0/24.0 + r2 * (-1.0/720.0 + r2 / 40320.0)));
  ISN q = ((k % 8) + 8) % 8;
  return s * CosineQuarter[q] + c * SineQuarter[q];
}

/* cos(x) via Taylor after range reduction.
   cos(r + k*pi/4) = c*CosineQuarter[k] - s*SineQuarter[k] */
inline FPD Cosine(FPD x) {
  if (IsNaN(x)) return x;
  if (IsInfinite(x)) return x / x;
  ISN k;
  FPD r = ReduceTrig(x, &k);
  FPD r2 = r * r;
  FPD c = 1.0 + r2 * (-0.5 + r2 * (1.0/24.0 + r2 * (-1.0/720.0 + r2 / 40320.0)));
  FPD s = r * (1.0 + r2 * (-1.0/6.0 + r2 * (1.0/120.0 + r2 * (-1.0/5040.0 + r2 / 362880.0))));
  ISN q = ((k % 8) + 8) % 8;
  return c * CosineQuarter[q] - s * SineQuarter[q];
}

/* tan(x) = sin(x) / cos(x) */
inline FPD Tan(FPD x) {
  FPD c = Cosine(x);
  if (c == 0) return IsInfinite(Sine(x)) ? 0.0 : x / x;
  return Sine(x) / c;
}

/* atan(x) via Taylor with argument reduction.
   For |x| > 0.7: use atan(x) = 2*atan(x/(1+sqrt(1+x^2))) to keep the
   series argument small. The Taylor series then converges fast. */
inline FPD ArcTangent(FPD x) {
  if (IsNaN(x)) return x;
  if (IsInfinite(x)) return x > 0 ? 1.5707963267948966 : -1.5707963267948966;
  FPD ax = Absolute(x);
  FPD result;
  if (ax > 0.7) {
    /* atan(x) = 2 * atan(x / (1 + sqrt(1+x^2))) for x > 0 */
    FPD reduced = ax / (1.0 + SquareRoot(1.0 + ax * ax));
    FPD x2 = reduced * reduced;
    FPD series = 1.0;
    FPD term = 1.0;
    for (ISN n = 1; n < 40; ++n) {
      term *= x2;
      FPD sign = (n & 1) ? -1.0 : 1.0;
      series += sign * term / FPD(2 * n + 1);
    }
    result = 2.0 * reduced * series;
  } else {
    FPD x2 = ax * ax;
    FPD series = 1.0;
    FPD term = 1.0;
    for (ISN n = 1; n < 40; ++n) {
      term *= x2;
      FPD sign = (n & 1) ? -1.0 : 1.0;
      series += sign * term / FPD(2 * n + 1);
    }
    result = ax * series;
  }
  return x < 0 ? -result : result;
}

/* atan2(y, x) from atan. */
inline FPD ArcTangent2(FPD y, FPD x) {
  if (x == 0 && y == 0) return 0.0;
  if (x > 0) return ArcTangent(y / x);
  if (x < 0 && y >= 0) return ArcTangent(y / x) + 3.1415926535897932;
  if (x < 0 && y < 0) return ArcTangent(y / x) - 3.1415926535897932;
  if (x == 0 && y > 0) return 1.5707963267948966;
  return -1.5707963267948966;
}

/* asin(x) = atan(x / sqrt(1-x^2)) */
inline FPD Arcsine(FPD x) {
  if (x < -1.0 || x > 1.0) return x / x;
  return ArcTangent(x / SquareRoot(1.0 - x * x));
}

/* acos(x) = pi/2 - asin(x) */
inline FPD ArcCosine(FPD x) {
  if (x < -1.0 || x > 1.0) return x / x;
  return 1.5707963267948966 - Arcsine(x);
}

/* ── exp / log via Newton + Taylor ──────────────────────────────── */

/* exp(x) = e^x. Range reduce: exp(x) = exp(x - k*ln2) * 2^k.
   Taylor for the remainder: e^r = 1 + r + r^2/2! + r^3/3! + ... */
inline FPD eToThe(FPD x) {
  if (IsNaN(x)) return x;
  if (x < -745.0) return 0.0;
  if (x > 709.0) return IsInfinite(x) ? x : 1e308;
  constexpr FPD Ln2 = 0.6931471805599453;
  constexpr FPD InverseLn2 = 1.4426950408889634;
  ISN k = ISN(x * InverseLn2 + (x >= 0 ? 0.5 : -0.5));
  FPD r = x - FPD(ISN(k)) * Ln2;
  FPD e = 1.0;
  FPD term = 1.0;
  for (ISN n = 1; n < 40; ++n) {
    term *= r / FPD(n);
    e += term;
  }
  /* Multiply by 2^k via repeated squaring. */
  FPD twoK = 1.0;
  FPD base2 = 2.0;
  ISN ak = k < 0 ? -k : k;
  while (ak > 0) {
    if (ak & 1) twoK *= base2;
    base2 *= base2;
    ak >>= 1;
  }
  if (k < 0) twoK = 1.0 / twoK;
  return e * twoK;
}

/* log(x) via Newton: find y such that e^y = x, i.e. g(y) = e^y - x.
   g'(y) = e^y. Newton: y_{n+1} = y_n - (e^{y_n} - x) / e^{y_n}.
   Start with y_0 from range reduction: log(x) = k*ln2 + log(m), m in [1,2). */
inline FPD Log_e(FPD x) {
  if (x <= 0) return x == 0 ? -1e308 : x / x;
  if (IsNaN(x) || IsInfinite(x)) return x;
  constexpr FPD Ln2 = 0.6931471805599453;
  /* Range reduce: x = m * 2^k, m in [1, 2). */
  ISN k = 0;
  FPD m = x;
  while (m >= 2.0) { m *= 0.5; ++k; }
  while (m < 1.0)  { m *= 2.0; --k; }
  /* log(1+u) via series: sum_{n=1}^{20} (-1)^(n+1) * u^n / n */
  FPD u = m - 1.0;
  FPD lm = 0.0;
  FPD term = u;
  for (ISN n = 1; n < 40; ++n) {
    lm += term / FPD(n);
    term *= -u;
  }
  return FPD(ISN(k)) * Ln2 + lm;
}

/* log10(x) = log(x) / log(10) */
inline FPD LogTen(FPD x) {
  constexpr FPD Log10 = 2.3025850929940457;
  return Log_e(x) / Log10;
}

/* pow(a, b) = exp(b * log(a)) */
inline FPD Pow(FPD base, FPD exponent) {
  if (base == 0) return exponent > 0 ? 0.0 : (exponent == 0 ? 1.0 : 1e308);
  if (base < 0 && exponent != FPD(ISN(exponent))) return base / base;
  return eToThe(exponent * Log_e(Absolute(base)));
}

/* ── erf via Abramowitz & Stegun approximation (formula 7.1.26) ────
   |error| < 1.5e-7, sufficient for physics. */
inline FPD ErrorFunction(FPD x) {
  FPD ax = Absolute(x);
  FPD t = 1.0 / (1.0 + 0.3275911 * ax);
  FPD poly = t * (0.254829592 + t * (-0.284496736 + t * (1.421413741 +
    t * (-1.453152027 + t * 1.061405429))));
  FPD result = 1.0 - poly * Absolute(eToThe(-ax * ax));
  return x < 0 ? -result : result;
}

/* ── FPC overloads (float32) ──────────────────────────────────────────
   One line each: run the FPD kernel, narrow once. Keeps a single
   implementation of every function and kills the implicit promotion. */
inline FPC Absolute(FPC x)      { return FPC(Absolute(FPD(x))); }
inline FPC Floor(FPC x)         { return FPC(Floor(FPD(x))); }
inline FPC Ceiling(FPC x)       { return FPC(Ceiling(FPD(x))); }
inline FPC Min(FPC a, FPC b)    { return FPC(Min(FPD(a), FPD(b))); }
inline FPC Remainder(FPC a, FPC b) { return FPC(Remainder(FPD(a), FPD(b))); }
inline FPC Max(FPC a, FPC b)    { return FPC(Max(FPD(a), FPD(b))); }
inline BOL IsFinite(FPC x)      { return IsFinite(FPD(x)); }
inline BOL IsNaN(FPC x)         { return IsNaN(FPD(x)); }
inline BOL IsInfinite(FPC x)    { return IsInfinite(FPD(x)); }
inline FPC SquareRoot(FPC x)    { return FPC(SquareRoot(FPD(x))); }
inline FPC Sine(FPC x)          { return FPC(Sine(FPD(x))); }
inline FPC Cosine(FPC x)        { return FPC(Cosine(FPD(x))); }
inline FPC Tan(FPC x)           { return FPC(Tan(FPD(x))); }
inline FPC ArcTangent(FPC x)    { return FPC(ArcTangent(FPD(x))); }
inline FPC ArcTangent2(FPC y, FPC x) { return FPC(ArcTangent2(FPD(y), FPD(x))); }
inline FPC Arcsine(FPC x)       { return FPC(Arcsine(FPD(x))); }
inline FPC ArcCosine(FPC x)     { return FPC(ArcCosine(FPD(x))); }
inline FPC eToThe(FPC x)        { return FPC(eToThe(FPD(x))); }
inline FPC Log_e(FPC x)         { return FPC(Log_e(FPD(x))); }
inline FPC LogTen(FPC x)        { return FPC(LogTen(FPD(x))); }
inline FPC Pow(FPC base, FPC exponent) {
  return FPC(Pow(FPD(base), FPD(exponent)));
}
inline FPC ErrorFunction(FPC x) { return FPC(ErrorFunction(FPD(x))); }

}  // namespace _
#endif
#endif  //< _CrabsMathHXX
