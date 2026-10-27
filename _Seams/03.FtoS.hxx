// Copyright AStarship <https://astarship.net>.
#if SEAM >= CRABS_FTOS
//
#include "../Ryu.hpp"
#include "../Random.h"
#include <cstdlib>
#if SEAM == CRABS_FTOS
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
#endif

using namespace ::_;
namespace CRTest {
const CHA* FtoS(const CHA* args) {
#if SEAM >= CRABS_FTOS
  A_TEST_BEGIN;
  D_COUT("\n\nTesting Ryu (IEEE-754 float to shortest decimal string)...");

  enum { Size = RyuDoubleStringLengthMax + 1 };
  CHA socket[Size];

  D_COUT("\n\nTesting RyuF2S known answers (FPC)...");
  struct TFtoSCase {
    FPC value;
    const CHA* answer;
  };
  static const TFtoSCase f_pc_cases[] = {
    { 0.0f, "0E0" },
    { 1.0f, "1E0" },
    { -1.0f, "-1E0" },
    { 1.1754944E-38f, "1.1754944E-38" },
    { 3.355445E7f, "3.355445E7" },
    { 8.999999E9f, "9E9" },
    { 3.4366717E10f, "3.436672E10" },
    { 3.0540412E5f, "3.0540412E5" },
    { 8.0990312E3f, "8.0990312E3" },
    { 2.4414062E-4f, "2.4414062E-4" },
    { 2.4414062E-3f, "2.4414062E-3" },
    { 4.3945312E-3f, "4.3945312E-3" },
    { 6.3476562E-3f, "6.3476562E-3" },
    { 4.7223665E21f, "4.7223665E21" },
    { 8388608.0f, "8.388608E6" },
    { 1.6777216E7f, "1.6777216E7" },
    { 3.3554436E7f, "3.3554436E7" },
    { 6.7131496E7f, "6.7131496E7" },
    { 1.9310392E-38f, "1.9310392E-38" },
    { -2.47E-43f, "-2.47E-43" },
    { 1.993244E-38f, "1.993244E-38" },
    { 4103.9003f, "4.1039004E3" },
    { 5.3399997E9f, "5.3399997E9" },
    { 6.0898E-39f, "6.0898E-39" },
    { 0.0010310042f, "1.0310042E-3" },
    { 2.8823261E17f, "2.882326E17" },
    { 7.038531E-26f, "7.038531E-26" },
    { 9.2234038E17f, "9.223404E17" },
    { 6.7108872E7f, "6.710887E7" },
    { 1.0E-44f, "1E-44" },
    { 2.816025E14f, "2.816025E14" },
    { 9.223372E18f, "9.223372E18" },
    { 1.5846085E29f, "1.5846086E29" },
    { 1.1811161E19f, "1.1811161E19" },
    { 5.368709E18f, "5.368709E18" },
    { 4.6143165E18f, "4.6143166E18" },
    { 0.007812537f, "7.812537E-3" },
    { 1.4E-45f, "1E-45" },
    { 1.18697724E20f, "1.18697725E20" },
    { 1.00014165E-36f, "1.00014165E-36" },
    { 200.0f, "2E2" },
    { 3.3554432E7f, "3.3554432E7" },
  };
  enum { FpcCaseCount = sizeof(f_pc_cases) / sizeof(f_pc_cases[0]) };
  for (ISC i = 0; i < FpcCaseCount; ++i) {
    RyuF2S(f_pc_cases[i].value, socket);
    A_AVOW(f_pc_cases[i].answer, socket);
  }

  D_COUT("\n\nTesting RyuF2S known answers (bit patterns)...");
  static const IUC f_pc_bits_cases[] = {
    0x00000000,  // +0.0
    0x80000000,  // -0.0
    0x7f800001,  // NaN
    0x7f800000,  // Infinity
    0xff800000,  // -Infinity
    0x7f7fffff,  // max float
    0x00000001,  // min subnormal
    0x5d1502f9,
    0x5d9502f9,
    0x5e1502f9,
  };
  static const CHA* f_pc_bits_answers[] = {
    "0E0",
    "-0E0",
    "NaN",
    "Infinity",
    "-Infinity",
    "3.4028235E38",
    "1E-45",
    "6.7108864E17",
    "1.3421773E18",
    "2.6843546E18",
  };
  enum { FpcBitsCount = sizeof(f_pc_bits_cases) /
                          sizeof(f_pc_bits_cases[0]) };
  for (ISC i = 0; i < FpcBitsCount; ++i) {
    const FPC value = ToFloat(f_pc_bits_cases[i]);
    RyuF2S(value, socket);
    A_AVOW(f_pc_bits_answers[i], socket);
  }

  D_COUT("\n\nTesting RyuD2S known answers (FPD)...");
  struct TD2SCase {
    FPD value;
    const CHA* answer;
  };
  static const TD2SCase d_pc_cases[] = {
    { 0.0, "0E0" },
    { 1.0, "1E0" },
    { -1.0, "-1E0" },
    { 2.2250738585072014E-308, "2.2250738585072014E-308" },
    { 2.98023223876953125E-8, "2.9802322387695312E-8" },
    { -2.109808898695963E16, "-2.109808898695963E16" },
    { 4.940656E-318, "4.940656E-318" },
    { 1.18575755E-316, "1.18575755E-316" },
    { 2.989102097996E-312, "2.989102097996E-312" },
    { 9.0608011534336E15, "9.0608011534336E15" },
    { 4.708356024711512E18, "4.708356024711512E18" },
    { 9.409340012568248E18, "9.409340012568248E18" },
    { 1.2345678, "1.2345678E0" },
    { 4.294967294, "4.294967294E0" },
    { 4.294967295, "4.294967295E0" },
    { 4.294967296, "4.294967296E0" },
    { 4.294967297, "4.294967297E0" },
    { 4.294967298, "4.294967298E0" },
  };
  enum { DpcCaseCount = sizeof(d_pc_cases) / sizeof(d_pc_cases[0]) };
  for (ISC i = 0; i < DpcCaseCount; ++i) {
    RyuD2S(d_pc_cases[i].value, socket);
    A_AVOW(d_pc_cases[i].answer, socket);
  }

  D_COUT("\n\nTesting RyuD2S known answers (bit patterns)...");
  static const IUD d_pc_bits_cases[] = {
    IUD(0x8000000000000000),  // -0.0
    IUD(0x7ff0000000000001),  // NaN
    IUD(0x7ff0000000000000),  // Infinity
    IUD(0xfff0000000000000),  // -Infinity
    IUD(0x7fefffffffffffff),  // max double
    IUD(1),                   // min subnormal
    IUD(0x4830f0cf064dd592),
    IUD(0x4840f0cf064dd592),
    IUD(0x4850f0cf064dd592),
  };
  static const CHA* d_pc_bits_answers[] = {
    "-0E0",
    "NaN",
    "Infinity",
    "-Infinity",
    "1.7976931348623157E308",
    "5E-324",
    "5.764607523034235E39",
    "1.152921504606847E40",
    "2.305843009213694E40",
  };
  enum { DpcBitsCount = sizeof(d_pc_bits_cases) /
                         sizeof(d_pc_bits_cases[0]) };
  for (ISC i = 0; i < DpcBitsCount; ++i) {
    const FPD value = ToFloat(d_pc_bits_cases[i]);
    RyuD2S(value, socket);
    A_AVOW(d_pc_bits_answers[i], socket);
  }

  D_COUT("\n\nTesting RyuF2S/RyuD2S end-pointer contract...");
  {
    // The end-pointer return must be exactly one past the last character,
    // i.e. socket + RyuX2S_N length.
    const ISN f_pc_length = RyuF2S_N(1.5f, socket);
    const CHA* f_pc_end = RyuF2S(1.5f, socket);
    A_ASSERT(f_pc_end == socket + f_pc_length);
    A_AVOW("1.5E0", socket);
  }
  {
    const ISN d_pc_length = RyuD2S_N(1.5, socket);
    const CHA* d_pc_end = RyuD2S(1.5, socket);
    A_ASSERT(d_pc_end == socket + d_pc_length);
    A_AVOW("1.5E0", socket);
  }

  D_COUT("\n\nTesting TRyuF2S<CHA> (capacity-checked template)...");
  {
    const CHA* end = TRyuF2S<CHA>(1.5f, socket, socket + Size);
    A_ASSERT_PTR(end);
    A_AVOW("1.5E0", socket);
  }
  D_COUT("\n\nTesting TRyuD2S<CHB> (UTF-16 output)...");
  {
    CHB utf16_socket[Size];
    const CHB* end = TRyuD2S<CHB>(1.5, utf16_socket, utf16_socket + Size);
    A_ASSERT_PTR(end);
    static const CHB expecting[] = { '1', '.', '5', 'E', '0', 0 };
    A_AVOW(expecting, utf16_socket);
  }
  D_COUT("\n\nTesting socket overflow detection...");
  A_ASSERT(IUW(TRyuF2S<CHA>(0.0f, socket, socket + 3)) == 0);
  A_ASSERT(IUW(TRyuD2S<CHA>(0.0, socket, socket + 5)) == 0);

  D_COUT("\n\nTesting RyuF2S round-trip (random FPC bit patterns)...");
#if SEAM == CRABS_FTOS
  enum { FpcTestCount = 1 << 15 };
#else
  enum { FpcTestCount = 1 << 10 };
#endif
  {
    for (ISC i = 0; i < FpcTestCount; ++i) {
      const IUC bits = IUCRandom();
      // NaNs print as "NaN" and cannot round-trip to the same bits.
      if (((bits & 0x7f800000) == 0x7f800000) && (bits & 0x007fffff)) {
        continue;
      }
      const FPC value = ToFloat(bits);
      const ISN length = RyuF2S_N(value, socket);
      A_ASSERT(length >= 1 && length <= RyuFloatStringLengthMax - 1);
      socket[length] = 0;
      const FPC parsed = strtof(socket, NILP);
      const IUC parsed_bits = ToUnsigned(parsed);
      if (parsed_bits != bits) {
        D_COUT_ERROR("RyuF2S round-trip mismatch");
        _::COut("\n  bits: 0x").Hex(bits) << "  parsed: 0x";
        _::COut().Hex(parsed_bits) << "  text: " << socket;
        A_ASSERT(false);
      }
    }
  }

  D_COUT("\n\nTesting RyuF2S round-trip (every exponent band)...");
  {
    const IUC mantissa = 0x123456;
    for (IUC exponent = 0; exponent < 255; ++exponent) {
      const IUC bits = (exponent << 23) | mantissa;
      const FPC value = ToFloat(bits);
      RyuF2S(value, socket);
      const FPC parsed = strtof(socket, NILP);
      const IUC parsed_bits = ToUnsigned(parsed);
      if (parsed_bits != bits) {
        D_COUT_ERROR("RyuF2S band mismatch");
        D_COUT("\n  exponent: " << exponent << "  text: " << socket);
        A_ASSERT(false);
      }
    }
  }

  D_COUT("\n\nTesting RyuD2S round-trip (random FPD bit patterns)...");
#if SEAM == CRABS_FTOS
  enum { DpcTestCount = 1 << 15 };
#else
  enum { DpcTestCount = 1 << 10 };
#endif
  {
    for (ISC i = 0; i < DpcTestCount; ++i) {
      const IUD bits = IUDRandom();
      // NaNs print as "NaN" and cannot round-trip to the same bits.
      if (((bits & IUD(0x7ff0000000000000)) ==
           IUD(0x7ff0000000000000)) &&
        (bits & IUD(0x000fffffffffffff))) {
        continue;
      }
      const FPD value = ToFloat(bits);
      const ISN length = RyuD2S_N(value, socket);
      A_ASSERT(length >= 1 && length <= RyuDoubleStringLengthMax - 1);
      socket[length] = 0;
      const FPD parsed = strtod(socket, NILP);
      const IUD parsed_bits = ToUnsigned(parsed);
      if (parsed_bits != bits) {
        D_COUT_ERROR("RyuD2S round-trip mismatch");
        _::COut("\n  bits: 0x").Hex(bits) << "  parsed: 0x";
        _::COut().Hex(parsed_bits) << "  text: " << socket;
        A_ASSERT(false);
      }
    }
  }

  D_COUT("\n\nTesting RyuD2S round-trip (every exponent band)...");
  {
    // The mantissa must fit in the 52-bit fraction field (bits 0-51); a
    // wider constant would spill into the exponent field and corrupt the
    // value. 0x123456789abcd is 13 hex digits = 52 bits.
    const IUD mantissa = IUD(0x123456789abcd);
    for (IUC exponent = 0; exponent < 2047; ++exponent) {
      const IUD bits = (IUD(exponent) << 52) | mantissa;
      const FPD value = ToFloat(bits);
      RyuD2S(value, socket);
      const FPD parsed = strtod(socket, NILP);
      const IUD parsed_bits = ToUnsigned(parsed);
      if (parsed_bits != bits) {
        D_COUT_ERROR("RyuD2S band mismatch");
        D_COUT("\n  exponent: " << exponent << "  text: " << socket);
        A_ASSERT(false);
      }
    }
  }

#endif
  return NILP;
}
}  //< namespace CRTest
