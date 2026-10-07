// Copyright AStarship <https://astarship.net>.
#include "Random.h"

namespace _ {

#if SEAM < CRABS_RANDOM
#include <random>

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
  enum {
    FPExponentBitCount = sizeof(FP) == 8 ? 11 : 8,
    FPMantissaBitCount = sizeof(FP) * 8 - 1 - FPExponentBitCount,
  };
  if (min >= max)
    return 0;
  I range = max - min + 1;
  if (range < I(1) << FPMantissaBitCount) {
    FP range_fp = FP(range);
    FP signed_prob = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
    if (signed_prob < 0.0) signed_prob = -signed_prob;
    return I(signed_prob * range_fp) + min;
  }
  return 0;
}

template<typename I, typename FP>
void TRandomBits(I& result, I bit_count) {
  if (bit_count <= 0) {
    result = 0;
    return;
  }
  const I BitCountMax = sizeof(I) * 8;
  if (bit_count >= BitCountMax) bit_count = BitCountMax;
  FP rnd = TRandomSignedProbility<FP>(FP(0.0), FP(1.0));
  result = I(result & ((I(1) << bit_count) - 1));
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
  if (bit_count < 0 || bit_count > sizeof(ISA) * 8) {
    result = 0;
    return;
  }
  FPC rnd = TRandomSignedProbility<FPC>(0.0, 1.0);
  result = ISA(result & ((ISA(1) << bit_count) - 1));
}

void RandomBits(ISB& result, ISB bit_count) {
  if (bit_count < 0 || bit_count > sizeof(ISB) * 8) {
    result = 0;
    return;
  }
  FPC rnd = TRandomSignedProbility<FPC>(0.0, 1.0);
  result = ISA(result & ((ISA(1) << bit_count) - 1));
}

void RandomBits(ISC& result, ISC bit_count) {
  TRandomBits<ISC, FPC>(result, bit_count);
}

void RandomBits(ISD& result, ISD bit_count) {
  TRandomBits<ISD, FPD>(result, bit_count);
}

}  // namespace _
