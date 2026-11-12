// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsStringfHXX
#define _CrabsStringfHXX
#include "Binary.hpp"
#include "Puff.hpp"
#include "Stringf.hpp"
/* The string primitives are thin, contiguou wrappers over
   the house TSScan/SScan scanner family. Consumers see only the _:: wrappers. */
namespace _ {

inline BOL IsPrintable(CHA value) {
  return value >= ' ' && value != 127 || value == 9 /*Tab*/;
}

inline BOL IsPrintable(CHB value) {
  return value >= ' ' && value != 127 || value == 9 /*Tab*/;
}

inline BOL IsPrintable(CHC value) {
  return value >= ' ' && value != 127 || value == 9 /*Tab*/;
}

inline BOL IsntPrintable(CHA value) {
  return !IsPrintable(value);
}

inline BOL IsntPrintable(CHB value) {
  return !IsPrintable(value);
}

inline BOL IsntPrintable(CHC value) {
  return !IsPrintable(value);
}

inline BOL IsWhitespace(CHC value) {
  return value <= ' ';
}

const CHA* STAPrintCharsHeader() {
  return "\n|0       8       16      24      32      40      48      56      |"
         " Begin address:";
}

const CHA* STAPrintCharsBorder() {
  return "\n|+-------+-------+-------+-------+-------+-------+-------+-------|"
         " ";
}

const CHA* STAPrintHexHeader() {
  return "\n|0               8               16              24              "
         "|  Begin address:";
}

const CHA* STAPrintHexBorder() {
  return "\n|+---------------+---------------+---------------+---------------|"
         " ";
}

}  //< namespace _

#if _Seam >= _CrabsCore
#if _Seam == _CrabsCore
#include "_Debug.h"
#else
#include "_Release.h"
#endif

namespace _ {

inline ISN StringLength(IUA value) {
  if (value < 10) return 1;
  if (value < 100) return 2;
  return 3;
}

inline ISN StringLength(ISA value) {
  if (value < 0) return StringLength(IUA(-value)) + 1;
  return StringLength(IUA(value));
}

inline const CHA* SScan(const CHA* start, FPC& value) {
  return TSScanFloat<FPC, CHA>(start, value);
}

inline const CHA* SScan(const CHA* start, FPD& value) {
  return TSScanFloat<FPD, CHA>(start, value);
}

inline const CHC* SScan(const CHC* start, CHC& result) {
	//if (IsError(start)) return start;
  result = *start++;
  return start;
}
inline CHC* SScan(CHC* start, CHC& value) {
  //if (IsError(start)) return start;
  const CHC* element = const_cast<const CHC*>(start);
  return const_cast<CHC*>(SScan(element, value));
}

inline const CHA* SScan(const CHA* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHA>(start, item);
}
inline CHA* SScan(CHA* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, IUA& item) {
  return TSScanUnsigned<IUA, CHA>(start, item);
}
inline CHA* SScan(CHA* start, IUA& item) {
  return TSScanUnsigned<IUA, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHA>(start, item);
}
inline CHA* SScan(CHA* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, IUB& item) {
  return TSScanUnsigned<IUB, CHA>(start, item);
}
inline CHA* SScan(CHA* start, IUB& item) {
  return TSScanUnsigned<IUB, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHA>(start, item);
}
inline CHA* SScan(CHA* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, IUC& item) {
  return TSScanUnsigned<IUC, CHA>(start, item);
}
inline CHA* SScan(CHA* start, IUC& item) {
  return TSScanUnsigned<IUC, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHA>(start, item);
}
inline CHA* SScan(CHA* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHA>(start, item);
}
inline const CHA* SScan(const CHA* start, IUD& item) {
  return TSScanUnsigned<IUD, CHA>(start, item);
}
inline CHA* SScan(CHA* start, IUD& item) {
  return TSScanUnsigned<IUD, CHA>(start, item);
}

inline const CHB* SScan(const CHB* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHB>(start, item);
}
inline CHB* SScan(CHB* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, IUA& item) {
  return TSScanUnsigned<IUA, CHB>(start, item);
}
inline CHB* SScan(CHB* start, IUA& item) {
  return TSScanUnsigned<IUA, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHB>(start, item);
}
inline CHB* SScan(CHB* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, IUB& item) {
  return TSScanUnsigned<IUB, CHB>(start, item);
}
inline CHB* SScan(CHB* start, IUB& item) {
  return TSScanUnsigned<IUB, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHB>(start, item);
}
inline CHB* SScan(CHB* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, IUC& item) {
  return TSScanUnsigned<IUC, CHB>(start, item);
}
inline CHB* SScan(CHB* start, IUC& item) {
  return TSScanUnsigned<IUC, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHB>(start, item);
}
inline CHB* SScan(CHB* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHB>(start, item);
}
inline const CHB* SScan(const CHB* start, IUD& item) {
  return TSScanUnsigned<IUD, CHB>(start, item);
}
inline CHB* SScan(CHB* start, IUD& item) {
  return TSScanUnsigned<IUD, CHB>(start, item);
}

inline const CHC* SScan(const CHC* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHC>(start, item);
}
inline CHC* SScan(CHC* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, IUA& item) {
  return TSScanUnsigned<IUA, CHC>(start, item);
}
inline CHC* SScan(CHC* start, IUA& item) {
  return TSScanUnsigned<IUA, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHC>(start, item);
}
inline CHC* SScan(CHC* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, IUB& item) {
  return TSScanUnsigned<IUB, CHC>(start, item);
}
inline CHC* SScan(CHC* start, IUB& item) {
  return TSScanUnsigned<IUB, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHC>(start, item);
}
inline CHC* SScan(CHC* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, IUC& item) {
  return TSScanUnsigned<IUC, CHC>(start, item);
}
inline CHC* SScan(CHC* start, IUC& item) {
  return TSScanUnsigned<IUC, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHC>(start, item);
}
inline CHC* SScan(CHC* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHC>(start, item);
}
inline const CHC* SScan(const CHC* start, IUD& item) {
  return TSScanUnsigned<IUD, CHC>(start, item);
}
inline CHC* SScan(CHC* start, IUD& item) {
  return TSScanUnsigned<IUD, CHC>(start, item);
}

inline const CHA* SScanBack(const CHA* start, CHA& value) {
  //if (IsError(start)) 
  //  return start;
  value = *--start;
  return start;
}

inline const CHA* SScanBack(const CHA* start, CHC& value) {
  //if (IsError(start))
  //  return start;
  // | Byte 1   | Byte 2   | Byte 3   | Byte 4   | UTF-32 Result         |
  // |:--------:|:--------:|:--------:|:--------:|:---------------------:|
  // | 0aaaaaaa |          |          |          | 00000000000000aaaaaaa |
  // | 110aaaaa | 10bbbbbb |          |          | 0000000000aaaaabbbbbb |
  // | 1110aaaa | 10bbbbbb | 10cccccc |          | 00000aaaabbbbbbcccccc |
  // | 11110aaa | 10bbbbbb | 10cccccc | 10dddddd | aaabbbbbbccccccdddddd |
  CHA c = *--start;
  if (c >> 7 == 0) {  //< 1-byte char.
    value = c;
    return start;
  }

  if (c >> 6 != 2)
    return _NilP;
  value = c & 0x3f;
  CHA c_last = c;
  c = *--start;
  if (c >> 5 == 6) {  // 2-byte char.
    if (c_last >> 5 != 6)
      return _NilP;
    value |= (c & 0x1f) << 6;
    return start;
  }
  c_last = c;
  c = *--start;
  if (c >> 4 == 14) {  // 3-byte char.
    if (c_last >> 6 != 2)
      return _NilP;
    value |= (c & 0x1f) << 12;
  }
  // 4-byte character
  c_last = c;
  c = *--start;
  if (c >> 3 != 54 || c_last >> 6 != 2)
    return _NilP;
  value |= (c & 0x1f) << 18;
  return start;
}


inline const CHB* SScanBack(const CHB* start, CHC& value) {
  //if (IsError(start)) return start;
  // | Bytes {4N, 4N+ 1} | Bytes {4N + 2, 4N+ 3} |    UTF-32 Result     |
  // |:-----------------:|:---------------------:|:--------------------:|
  // | 000000aaaaaaaaaa  |                       | 0000000000aaaaaaaaaa |
  // | 110110yyyyyyyyyy  |   110111xxxxxxxxxx    | yyyyyyyyyyxxxxxxxxxx |
  CHB c = *--start;
  if (c >> 10 == 0) {
    value = c;
    return start;
  }
  if (c >> 10 != 55)
    return _NilP;
  value = c & 0x3ff;
  c = *--start;
  if (c >> 10 != 54)
    return _NilP;
  value |= (c & 0x3ff) << 10;
  return start;
}

inline const CHC* SScanBack(const CHC* start, CHC& value) {
  //if (IsError(start))
  // return start;
  value = *--start;
  return start;
}

inline CHA* SPrint(CHA* start, CHA* stop, IUA item) {
	return SPrint(start, stop, IUC(item));
}
inline CHA* SPrint(CHA* start, ISW size, IUA item) {
	return SPrint(start, start + size, item);
}
inline CHA* SPrint(CHA* start, CHA* stop, ISA item) {
  return SPrint(start, stop, ISC(item));
}
inline CHA* SPrint(CHA* start, ISW size, ISA item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, IUB item) {
  return SPrint(start, stop, IUC(item));
}
inline CHA* SPrint(CHA* start, ISW size, IUB item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, ISB item) {
  return SPrint(start, stop, ISC(item));
}
inline CHA* SPrint(CHA* start, ISW size, ISB item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, IUC item) {
  #if _Seam < _CrabsUniprinter
  return start;
  #else 
  return TSPrintUnsigned<IUC, CHA>(start, stop, item);
  #endif
}
inline CHA* SPrint(CHA* start, ISW size, IUC item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, ISC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISC, IUC, CHA>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, ISC item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, IUD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintUnsigned<IUD, CHA>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, IUD item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, ISD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISD, IUD, CHA>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, ISD item) {
  return SPrint(start, start + size, item);
}

CHA* SPrint(CHA* start, CHA* stop, CHC value) {
  // | Byte 1   | Byte 2   | Byte 3   | Byte 4   | UTF-32 Result         |
  // |:--------:|:--------:|:--------:|:--------:|:---------------------:|
  // | 0aaaaaaa |          |          |          | 00000000000000aaaaaaa |
  // | 110aaaaa | 10bbbbbb |          |          | 0000000000aaaaabbbbbb |
  // | 1110aaaa | 10bbbbbb | 10cccccc |          | 00000aaaabbbbbbcccccc |
  // | 11110aaa | 10bbbbbb | 10cccccc | 10dddddd | aaabbbbbbccccccdddddd |

  if (IsError(start) || IsError(stop)) return _NilP;

  _DROut("\n\n" << IUC(value) << ".) Printed:0x");
  if (value >> 7 == 0) { // 1 ASCII CHA.
    if (start + 1 >= stop) return _NilP;
    *start++ = CHA(value);
    _DROut(Hexf(*(start - 1)) << "   ");
  } else {
    CHB lsb_mask = 0x3f, msb_mask = 0x80;
    if ((value >> 11) == 0) {
      if (start + 2 >= stop) return _NilP;
      *start++ = CHA(0xC0 | (value >> 6));
      *start++ = CHA(msb_mask | (value & lsb_mask));
      _DROut(Hexf(*(start - 2)) << '_' << Hexf(*(start - 1)) << "   ");
    } else if ((value >> 16) == 0) {
      if (start + 3 >= stop) return _NilP;
      *start++ = CHA(0xE0 | (value >> 12));
      *start++ = CHA(msb_mask | ((value >> 6) & lsb_mask));
      *start++ = CHA(msb_mask | (value & lsb_mask));
      _DROut(Hexf(*(start - 3)) << '_' << Hexf(*(start - 2)) << '_' << 
             Hexf(*(start - 1)) << "   ");
    } else if ((value >> 21) == 0) {
      if (start + 4 >= stop) return _NilP;
      *start++ = CHA(0xF0 | (value >> 18));
      *start++ = CHA(msb_mask | ((value >> 12) & lsb_mask));
      *start++ = CHA(msb_mask | ((value >> 6) & lsb_mask));
      *start++ = CHA(msb_mask | (value & lsb_mask));
      _DROut(Hexf(*(start - 4)) << '_' << Hexf(*(start - 3)) << '_' << 
             Hexf(*(start - 2)) << '_' << Hexf(*(start - 1)) << "   ");
    } else {
      _DROut("\n\nUTF8 print Error: CHC is out of range!");
      return _NilP;
    }
  }
  *start = 0;
  return start;
}

inline CHA* SPrint(CHA* start, CHA* stop, CHA value) {
  // Needs 2 bytes: the char and the nil, both strictly before stop.
  if (IsErrorSocket(start, stop) || start + 1 >= stop) return _NilP;
  *start++ = value;
  *start = 0;
  return start;
}

inline CHA* SPrint(CHA* start, ISW size, CHA value) {
  return SPrint(start, start + size, value);
}

inline CHB* SPrint(CHB* start, CHB* stop, IUA item) {
  return SPrint(start, stop, IUC(item));
}
inline CHB* SPrint(CHB* start, ISW size, IUA item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, ISA item) {
  return SPrint(start, stop, ISC(item));
}
inline CHB* SPrint(CHB* start, ISW size, ISA item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, IUB item) {
  return SPrint(start, stop, IUC(item));
}
inline CHB* SPrint(CHB* start, ISW size, IUB item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, ISB item) {
  return SPrint(start, stop, ISC(item));
}
inline CHB* SPrint(CHB* start, ISW size, ISB item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, IUC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintUnsigned<IUC, CHB>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, IUC item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, ISC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISC, IUC, CHB>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, ISC item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, IUD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintUnsigned<IUD, CHB>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, IUD item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, ISD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISD, IUD, CHB>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, ISD item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* string, CHB* stop, FPC item) {
#if _Seam < _CrabsFToS
  return string;
#else
  return TSPrint<CHB>(string, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, FPC item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* string, CHB* stop, FPD item) {
#if _Seam < _CrabsFToS
  return string;
#else
  return TSPrint<CHB>(string, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, FPD item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, CHB value) {
  // Needs 2 units: the char and the nil, both strictly before stop.
  if (IsErrorSocket(start, stop) || start + 1 >= stop) return _NilP;
  *start++ = value;
  *start = 0;
  return start;
}
inline CHB* SPrint(CHB* start, CHB* stop, CHA value) {
  return SPrint(start, stop, CHB(value));
}
inline CHB* SPrint(CHB* start, ISW size, CHA value) {
  return SPrint(start, start + size, value);
}
inline CHB* SPrint(CHB* start, ISW size, CHB value) {
  return SPrint(start, start + size, value);
}
inline CHB* SPrint(CHB* start, ISW size, CHC value) {
  return SPrint(start, start + size, value);
}

inline CHC* SPrint(CHC* start, CHC* stop, IUA item) {
  return SPrint(start, stop, IUC(item));
}
inline CHC* SPrint(CHC* start, ISW size, IUA item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, ISA item) {
  return SPrint(start, stop, ISC(item));
}
inline CHC* SPrint(CHC* start, ISW size, ISA item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, IUB item) {
  return SPrint(start, stop, IUC(item));
}
inline CHC* SPrint(CHC* start, ISW size, IUB item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, ISB item) {
  return SPrint(start, stop, ISC(item));
}
inline CHC* SPrint(CHC* start, ISW size, ISB item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, IUC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintUnsigned<IUC, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, IUC item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, ISC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISC, IUC, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, ISC item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISC, IUC, CHC>(start, start + size, item);
#endif
}

inline CHC* SPrint(CHC* start, CHC* stop, IUD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintUnsigned<IUD, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, IUD item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, ISD item) {
#if _Seam < _CrabsUniprinter
  return start;
#else 
  return TSPrintSigned<ISD, IUD, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, ISD item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, ISW size, CHC value) {
  return SPrint(start, start + size, value);
}
inline CHC* SPrint(CHC* start, ISW size, CHC item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* string, CHC* stop, FPC value) {
#if _Seam < _CrabsFToS
  return string;
#else
  return TSPrint<CHC>(string, stop, value);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, FPC item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* string, CHC* stop, FPD value) {
#if _Seam < _CrabsFToS
  return string;
#else
  return TSPrint<CHC>(string, stop, value);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, FPD item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, CHC value) {
  // Needs 2 units: the char and the nil, both strictly before stop.
  if (IsErrorSocket(start, stop) || start + 1 >= stop) return _NilP;
  *start++ = value;
  *start = 0;
  return start;
}
inline CHC* SPrint(CHC* start, CHC* stop, CHA value) {
  return SPrint(start, stop, CHC(value));
}
inline CHC* SPrint(CHC* start, ISW size, CHA value) {
  return SPrint(start, start + size, CHC(value));
}

inline CHC* SPrint(CHC* start, ISW size, CHB value) {
  return SPrint(start, start + size, CHC(value));
}
inline CHC* SPrint(CHC* start, CHC* stop, CHB value) {
  return SPrint(start, stop, CHC(value));
}

CHB* SPrint(CHB* start, CHB* stop, CHC value) {
  // | Bytes {4N, 4N+ 1} | Bytes {4N + 2, 4N+ 3} |    UTF-32 Result     |
  // |:-----------------:|:---------------------:|:--------------------:|
  // | 000000aaaaaaaaaa  |                       | 0000000000aaaaaaaaaa |
  // | 110110yyyyyyyyyy  |   110111xxxxxxxxxx    | yyyyyyyyyyxxxxxxxxxx |
  if (IsError(start) || start + 1 >= stop)
    return _NilP;
  CHC lsb_mask = 0x3f, lsb = value & lsb_mask, msb = value >> 10;
  if (!msb) {
    if (start + 1 >= stop)
      return _NilP;
    *start++ = CHB(value);
    _DROut("\nPrinting 1:" << Hexf(CHB(value)));
    *start = 0;
    return start;
  } else {
    CHC msb_mask = 0xDC00;
    if (msb >> 10)  // Non-Unicode value.
      return _NilP;
    if (start + 2 >= stop)
      return _NilP;
    CHB nibble = CHB(lsb & msb_mask);
    _DROut("\nPrinting 2:" << Hexf(CHB(nibble)));
    *start++ = nibble;
    nibble = CHB(msb & msb_mask);
    _DROut(Hexf(CHB(nibble)));
    *start++ = nibble;
    *start = 0;
    return start;
  }
}

inline CHA* SPrint(CHA* start, CHA* stop, const CHA* item) {
#if _Seam < _CrabsUniprinter
	return start;
#else
	return TSPrint<CHA, CHC>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, const CHA* item) {
	return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, const CHB* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHA, CHC>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, const CHB* item) {
  return SPrint(start, start + size, item);
}

inline CHA* SPrint(CHA* start, CHA* stop, const CHC* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHA, CHC>(start, stop, item);
#endif
}
inline CHA* SPrint(CHA* start, ISW size, const CHC* item) {
  return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, const CHA* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHB, CHC>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, const CHA* item) {
	return SPrint(start, start + size, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, const CHB* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHB, CHC>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW bytes, const CHB* item) {
	return SPrint(start, start + bytes - 1, item);
}

inline CHB* SPrint(CHB* start, CHB* stop, const CHC* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHB, CHC>(start, stop, item);
#endif
}
inline CHB* SPrint(CHB* start, ISW size, const CHC* item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, const CHA* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHC, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, const CHA* item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, const CHB* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHC, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, const CHB* item) {
  return SPrint(start, start + size, item);
}

inline CHC* SPrint(CHC* start, CHC* stop, const CHC* item) {
#if _Seam < _CrabsUniprinter
  return start;
#else
  return TSPrint<CHC, CHC>(start, stop, item);
#endif
}
inline CHC* SPrint(CHC* start, ISW size, const CHC* item) {
  return SPrint(start, start + size, item);
}


ATypef::ATypef(DTB type, Sizef count) : count(count.size), type(type) {}
ATypef::ATypef(DTC type, Sizef count) : count(count.size), type(type) {}
ATypef::ATypef(DTD type, Sizef count) : count(count.size), type(type) {}

ATypef::ATypef(DTB pod, DTB vt, Sizef count) :
  count(count.size),
  type(ATypePack(pod, vt))
{}

ATypef::ATypef(DTB pod, DTB vt, DTB sw, Sizef count) :
  count(count.size),
  type(ATypePack(pod, vt, sw))
{}

ATypef::ATypef(DTB pod, DTB vt, DTB sw, DTB map, Sizef count) :
  count(count.size),
  type(ATypePack(pod, vt, sw, map))
{}

ATypef::ATypef(DTB pod, DTB vt, DTB sw, DTB map, DTB mod, Sizef count) :
  count(count.size),
  type(ATypePack(pod, vt, sw, map, mod))
{}


Centerf ATypef::Center(ISW count) {
  Centerf result;

  return result;
}

Rightf ATypef::Right(ISW count) {
  Rightf result;

  return result;
}

AErrorf::AErrorf(ISW error, const CHA* message) :
  error(error),
  message(message) {}

Valuef::Valuef() : count(0), value() {}

Valuef::Valuef(void* value, ISW count) : count(count), value(value) {}
Valuef::Valuef(const void* value, ISW count) : count(count), value(value) {}

Valuef::Valuef(CHA value, ISW count) : count(count), value(value) {}
Valuef::Valuef(const CHA* value, ISW count) : count(count), value(value) {}

Valuef::Valuef(CHB value, ISW count) : count(count), value(value) {}
Valuef::Valuef(const CHB* value, ISW count) : count(count), value(value) {}

Valuef::Valuef(CHC value, ISW count) : count(count), value(value) {}
Valuef::Valuef(const CHC* value, ISW count) : count(count), value(value) {}

Valuef::Valuef(ISA value, ISW count) : count(count), value(value) {}
Valuef::Valuef(IUA value, ISW count) : count(count), value(value) {}
Valuef::Valuef(ISB value, ISW count) : count(count), value(value) {}
Valuef::Valuef(IUB value, ISW count) : count(count), value(value) {}
Valuef::Valuef(ISC value, ISW count) : count(count), value(value) {}
Valuef::Valuef(IUC value, ISW count) : count(count), value(value) {}
Valuef::Valuef(ISD value, ISW count) : count(count), value(value) {}
Valuef::Valuef(IUD value, ISW count) : count(count), value(value) {}

Valuef::Valuef(FPC value, ISW count) : count(count), value(value) {}

Valuef::Valuef(FPD value, ISW count) : count(count), value(value) {}

ISW Valuef::Type() { return value.Type(); }

ISW Valuef::Count() { return count; }

void* Valuef::Value() { return value.WordPTR(); }

void* Valuef::ToPtr() { return value.ToPTR(); }

CHA* Valuef::ToSTA() { return value.ToSTA(); }
CHB* Valuef::ToSTB() { return value.ToSTB(); }
CHC* Valuef::ToSTC() { return value.ToSTC(); }

IUW Valuef::ToWord() { return value.Value(); }

Hexf::Hexf(const void* origin, ISW size) : element(origin, size) {}
#if _CPUEndian == _CPUEndianLittle
Hexf::Hexf(const void* value) : element(IUD(value), -ISW(sizeof(const void*))) {}
Hexf::Hexf(CHA value) : element(value, -ISW(sizeof(ISA))) {}
Hexf::Hexf(ISA value) : element(value, -ISW(sizeof(ISA))) {}
Hexf::Hexf(IUA value) : element(value, -ISW(sizeof(IUA))) {}
Hexf::Hexf(CHB value) : element(value, -ISW(sizeof(CHB))) {}
Hexf::Hexf(ISB value) : element(value, -ISW(sizeof(ISB))) {}
Hexf::Hexf(IUB value) : element(value, -ISW(sizeof(IUB))) {}
Hexf::Hexf(CHC value) : element(value, -ISW(sizeof(CHC))) {}
Hexf::Hexf(ISC value) : element(value, -ISW(sizeof(ISC))) {}
Hexf::Hexf(IUC value) : element(value, -ISW(sizeof(IUC))) {}
Hexf::Hexf(ISD value) : element(value, -ISW(sizeof(ISD))) {}
Hexf::Hexf(IUD value) : element(value, -ISW(sizeof(IUD))) {}

Hexf::Hexf(FPC value) : element(value, -ISW(sizeof(FPC))) {}

Hexf::Hexf(FPD value) : element(value, -ISW(sizeof(FPD))) {}

Binaryf::Binaryf(const void* origin, ISW size) : element(origin, size) {}
Binaryf::Binaryf(const void* value)
    : element(IUD(value), -ISW(sizeof(const void*))) {}
Binaryf::Binaryf(ISA value) : element(value, -ISW(sizeof(ISA))) {}
Binaryf::Binaryf(IUA value) : element(value, -ISW(sizeof(IUA))) {}
Binaryf::Binaryf(ISB value) : element(value, -ISW(sizeof(ISB))) {}
Binaryf::Binaryf(IUB value) : element(value, -ISW(sizeof(IUB))) {}
Binaryf::Binaryf(ISC value) : element(value, -ISW(sizeof(ISC))) {}
Binaryf::Binaryf(IUC value) : element(value, -ISW(sizeof(IUC))) {}
Binaryf::Binaryf(ISD value) : element(value, -ISW(sizeof(ISD))) {}
Binaryf::Binaryf(IUD value) : element(value, -ISW(sizeof(IUD))) {}

Binaryf::Binaryf(FPC value) : element(value, -ISW(sizeof(FPC))) {}

Binaryf::Binaryf(FPD value) : element(value, -ISW(sizeof(FPD))) {}

#else
Hexf::Hexf(const void* value) : valuef(value, sizeof(const void*)) {}
Hexf::Hexf(ISA value) : valuef(value, sizeof(ISA)) {}
Hexf::Hexf(IUA value) : valuef(value, sizeof(IUA)) {}
Hexf::Hexf(ISB value) : valuef(value, sizeof(ISB)) {}
Hexf::Hexf(IUB value) : valuef(value, sizeof(IUB)) {}
Hexf::Hexf(ISC value) : valuef(value, sizeof(ISC)) {}
Hexf::Hexf(IUC value) : valuef(value, sizeof(IUC)) {}
Hexf::Hexf(ISD value) : valuef(value, sizeof(ISD)) {}
Hexf::Hexf(IUD value) : valuef(value, sizeof(IUD)) {}
Hexf::Hexf(FPC value) : valuef(value, sizeof(FPC)) {}

Hexf::Hexf(FPD value) : valuef(value, sizeof(FPD)) {}

Binaryf::Binaryf(const void* origin, ISW size) : valuef(origin, size) {}
Binaryf::Binaryf(const void* origin) : valuef(origin, sizeof(const void*)) {}
Binaryf::Binaryf(ISA value) : valuef(value, sizeof(ISA)) {}
Binaryf::Binaryf(IUA value) : valuef(value, sizeof(IUA)) {}
Binaryf::Binaryf(ISB value) : valuef(value, sizeof(ISB)) {}
Binaryf::Binaryf(IUB value) : valuef(value, sizeof(IUB)) {}
Binaryf::Binaryf(ISC value) : valuef(value, sizeof(ISC)) {}
Binaryf::Binaryf(IUC value) : valuef(value, sizeof(IUC)) {}
Binaryf::Binaryf(ISD value) : valuef(value, sizeof(ISD)) {}
Binaryf::Binaryf(IUD value) : valuef(value, sizeof(IUD)) {}
Binaryf::Binaryf(FPC value) : valuef(value, sizeof(FPC)) {}

Binaryf::Binaryf(FPD value) : valuef(value, sizeof(FPD)) {}
#endif

// Stringf::Stringf () {}
Stringf::Stringf() : count_(0), type_(_NIL), buffer_() {
  string_ = &buffer_[0];
  *buffer_ = 0;
}
//< Visual C++ is complaining about unitialized members. I think it's a bug.

Stringf::Stringf(const CHA* value) : string_(value), type_(_STA), count_(0) { Print(value); }

Stringf::Stringf(const CHB* value) : string_(value), type_(_STA), count_(0) { Print(value); }

Stringf::Stringf(const CHC* value) : string_(value), type_(_STA), count_(0) { Print(value); }

Stringf::Stringf(CHA value) : string_(buffer_), type_(_CHA), count_(0) { Print(value); }
Stringf::Stringf(CHB value) : string_(buffer_), type_(_CHB), count_(0) { Print(value); }
Stringf::Stringf(CHC value) : string_(buffer_), type_(_CHC), count_(0) { Print(value); }
Stringf::Stringf(ISC value) : string_(buffer_), type_(_ISC), count_(0) { Print(value); }
Stringf::Stringf(IUC value) : string_(buffer_), type_(_IUC), count_(0) { Print(value); }
Stringf::Stringf(ISD value) : string_(buffer_), type_(_ISD), count_(0) { Print(value); }
Stringf::Stringf(IUD value) : string_(buffer_), type_(_IUD), count_(0) { Print(value); }
Stringf::Stringf(FPC value) : string_(buffer_), type_(_FPC), count_(0) { Print(value); }
Stringf::Stringf(FPD value) : string_(buffer_), type_(_FPD), count_(0) { Print(value); }
Stringf::Stringf(const CHA* value, ISW count) : string_(value), type_(_STA), count_(count) {
  Print(value);
}
Stringf::Stringf(const CHB* value, ISW count) : string_(value), type_(_STB), count_(count) {
  Print(value);
}
Stringf::Stringf(const CHC* value, ISW count) : string_(value), type_(_STC), count_(count) {
  Print(value);
}
Stringf::Stringf(CHA value, ISW count) : string_(buffer_), type_(_CHA), count_(count) {
  Print(value);
}
Stringf::Stringf(CHB value, ISW count) : string_(buffer_), type_(_CHB), count_(count) {
  Print(value);
}
Stringf::Stringf(CHC value, ISW count) : string_(buffer_), type_(_CHC), count_(count) {
  Print(value);
}
Stringf::Stringf(ISC value, ISW count) : string_(buffer_), type_(_ISC), count_(count) {
  Print(value);
}
Stringf::Stringf(IUC value, ISW count) : string_(buffer_), type_(_IUC), count_(count) {
  Print(value);
}
Stringf::Stringf(ISD value, ISW count) : string_(buffer_), type_(_ISD), count_(count) {
  Print(value);
}
Stringf::Stringf(IUD value, ISW count) : string_(buffer_), type_(_IUD), count_(count) {
  Print(value);
}
Stringf::Stringf(FPC value, ISW count) : string_(buffer_), type_(_FPC), count_(count) {
  Print(value);
}
Stringf::Stringf(FPD value, ISW count) : string_(buffer_), type_(_FPD), count_(count) {
  Print(value);
}

IUW Stringf::Word() { return buffer_[0]; }

void* Stringf::Value() { return buffer_; }

void* Stringf::Ref() { return TRef<void>(buffer_[0]); }

const CHA* Stringf::STA() { return TRef<const CHA>(string_); }
const CHB* Stringf::STB() { return TRef<const CHB>(string_); }
const CHC* Stringf::STC() { return TRef<const CHC>(string_); }

ISW Stringf::Type() { return type_; }

ISW Stringf::Count() { return count_; }

void Stringf::Print(const CHA* value) {
  type_ = _STA;
  string_ = value;
}

void Stringf::Print(const CHB* value) {
  type_ = _STB;
  string_ = value;
}

void Stringf::Print(const CHC* value) {
  type_ = _STC;
  string_ = value;
}

void Stringf::Print(CHA value) {
  CHA* buffer = TRef<CHA>(buffer_);
  ::_::SPrint(buffer, buffer + LengthMax, value);
  type_ = _CHA;
  string_ = buffer_;
}

void Stringf::Print(CHC value) {
  CHA* buffer = TRef<CHA>(buffer_);
  ::_::SPrint(buffer, buffer + LengthMax, value);
  type_ = _CHC;
  string_ = buffer;
}
void Stringf::Print(ISC value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsIToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
#endif
  type_ = _ISC;
  string_ = buffer;
}

void Stringf::Print(IUC value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsIToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
#endif
  type_ = _IUC;
  string_ = buffer;
}

void Stringf::Print(ISD value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsIToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
#endif
  type_ = _ISD;
  string_ = buffer;
}

void Stringf::Print(IUD value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsIToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
#endif
  type_ = _SWA;
  string_ = buffer;
}

void Stringf::Print(FPC value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsFToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
  type_ = _FPC;
  string_ = buffer;
#endif
}

void Stringf::Print(FPD value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsFToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
  type_ = _FPD;
  string_ = buffer;
#endif
}

/*
void Stringf::PrintTMC(TMC value) {}

void Stringf::PrintTME(TMC value, IUC subsecond_tick) {}

void Stringf::PrintTMD(TMD value) {}

void Stringf::Print(ATypef value) {
  CHA* buffer = TRef<CHA>(buffer_);
#if _Seam >= _CrabsIToS
  ::_::TSPrint<CHA>(buffer, buffer + LengthMax, value);
#endif
  type_ = value.type;
  string_ = buffer;
}

void Stringf::Print(TypeValue value) {
  DTW type = value.Type();
  type_ = type;
  static const CHA NotSupported[] = "Not supported\0";
  DTW pod_type = type & ATypePODMask;
  if (type != pod_type) {  // It's not a POD type.
  }
  switch (type) {
    case _NIL:
      // CHC c = 2205; // Empty-set Unicode character.
      return;
    case _CHA:
      Print(CHA(value.ToIUA()));
      return;
    case _ISA:
      Print(ISA(value.ToIUA()));
      return;
    case _IUA:
      Print((value.ToIUA()));
      return;
    case _CHB:
      Print(CHB(value.ToIUB()));
      return;
    case _ISB:
      Print(ISB(value.ToIUB()));
      return;
    case _IUB:
      Print(value.ToIUB());
      return;
    case _FPB:
      Print(value.ToIUB());
      return;
    case _CHC:
      Print(CHC(value.ToIUC()));
      return;
    case _ISC:
      Print(ISC(value.ToIUC()));
      return;
    case _IUC:
      Print(IUC(value.ToIUC()));
      return;
    case _FPC:
      Print(ToFloat(value.ToIUC()));
      return;
    case _TME:
      PrintTME(ISC(value.Word()), IUC(value.Word2()));
    case _ISD:
      Print(ISD(value.ToUID()));
      return;
    case _IUD:
      Print(value.ToUID());
      return;
    case _FPD:
      Print(ToFloat(value.ToUID()));
      return;
  }
  string_ = NotSupported;
}

Stringf::Stringf(ATypef value, ISW count) : string_(buffer_), count_(count) {
  Print(value);
}
Stringf::Stringf(TypeValue value, ISW count) : string_(buffer_), count_(count) {
  Print(value);
}*/

void Stringf::Hex(CHA value, ISW count) {
  *TRef<CHA>(buffer_) = value;
  type_ = _CHA;
  count_ = -count;
}

void Stringf::Hex(CHB value, ISW count) {
  *TRef<CHB>(buffer_) = value;
  type_ = _CHB;
  count_ = -count;
}

void Stringf::Hex(CHC value, ISW count) {
  *TRef<CHC>(buffer_) = value;
  type_ = _CHC;
  count_ = -count;
}
void Stringf::Hex(ISA value, ISW count) {
  *TRef<ISA>(buffer_) = value;
  type_ = _ISA;
  count_ = -count;
}
void Stringf::Hex(IUA value, ISW count) {
  *TRef<IUA>(buffer_) = value;
  type_ = _IUA;
  count_ = -count;
}

void Stringf::Hex(ISB value, ISW count) {
  *TRef<ISB>(buffer_) = value;
  type_ = _ISB;
  count_ = -count;
}

void Stringf::Hex(IUB value, ISW count) {
  *TRef<IUB>(buffer_) = value;
  type_ = _IUB;
  count_ = -count;
}

void Stringf::Hex(ISC value, ISW count) {
  *TRef<ISC>(buffer_) = value;
  type_ = _ISC;
  count_ = -count;
}

void Stringf::Hex(IUC value, ISW count) {
  *TRef<IUC>(buffer_) = value;
  type_ = _IUC;
  count_ = -count;
}

void Stringf::Hex(ISD value, ISW count) {
  *TRef<ISD>(buffer_) = value;
  type_ = _ISD;
  count_ = -count;
}

void Stringf::Hex(IUD value, ISW count) {
  *TRef<IUD>(buffer_) = value;
  type_ = _IUD;
  count_ = -count;
}

void Stringf::Hex(FPC value, ISW count) {
  *TRef<FPC>(buffer_) = value;
  type_ = _FPC;
  count_ = -count;
}

void Stringf::Hex(FPD value, ISW count) {
  *TRef<FPD>(buffer_) = value;
  type_ = _FPD;
  count_ = -count;
}

Centerf::Centerf() {}
Centerf::Centerf(CHA value, ISW count) : element(value, count) {}
Centerf::Centerf(const CHA* value, ISW count) : element(value, count) {}
Centerf::Centerf(CHB value, ISW count) : element(value, count) {}
Centerf::Centerf(const CHB* value, ISW count) : element(value, count) {}
Centerf::Centerf(CHC value, ISW count) : element(value, count) {}
Centerf::Centerf(const CHC* value, ISW count) : element(value, count) {}
Centerf::Centerf(ISC value, ISW count) : element(value, count) {}
Centerf::Centerf(IUC value, ISW count) : element(value, count) {}
Centerf::Centerf(ISD value, ISW count) : element(value, count) {}

Centerf::Centerf(FPC value, ISW count) : element(value, count) {}

Centerf::Centerf(FPD value, ISW count) : element(value, count) {}

Centerf::Centerf(IUD value, ISW count) : element(value, count) {}
//Centerf::Centerf(ATypef value, ISW count) : element(value, count) {}

Centerf& Centerf::Hex(CHA value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Centerf& Centerf::Hex(CHB value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Centerf& Centerf::Hex(CHC value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Centerf& Centerf::Hex(ISA value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(IUA value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(ISB value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(IUB value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(ISC value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(IUC value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(ISD value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Centerf& Centerf::Hex(IUD value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Centerf& Centerf::Hex(FPC value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Centerf& Centerf::Hex(FPD value, ISW count) {
  element.Hex(value, count);
  return *this;
}


DTW Centerf::Type() { return element.Type(); }

Rightf::Rightf() {}
Rightf::Rightf(CHA value, ISW count) : element(value, count) {}
Rightf::Rightf(const CHA* value, ISW count) : element(value, count) {}

Rightf::Rightf(CHB value, ISW count) : element(value, count) {}
Rightf::Rightf(const CHB* value, ISW count) : element(value, count) {}

Rightf::Rightf(CHC value, ISW count) : element(value, count) {}
Rightf::Rightf(const CHC* value, ISW count) : element(value, count) {}

Rightf::Rightf(ISC value, ISW count) : element(value, count) {}
Rightf::Rightf(IUC value, ISW count) : element(value, count) {}
Rightf::Rightf(ISD value, ISW count) : element(value, count) {}
Rightf::Rightf(IUD value, ISW count) : element(value, count) {}

Rightf::Rightf(FPC value, ISW count) : element(value, count) {}

Rightf::Rightf(FPD value, ISW count) : element(value, count) {}


Rightf& Rightf::Hex(CHA value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Rightf& Rightf::Hex(CHB value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Rightf& Rightf::Hex(CHC value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Rightf& Rightf::Hex(ISA value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(IUA value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(ISB value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(IUB value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(ISC value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(IUC value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(ISD value, ISW count) {
  element.Hex(value, count);
  return *this;
}
Rightf& Rightf::Hex(IUD value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Rightf& Rightf::Hex(FPC value, ISW count) {
  element.Hex(value, count);
  return *this;
}

Rightf& Rightf::Hex(FPD value, ISW count) {
  element.Hex(value, count);
  return *this;
}

DTW Rightf::Type() { return element.Type(); }

Linef::Linef(CHA value, ISW count) : element(value, count), format("") {
  CHA* cursor = format;
  *cursor++ = '\n';
  *cursor++ = value;
  *cursor++ = value;
  *cursor++ = value;
  *cursor = 0;
}
Linef::Linef(const CHA* value, ISW count) : element(value, count), format("") {}

Linef::Linef(const CHB* value, ISW count) : element(value, count), format("") {}

Linef::Linef(const CHC* value, ISW count) : element(value, count), format("") {}

DTW Linef::Type() { return element.Type(); }


Repeatf::Repeatf(const CHA* element, ISW count) : element(element), count(count) {}
Repeatf::Repeatf(const CHB* element, ISW count) : element(element), count(count) {}
Repeatf::Repeatf(const CHC* element, ISW count) : element(element), count(count) {}

Headingf::Headingf(const CHA* caption1)
    : element(caption1, AConsoleWidth),
      style(_NilP),
      caption2(_NilP),
      caption3(_NilP) {}

Headingf::Headingf(const CHA* caption1, const CHA* caption2)
    : element(caption1, AConsoleWidth),
      style(_NilP),
      caption2(caption2),
      caption3(_NilP) {}

Headingf::Headingf(const CHA* caption1, const CHA* caption2,
                   const CHA* caption3, const CHA* style, ISW count)
    : element(caption1, count),
      style(style),
      caption2(caption2),
      caption3(caption3) {}

DTW Headingf::Type() { return element.Type(); }

Charsf::Charsf(const void* start, const void* stop)
    : element(TRef<const CHA>(start),
              TRef<const CHA>(stop) - TRef<const CHA>(start)) {}
Charsf::Charsf(const void* start, ISW count)
    : element(TRef<const CHA>(start), count) {}

Charsf::Charsf(const CHB* start, const CHB* stop)
    : element(start, stop - start) {}
Charsf::Charsf(const CHB* start, ISW count) : element(start, count) {}

Charsf::Charsf(const CHC* start, const CHC* stop)
    : element(start, stop - start) {}
Charsf::Charsf(const CHC* start, ISW count) : element(start, count) {}

DTW Charsf::Type() { return element.Type(); }



Indentf::Indentf(ISW indent_count) : indent_count(indent_count) {}

Sizef::Sizef(ISA size) : size(size) {}
Sizef::Sizef(ISB size) : size(size) {}
Sizef::Sizef(ISC size) : size(size) {}
Sizef::Sizef(ISD size) : size(size) {}

}  //< namespace _
#endif

#if _Seam >= _CrabsFToS
#if _Seam == _CrabsFToS
#include "_Debug.h"
#else
#include "_Release.h"
#endif


namespace _ {

CHA* SPrint(CHA* start, CHA* stop, FPC value) {
  if (IsError(start) || start >= stop) return _NilP;
  return TRyuF2S(value, start, static_cast<ISW>(stop - start));
}
inline CHA* SPrint(CHA* start, ISW size, FPC item) {
  return SPrint(start, start + size, item);
}

CHA* SPrint(CHA* start, CHA* stop, FPD value) {
  if (IsError(start) || start >= stop) return _NilP;
  return TRyuD2S(value, start, static_cast<ISW>(stop - start));
}
inline CHA* SPrint(CHA* start, ISW size, FPD item) {
  return SPrint(start, start + size, item);
}

}  //< namespace _
#endif  //< _Seam >= _CrabsFToS
#endif  //< _CrabsStringfHXX
