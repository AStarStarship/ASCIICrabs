// Copyright AStarship <https://astarship.net>.
#include "BSeq.h"
#if SEAM >= CRABS_OP
namespace _ {

IUC BSeqParamNumber(const DTB* params, ISN param_number) {
  enum {
    StateNormal = 0,
    StateVarint = 1,
  };
  if (IsError(params))
    return 0;
  /* Read param_count as VUC (32-bit unsigned varint).
   * Each byte: bit 7 = continuation, bits 0-6 = data.
   * Max 5 bytes for 32-bit value. */
  IUC param_count = 0;
  IUC shift = 0;
  DTB byte;
  do {
    byte = *params++;
    param_count |= IUC(byte & 0x7F) << shift;
    shift += 7;
  } while (byte & 0x80);
  if (param_number > param_count)
    return _NIL;
  ISC state = 0,
    bits_shift = 0;
  ISC i;
  for (i = 0; i < param_number; ++i) {
    DTB value = *params++;
		if (ATypeIsVarLength(value)) {
      if (state == StateNormal) {
        state = StateVarint;
        bits_shift = 0;
      }
      value ^= DTB(1) << 15;
      value |= ISC(*params++) << 15;
    }
  }
  return *params++;
}
}  //< namespace _
#endif
