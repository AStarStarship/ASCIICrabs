// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_RANDOM_HXX
#define CRABS_RANDOM_HXX
#include "Random.h"
// rand()/RAND_MAX/sqrt/log are used by the Box-Muller path in both seam
// branches; <cstdlib>/<cmath> are C system headers (allowed under the
// no-stdlib rule, which bans C++ standard *types/containers*, not the C
// runtime).
#include <cstdlib>
#include <cmath>
#if SEAM < CRABS_RANDOM
#include <random>
#endif

namespace _ {

#if SEAM < CRABS_RANDOM

static ::std::random_device seed_gen;
static IUC seed = seed_gen();

static ::std::default_random_engine engine(seed);

inline IUC RandomSeed() { return seed; }

void RandomizeSeed() {
  seed = seed_gen();
  engine.seed(seed);
}

template<typename I, typename FP>
I TRandom(I min, I max) {
  ::std::uniform_int_distribution<I> dist(min, max);
  return dist(engine);
}

template<typename I, typename FP>
void TRandomBits(I& result, I bit_count) {
  if (bit_count < 0) {
    result = 0;
    return;
  }
  if (bit_count >= sizeof(I) * 8) bit_count = sizeof(I) * 8;
  ::std::uniform_int_distribution<I> dist(0, (I(1) << (bit_count - 1)) - 1);
  result = dist(engine);
}

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
      u = FP(2.0) * rand() / FP(RAND_MAX) - FP(1.0);
      v = FP(2.0) * rand() / FP(RAND_MAX) - FP(1.0);
      s = u * u + v * v;
    } while (s >= FP(1.0) || s == FP(0.0));
    s = sqrt(-FP(2.0) * log(s) / s);
    spare = v * s;
    has_spare = true;
    return mean + std_dev * u * s;
  }
}
#else
template<typename FP = FPC>
inline FP TRandomSignedProbility(FP mean, FP std_dev) {
  static FP spare;
  static bool has_spare = false;

  if (has_spare) {
    has_spare = false;
    return spare * std_dev + mean;
  } else {
    FP u, v, s;
    do {
      u = FP(2.0) * rand() / FP(RAND_MAX) - FP(1.0);
      v = FP(2.0) * rand() / FP(RAND_MAX) - FP(1.0);
      s = u * u + v * v;
    } while (s >= FP(1.0) || s == FP(0.0));
    s = sqrt(-FP(2.0) * log(s) / s);
    spare = v * s;
    has_spare = true;
    return mean + std_dev * u * s;
  }
}

template<typename I, typename FP = FPC>
inline I TRandom(I min, I max) {
  if (min >= max)
    return 0;
  // Range in FP: `max - min + 1` in I overflows for full-width types
  // (e.g. ~IUC(0) + 1 wraps to zero), which used to collapse every draw
  // to 0. Wide types lose mantissa precision on the fold — the value
  // still spans the full range, just at coarser granularity.
  FP range_fp = FP(max) - FP(min) + FP(1);
  FP prob = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
  if (prob < FP(0.0)) prob = -prob;
  // |N(0,1)| exceeds 1 about 32% of the time; fold it into [0, 1) so the
  // result can never escape [min, max].
  while (prob >= FP(1.0)) prob -= FP(1.0);
  return I(min) + I(prob * range_fp);
}

template<typename I, typename FP>
void TRandomBits(I& result, I bit_count) {
  if (bit_count <= 0) {
    result = 0;
    return;
  }
  const I BitCountMax = I(sizeof(I) * 8);
  if (bit_count >= BitCountMax) bit_count = BitCountMax;
  FP rnd = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
  if (rnd < FP(0.0)) rnd = -rnd;
  while (rnd >= FP(1.0)) rnd -= FP(1.0);
  // Scale by 2^bit_count via 2^(bit_count-1) in word units so the shift
  // itself never overflows; the mask keeps the result to `bit_count` bits.
  result = I(rnd * (FP(IUW(I(1)) << IUW(bit_count - 1)) * FP(2))) &
           (I(-1) >> (BitCountMax - bit_count));
}
#endif

inline IUC IUCRandom() { return TRandom<IUC, FPC>(IUC(0), ~IUC(0)); }
inline ISC ISCRandom() { return ISC(IUCRandom()); }
inline IUD IUDRandom() { return TRandom<IUD, FPD>(IUD(0), ~IUD(0)); }
inline ISD ISDRandom() { return ISD(IUDRandom()); }

inline BOL BOLRandom() { return BOL(IUCRandom()); }
inline IUA IUARandom() { return IUA(IUCRandom()); }
inline ISA ISARandom() { return ISA(IUCRandom()); }
inline IUB IUBRandom() { return IUB(IUCRandom()); }
inline ISB ISBRandom() { return ISB(IUCRandom()); }

inline void Random(BOL& result) { result = BOL(IUCRandom() & 1); }
inline void Random(IUA& result) { result = IUARandom(); }
inline void Random(ISA& result) { result = ISARandom(); }
inline void Random(IUB& result) { result = IUBRandom(); }
inline void Random(ISB& result) { result = ISBRandom(); }
inline void Random(IUC& result) { result = IUCRandom(); }
inline void Random(ISC& result) { result = ISCRandom(); }
inline void Random(IUD& result) { result = IUDRandom(); }
inline void Random(ISD& result) { result = ISDRandom(); }

inline void RandomProbability(FPC& result) {
  //result = TRandom<ISC, FPC>(-1.0f, 1.0f);
}

inline void RandomProbability(FPD& result) {
  //result = TRandom<ISD, FPD>(-1.0, 1.0);
}

inline void RandomProbabilitySigned(FPC& result) {
  //result = TRandom<ISC, FPC>(-1.0f, 1.0f);
}

inline void RandomProbabilitySigned(FPD& result) {
  //result = TRandom<ISD, FPD>(-1.0, 1.0);
}

IUA Random(IUA min, IUA max) { return IUA(TRandom<IUC, FPC>(min, max)); }

ISA Random(ISA min, ISA max) {
  return ISA(TRandom<IUC, FPC>(IUC(min), IUC(max)));
}

IUB Random(IUB min, IUB max) { return IUB(TRandom<IUC, FPC>(min, max)); }

ISB Random(ISB min, ISB max) {
  return ISB(TRandom<ISC, FPC>(ISC(min), ISC(max)));
}

IUC Random(IUC min, IUC max) { return TRandom<IUC, FPC>(min, max); }

ISC Random(ISC min, ISC max) { return TRandom<ISC, FPC>(min, max); }

IUD Random(IUD min, IUD max) { return TRandom<IUD, FPD>(min, max); }

ISD Random(ISD min, ISD max) { return TRandom<ISD, FPD>(min, max); }

void RandomBits(ISA& result, ISA bit_count) {
  TRandomBits<ISA, FPC>(result, bit_count);
}

void RandomBits(ISB& result, ISB bit_count) {
  TRandomBits<ISB, FPC>(result, bit_count);
}

void RandomBits(ISC& result, ISC bit_count) {
  TRandomBits<ISC, FPC>(result, bit_count);
}

void RandomBits(ISD& result, ISD bit_count) {
  TRandomBits<ISD, FPD>(result, bit_count);
}

}  // namespace _
#endif  // CRABS_RANDOM_HXX
