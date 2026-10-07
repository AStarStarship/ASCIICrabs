// Copyright AStarship <https://astarship.net>.
#include "Puff.hpp"
//
#include <cmath>
//
// Because some of y'all will try to make this a single file header, just
// keep in mind that the static data will get duplicated in each library
// you build.
namespace _ {
#if SEAM >= CRABS_ITOS

const IUD* Pow10_UI8() {
  static const IUD TenToThe[20] = {
      1,                     //< 10^0
      10,                    //< 10^1
      100,                   //< 10^2
      1000,                  //< 10^3
      10000,                 //< 10^4
      100000,                //< 10^5
      1000000,               //< 10^6
      10000000,              //< 10^7
      100000000,             //< 10^8
      1000000000,            //< 10^9
      10000000000,           //< 10^10
      100000000000,          //< 10^11
      1000000000000,         //< 10^12
      10000000000000,        //< 10^13
      100000000000000,       //< 10^14
      1000000000000000,      //< 10^15
      10000000000000000,     //< 10^16
      100000000000000000,    //< 10^17
      1000000000000000000,   //< 10^18
      10000000000000000000,  //< 10^19
  };
  return TenToThe;
}

IUD Pow10(IUD index) { return (index > 19) ? 0 : Pow10_UI8()[index]; }

const IUC* Pow10_UI4() {
  static const IUC TenToThe[20] = {
      1,           //< 10^0
      10,          //< 10^1
      100,         //< 10^2
      1000,        //< 10^3
      10000,       //< 10^4
      100000,      //< 10^5
      1000000,     //< 10^6
      10000000,    //< 10^7
      100000000,   //< 10^8
      1000000000,  //< 10^9
  };
  return TenToThe;
}

IUC Pow10(IUC index) { return (index > 9) ? 0 : Pow10_UI4()[index]; }

const IUB* Pow10_UI2() {
  static const IUB TenToThe[20] = {
      1,      //< 10^0
      10,     //< 10^1
      100,    //< 10^2
      1000,   //< 10^3
      10000,  //< 10^4
  };
  return TenToThe;
}

IUB Pow10(IUB index) { return (index > 4) ? 0 : Pow10_UI2()[index]; }

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
static const IUB Digits00To99[100] = {
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
    0x3930, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
    0x3831, 0x3931, 0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632,
    0x3732, 0x3832, 0x3932, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
    0x3633, 0x3733, 0x3833, 0x3933, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434,
    0x3534, 0x3634, 0x3734, 0x3834, 0x3934, 0x3035, 0x3135, 0x3235, 0x3335,
    0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x3036, 0x3136, 0x3236,
    0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x3037, 0x3137,
    0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x3038,
    0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
    0x3939,
};
#else
static const IUB Digits00To99[100] = {
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035, 0x3036, 0x3037, 0x3038,
    0x3039, 0x3130, 0x3131, 0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137,
    0x3138, 0x3139, 0x3230, 0x3231, 0x3232, 0x3233, 0x3234, 0x3235, 0x3236,
    0x3237, 0x3238, 0x3239, 0x3330, 0x3331, 0x3332, 0x3333, 0x3334, 0x3335,
    0x3336, 0x3337, 0x3338, 0x3339, 0x3430, 0x3431, 0x3432, 0x3433, 0x3434,
    0x3435, 0x3436, 0x3437, 0x3438, 0x3439, 0x3530, 0x3531, 0x3532, 0x3533,
    0x3534, 0x3535, 0x3536, 0x3537, 0x3538, 0x3539, 0x3630, 0x3631, 0x3632,
    0x3633, 0x3634, 0x3635, 0x3636, 0x3637, 0x3638, 0x3639, 0x3730, 0x3731,
    0x3732, 0x3733, 0x3734, 0x3735, 0x3736, 0x3737, 0x3738, 0x3739, 0x3830,
    0x3831, 0x3832, 0x3833, 0x3834, 0x3835, 0x3836, 0x3837, 0x3838, 0x3839,
    0x3930, 0x3931, 0x3932, 0x3933, 0x3934, 0x3935, 0x3936, 0x3937, 0x3938,
    0x3939 };
#endif

const IUB* BinaryLUTDecimals() { return Digits00To99; }

// @todo This is not Puff and it should be. We really only want one that works
// with one word. We also have to run on 16-bit systems so we have to provide
// a lower memory cost overhead version of puff.

ISN STRLength(IUB value) {
  if (value < 10) return 1;
  if (value < 100) return 2;
  if (value < 1000) return 3;
  if (value < 10000) return 4;
  return 5;
}
ISN STRLength(ISB value) {
  if (value < 0) return STRLength((IUB)-value) + 1;
  return STRLength((IUB)value);
}

ISN STRLength(IUC value) {
  if ((value >> 16) == 0) return STRLength((IUB)value);
  if (value < 1000000) return 6;
  if (value < 10000000) return 7;
  if (value < 100000000) return 8;
  if (value < 1000000000) return 9;
  return 10;
}
ISN STRLength(ISC value) {
  if (value < 0) return STRLength((IUC)-value) + 1;
  return STRLength((IUC)value);
}

ISN STRLength(IUD value) {
  if ((value >> 32) == 0) return STRLength((IUC)value);
  if (value < 100000000000) return 11;
  if (value < 1000000000000) return 12;
  if (value < 10000000000000) return 13;
  if (value < 100000000000000) return 14;
  if (value < 1000000000000000) return 15;
  if (value < 10000000000000000) return 16;
  if (value < 100000000000000000) return 17;
  if (value < 1000000000000000000) return 18;
  if (value < 10000000000000000000) return 19;
  return 20;
}
ISN STRLength(ISD value) {
  if (value < 0) return STRLength((IUD)-value) + 1;
  return STRLength((IUD)value);
}

#endif

#if SEAM >= CRABS_FTOS

IUD TComputePow10(ISC e, ISC alpha, ISC gamma) {
  FPD pow_10 = 0.30102999566398114,  //< 1/lg(10)
      alpha_minus_e_plus_63 = static_cast<FPD>(ISD(alpha) - e + 63),
      ceiling = Ceiling(alpha_minus_e_plus_63 * pow_10);
  return *TPtr<IUD>(&pow_10);
}

constexpr ISW CIEEE754LutElementCount() { return 87; }

/* Precomputed IEEE 754 base 2 powers of ten exponents:
10^-348, 10^-340, ..., 10^340.
Size bytes is 87 elements * 8 bytes/element = 696 bytes. */
static const ISB IEEE754Pow10E[] = {
    -1220, -1193, -1166, -1140, -1113, -1087, -1060, -1034, -1007, -980, -954,
    -927,  -901,  -874,  -847,  -821,  -794,  -768,  -741,  -715,  -688, -661,
    -635,  -608,  -582,  -555,  -529,  -502,  -475,  -449,  -422,  -396, -369,
    -343,  -316,  -289,  -263,  -236,  -210,  -183,  -157,  -130,  -103, -77,
    -50,   -24,   3,     30,    56,    83,    109,   136,   162,   189,  216,
    242,   269,   295,   322,   348,   375,   402,   428,   455,   481,  508,
    534,   561,   588,   614,   641,   667,   694,   720,   747,   774,  800,
    827,   853,   880,   907,   933,   960,   986,   1013,  1039,  1066};

/* Precomputed IEEE 754 powers of ten integral portions:
10^-348, 10^-340, ..., 10^340.
Size bytes is 87 elements * 8 bytes/element = 696 bytes. */
static const IUD IEEE754Pow10F8[] = {
    0xfa8fd5a0081c0288, 0xbaaee17fa23ebf76, 0x8b16fb203055ac76,
    0xcf42894a5dce35ea, 0x9a6bb0aa55653b2d, 0xe61acf033d1a45df,
    0xab70fe17c79ac6ca, 0xff77b1fcbebcdc4f, 0xbe5691ef416bd60c,
    0x8dd01fad907ffc3c, 0xd3515c2831559a83, 0x9d71ac8fada6c9b5,
    0xea9c227723ee8bcb, 0xaecc49914078536d, 0x823c12795db6ce57,
    0xc21094364dfb5637, 0x9096ea6f3848984f, 0xd77485cb25823ac7,
    0xa086cfcd97bf97f4, 0xef340a98172aace5, 0xb23867fb2a35b28e,
    0x84c8d4dfd2c63f3b, 0xc5dd44271ad3cdba, 0x936b9fcebb25c996,
    0xdbac6c247d62a584, 0xa3ab66580d5fdaf6, 0xf3e2f893dec3f126,
    0xb5b5ada8aaff80b8, 0x87625f056c7c4a8b, 0xc9bcff6034c13053,
    0x964e858c91ba2655, 0xdff9772470297ebd, 0xa6dfbd9fb8e5b88f,
    0xf8a95fcf88747d94, 0xb94470938fa89bcf, 0x8a08f0f8bf0f156b,
    0xcdb02555653131b6, 0x993fe2c6d07b7fac, 0xe45c10c42a2b3b06,
    0xaa242499697392d3, 0xfd87b5f28300ca0e, 0xbce5086492111aeb,
    0x8cbccc096f5088cc, 0xd1b71758e219652c, 0x9c40000000000000,
    0xe8d4a51000000000, 0xad78ebc5ac620000, 0x813f3978f8940984,
    0xc097ce7bc90715b3, 0x8f7e32ce7bea5c70, 0xd5d238a4abe98068,
    0x9f4f2726179a2245, 0xed63a231d4c4fb27, 0xb0de65388cc8ada8,
    0x83c7088e1aab65db, 0xc45d1df942711d9a, 0x924d692ca61be758,
    0xda01ee641a708dea, 0xa26da3999aef774a, 0xf209787bb47d6b85,
    0xb454e4a179dd1877, 0x865b86925b9bc5c2, 0xc83553c5c8965d3d,
    0x952ab45cfa97a0b3, 0xde469fbd99a05fe3, 0xa59bc234db398c25,
    0xf6c69a72a3989f5c, 0xb7dcbf5354e9bece, 0x88fcf317f22241e2,
    0xcc20ce9bd35c78a5, 0x98165af37b2153df, 0xe2a0b5dc971f303a,
    0xa8d9d1535ce3b396, 0xfb9b7cd9a4a7443c, 0xbb764c4ca7a44410,
    0x8bab8eefb6409c1a, 0xd01fef10a657842c, 0x9b10a4e5e9913129,
    0xe7109bfba19c0c9d, 0xac2820d9623bf429, 0x80444b5e7aa7cf85,
    0xbf21e44003acdd2d, 0x8e679c2f5e44ff8f, 0xd433179d9c8cb841,
    0x9e19db92b4e31ba9, 0xeb96bf6ebadf77d9, 0xaf87023b9bf0ee6b};

static const IUC Pow10Digits[] = {0,        1,         10,        100,
                                  1000,     10000,     100000,    1000000,
                                  10000000, 100000000, 1000000000};

/* Precomputed IEEE 754 powers of ten integral portions:
10^-348, 10^-340, ..., 10^340.
Size bytes is 87 elements * 8 bytes/element = 696 bytes. */
static const IUC IEEE754Pow10F4[] = {0};

const void* BinaryPow10Exponents() { return IEEE754Pow10E; }

const void* Binary32Pow10IntegralPortions() { return IEEE754Pow10F4; }

const void* Binary64Pow10IntegralPortions() { return IEEE754Pow10F8; }

BOL IsFinite(FPC value) {
  return static_cast<FPC>(std::isfinite(static_cast<FPD>(value)));
}

BOL IsFinite(FPD value) { return std::isfinite(value); }

BOL IsInfinite(FPC value) {
  return static_cast<FPC>(std::isinf(static_cast<FPD>(value)));
}

BOL IsInfinite(FPD value) { return std::isinf(value); }

FPD Ceiling(FPD value) { return ceil(value); }

FPC Ceiling(FPC value) {
  return static_cast<FPC>(ceil(static_cast<FPD>(value)));
}

#endif
}  //< namespace _
