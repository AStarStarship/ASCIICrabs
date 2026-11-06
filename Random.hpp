// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsRandomHPP
#define _CrabsRandomHPP
#include "Random.h"
#include "Math.h"

namespace _ {

/* Uniform [0,1) float via in-house IUCRandom (32-bit). FP = FPC or FPD.
   2^32 = 4294967296. Less precise than std::uniform_real_distribution
   but uses only the in-house API (no <random>). */
template<typename FP>
inline FP TUniform01() {
  return FP(IUCRandom()) / FP(IUD(4294967296));
}

/* Box-Muller Gaussian: random normal(mean, std_dev). FP = FPC or FPD.
   Uses in-house FPDSquareRoot/FPDLog (Math.h) — no <cmath>, no <random>. */
template<typename FP>
inline FP TRandomSignedProbility(FP mean, FP std_dev) {
  static FP spare;
  static bool has_spare = false;

  if (has_spare) {
    has_spare = false;
    return spare * std_dev + mean;
  } else {
    FP u, v, s;
    do {
      u = FP(2.0) * TUniform01<FP>() - FP(1.0);
      v = FP(2.0) * TUniform01<FP>() - FP(1.0);
      s = u * u + v * v;
    } while (s >= FP(1.0) || s == FP(0.0));
    s = FP(FPDSquareRoot(FPD(-FPD(2.0) * FPDLog(FPD(s)) / FPD(s))));
    spare = v * s;
    has_spare = true;
    return mean + std_dev * u * s;
  }
}

/* Random integer in [min, max). I = integer type, FP = float type.
   Range in FP: max-min+1 in I overflows for full-width types.
   Wide types lose mantissa precision on the fold — the value still
   spans the full range, just at coarser granularity. */
template<typename I, typename FP = FPC>
inline I TRandom(I min, I max) {
  if (min >= max)
    return 0;
  FP range_fp = FP(max) - FP(min) + FP(1);
  FP prob = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
  if (prob < FP(0.0)) prob = -prob;
  // |N(0,1)| exceeds 1 about 32% of the time; fold it into [0, 1) so the
  // result can never escape [min, max].
  while (prob >= FP(1.0)) prob -= FP(1.0);
  return I(min) + I(prob * range_fp);
}

/* Random integer with the given number of bits set. */
template<typename I, typename FP>
inline void TRandomBits(I& result, I bit_count) {
  if (bit_count <= 0) {
    result = 0;
    return;
  }
  const I BitCountMax = I(sizeof(I) * 8);
  if (bit_count >= BitCountMax) bit_count = BitCountMax;
  FP rnd = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
  if (rnd < FP(0.0)) rnd = -rnd;
  while (rnd >= FP(1.0)) rnd -= FP(1.0);
  result = I(rnd * (FP(IUW(I(1)) << IUW(bit_count - 1)) * FP(2))) &
           (I(-1) >> (BitCountMax - bit_count));
}

}  //< namespace _
#endif  //< _CrabsRandomHPP
