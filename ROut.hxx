// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsROutHXX
#define _CrabsROutHXX
#include "ROut.h"
#if _Seam >= _CrabsCore
#include "Uniprinter.hpp"
#include "Puff.hpp"
#if _Seam == _CrabsCore
#include "_Debug.h"
#else
#include "_Release.h"
#endif

namespace _ {

// The single persistent console-out instance. ROut::Print backs onto it via
// RefOut(); std is not used (one-token RefOut replaces it).
ROut _ROut;

ROut::ROut() {}

ROut::ROut(CHA item) { Print(item); }
ROut::ROut(const CHA* item) { Print(item); }
ROut::ROut(CHB item) { Print(item); }
ROut::ROut(const CHB* item) { Print(item); }
ROut::ROut(CHC item) { Print(item); }
ROut::ROut(const CHC* item) { Print(item); }
ROut::ROut(ISA item) { Print(item); }
ROut::ROut(IUA item) { Print(item); }
ROut::ROut(ISB item) { Print(item); }
ROut::ROut(IUB item) { Print(item); }
ROut::ROut(ISC item) { Print(item); }
ROut::ROut(IUC item) { Print(item); }
ROut::ROut(ISD item) { Print(item); }
ROut::ROut(IUD item) { Print(item); }
ROut::ROut(FPC item) { Print(item); }
ROut::ROut(FPD item) { Print(item); }
ROut::ROut(Hexf item) { Print(item); }
ROut::ROut(Binaryf item) { Print(item); }
ROut::ROut(Centerf item) { Print(item); }
ROut::ROut(Rightf item) { Print(item); }
ROut::ROut(Linef item) { Print(item); }
ROut::ROut(Repeatf item) { Print(item); }
ROut::ROut(Headingf item) { Print(item); }
ROut::ROut(Indentf item) { Print(item); }
ROut::ROut(Charsf item) { Print(item); }

ROut::ROut(const CHA* item, CHA second_item) {
  Print(item);
  Print(second_item);
}

ROut::ROut(const CHA* item, const CHA* second_item) {
  Print(item);
  Print(second_item);
}
ROut::ROut(CHA item, CHA second_item) {
  Print(item);
  Print(second_item);
}
ROut::ROut(CHA item, const CHA* second_item) {
  Print(item);
  Print(second_item);
}
ROut::ROut(CHA item, CHA second_item, CHA third_item) {
  Print(item);
  Print(second_item);
  Print(third_item);
}
ROut::ROut(CHA item, CHA second_item, const CHA* third_item) {
  Print(item);
  Print(second_item);
  Print(third_item);
}
ROut::ROut(const CHA* item, const CHA* second_item, CHA third_item) {
  Print(item);
  Print(second_item);
  Print(third_item);
}
ROut::ROut(const CHA* item, const CHA* second_item, const CHA* third_item) {
  Print(item);
  Print(second_item);
  Print(third_item);
}

ROut& ROut::Star() { return *this; }

ROut& ROut::Print(CHA item) {
  // Direct write to stdout: this is the leaf of the ROut print cycle.
  // (operator<<(ROut, CHA) -> Print(CHA); writing via `_ROut << item` here
  // would recurse operator<< -> Print -> operator<< -> ... and overflow
  // the stack.)
  fputc(item, stdout);
  return *this;
}

 ROut& ROut::Print(const CHA* item) {
  return TPrint<ROut, CHA>(*this, item);
}

ROut& ROut::Print(CHB item) {
  // Leaf: emit the codepoint as a CHA byte directly (console is CHA/8-bit).
  // Going via `_ROut << CHN(item)` would route to operator<<(ROut, CHC) ->
  // Print(CHC) and recurse; write the byte instead.
  fputc(CHA(item & 0x7f), stdout);
  return *this;
}

ROut& ROut::Print(const CHB* item) {
  return TPrint<ROut, CHB>(*this, item);
}

ROut& ROut::Print(CHC item) {
  // Leaf: emit as CHA bytes directly. (The old body did `_ROut << CHN(...)`,
  // routing to operator<<(ROut, CHC) -> Print(CHC) -> ... and recursing.)
  // Non-BMP codepoints are emitted as their two UTF-16 halves via Print(CHB);
  // BMP codepoints as a single CHA byte.
  if (item >= (CHC(1) << 9)) {
    Print(CHB(CHN(item & 0x3f) + 0xD800));
    Print(CHB(CHN((item >> 10) & 0x3f) + 0xDC00));
  } else {
    Print(CHA(item));
  }
  return *this;
}
ROut& ROut::Print(const CHC* item) {
  return TPrint<ROut, CHC>(*this, item);
}

ROut& ROut::Print(ISC item) {
#if _Seam <= _CrabsIToS
  _ROut << item;
#else
#if _CPUSize == _CPU8Byte
  Print(ISD(item));
#else
  enum { Size = 24 };
  CHA socket[Size];
  TSPrintSigned<ISD, IUD, CHA>(socket, Size, ISD(item));
  Print(socket);
#endif
#endif
  return *this;
}

ROut& ROut::Print(IUC item) {
#if _Seam <= _CrabsIToS
  _ROut << item;
#else
#if _CPUSize == _CPU8Byte
  Print((IUD)item);
#else
  enum { Size = 24 };
  CHA buffer[Size];
  TSPrintUnsigned<IUC, CHA>(buffer, Size, item);
  Print(buffer);
#endif
#endif
  return *this;
}

ROut& ROut::Print(ISD item) {
#if _Seam <= _CrabsIToS
  _ROut << item;
#else
  enum { Size = 24 };
  CHA buffer[Size];
  auto result = TSPrint<CHA>(buffer, Size, item);
  Print(buffer);
#endif
  return *this;
}

ROut& ROut::Print(IUD item) {
#if _Seam <= _CrabsIToS
  _ROut << item;
#else
  enum { Size = 24 };
  CHA buffer[Size];
  TSPrint<CHA>(buffer, Size, item);
  Print(buffer);
#endif
  return *this;
}

ROut& ROut::Print(FPC item) {
#if _Seam <= _CrabsFToS
  _ROut << item;
#else
  enum { Size = 16 };
  CHA buffer[Size];
  TSPrint<CHA>(buffer, Size, item);
  Print(buffer);
#endif
  return *this;
}

ROut& ROut::Print(FPD item) {
#if _Seam <= _CrabsFToS
  _ROut << item;
#else
  enum { Size = 24 };
  CHA buffer[Size];
  TSPrint<CHA>(buffer, Size, item);
  Print(buffer);
#endif
  return *this;
}

ROut& ROut::Print(Hexf& item) {
  return TPrintHex<ROut>(*this, item.element.Value(), item.element.count);
}

ROut& ROut::Print(Binaryf& item) {
  return TPrintBinary<ROut>(*this, item.element.Value(), item.element.count);
}

ROut& ROut::Print(Centerf& item) {
  return TPrintCenter<ROut>(*this, item.element);
}

ROut& ROut::Print(Rightf& item) {
  return TPrintRight<ROut>(*this, item.element);
}

ROut& ROut::Print(Linef& item) { return TPrint<ROut>(*this, item); }

ROut& ROut::Print(Repeatf& item) { return TPrint<ROut>(*this, item); }

ROut& ROut::Print(Headingf& item) { return TPrint<ROut>(*this, item); }

ROut& ROut::Print(Indentf& item) {
  return ::_::TPrintIndent<::_::ROut>(*this, item.indent_count);
}

ROut& ROut::Print(Charsf& item) {
  return ::_::TPrint<ROut>(*this, item);
}

ROut& ROut::NL() { return Print('\n'); }
ROut& ROut::NL(CHA item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(const CHA* item) {
  NL();
  return Print(item);
}

ROut& ROut::NL(CHB item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(const CHB* item) {
  NL();
  return Print(item);
}

ROut& ROut::NL(CHC item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(const CHC* item) {
  NL();
  return Print(item);
}

ROut& ROut::NL(ISC item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(IUC item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(ISD item) {
  NL();
  return Print(item);
}
ROut& ROut::NL(IUD item) {
  NL();
  return Print(item);
}

ROut& ROut::NL(FPC item) {
  NL();
  return Print(item);
}

ROut& ROut::NL(FPD item) {
  NL();
  return Print(item);
}

ROut& ROut::Hex(const void* item) { return TPrintHex<ROut>(*this, item); }
ROut& ROut::Hex(const void* item, ISW size) {
  return TPrintHex<ROut>(*this, item, size);
}
ROut& ROut::Hex(const void* item, const void* item_stop) {
  return TPrintHex<ROut>(*this, item, item_stop);
}
ROut& ROut::Hex(ISA item) { return TPrintHex<ROut, ISA, IUA>(*this, item); }
ROut& ROut::Hex(IUA item) { return TPrintHex<ROut, IUA>(*this, item); }
ROut& ROut::Hex(ISB item) { return TPrintHex<ROut, ISB, IUB>(*this, item); }
ROut& ROut::Hex(IUB item) { return TPrintHex<ROut, IUB>(*this, item); }
ROut& ROut::Hex(ISC item) { return TPrintHex<ROut, ISC, IUC>(*this, item); }
ROut& ROut::Hex(IUC item) { return TPrintHex<ROut, IUC>(*this, item); }
ROut& ROut::Hex(ISD item) { return TPrintHex<ROut, ISD, IUD>(*this, item); }
ROut& ROut::Hex(IUD item) { return TPrintHex<ROut, IUD>(*this, item); }
ROut& ROut::Hex(FPC item) { return TPrintHex<ROut>(*this, item); }
ROut& ROut::Hex(FPD item) { return TPrintHex<ROut>(*this, item); }

/* Stores the given item to the buffer and store the size. */
ROut& ROut::Binary(const void* item) { return TPrintBinary<ROut>(*this, item); }
ROut& ROut::Binary(const void* item, ISW size) {
  return TPrintBinary<ROut>(*this, item, size);
}
ROut& ROut::Binary(const void* start, const void* stop) {
  return TPrintBinary<ROut>(
      *this, start,
      TRef<const CHA>(stop) - TRef<const CHA>(start));
}
ROut& ROut::Binary(ISA item) {
  return TPrintBinary<ROut, ISA, IUA>(*this, item);
}
ROut& ROut::Binary(IUA item) { return TPrintBinary<ROut, IUA>(*this, item); }
ROut& ROut::Binary(ISB item) {
  return TPrintBinary<ROut, ISB, IUB>(*this, item);
}
ROut& ROut::Binary(IUB item) { return TPrintBinary<ROut, IUB>(*this, item); }
ROut& ROut::Binary(ISC item) {
  return TPrintBinary<ROut, ISC, IUC>(*this, item);
}
ROut& ROut::Binary(IUC item) { return TPrintBinary<ROut, IUC>(*this, item); }
ROut& ROut::Binary(ISD item) {
  return TPrintBinary<ROut, ISD, IUD>(*this, item);
}
ROut& ROut::Binary(IUD item) { return TPrintBinary<ROut, IUD>(*this, item); }

ROut& ROut::Binary(FPC item) { return TPrintBinary<ROut>(*this, item); }

ROut& ROut::Binary(FPD item) { return TPrintBinary<ROut>(*this, item); }

ISN ROut::PrintAndCount(const CHA* string) {
  return TPrintAndCount<ROut, CHA>(*this, string);
}

ISN ROut::PrintAndCount(const CHB* string) {
  return TPrintAndCount<ROut, CHB>(*this, string);
}

ISN ROut::PrintAndCount(const CHC* string) {
  return TPrintAndCount<ROut, CHC>(*this, string);
}

inline ROut& RefOut() { return _ROut; }

ROut CPrint(CHA item) { return ROut(item); }
ROut CPrint(const CHA* item) { return ROut(item); }
ROut CPrint(CHB item) { return ROut(item); }
ROut CPrint(const CHB* item) { return ROut(item); }
ROut CPrint(CHC item) { return ROut(item); }
ROut CPrint(const CHC* item) { return ROut(item); }

ROut CPrint(ISC item) { return ROut(item); }
ROut CPrint(IUC item) { return ROut(item); }
ROut CPrint(ISD item) { return ROut(item); }
ROut CPrint(IUD item) { return ROut(item); }
ROut CPrint(FPC item) { return ROut(item); }
ROut CPrint(FPD item) { return ROut(item); }
ROut CPrint(Hexf& item) { return ROut(item); }
ROut CPrint(Binaryf& item) { return ROut(item); }
ROut CPrint(Centerf& item) { return ROut(item); }
ROut CPrint(Rightf& item) { return ROut(item); }
ROut CPrint(Linef& item) { return ROut(item); }
ROut CPrint(Headingf& item) { return ROut(item); }
ROut CPrint(Indentf& item) { return ROut(item); }

}  //< namespace _

inline ::_::ROut& operator<<(::_::ROut& o, CHA item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, const CHA* item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, CHB item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, const CHB* item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, CHC item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, const CHC* item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, ISC item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, IUC item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, ISD item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, IUD item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, FPC item) { return o.Print(item); }
inline ::_::ROut& operator<<(::_::ROut& o, FPD item) { return o.Print(item); }

inline ::_::ROut& operator<<(::_::ROut& o, ::_::Hexf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Binaryf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}

inline ::_::ROut& operator<<(::_::ROut& o, ::_::Headingf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}

#if _Seam >= _CrabsUniprinter
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Centerf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Rightf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Linef item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Repeatf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Indentf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Charsf item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Sizef item) {
  return ::_::TPrint<::_::ROut>(o, item);
}
inline ::_::ROut& operator<<(::_::ROut& printer, ::_::ATypef item) {
  return ::_::TPrint<::_::ROut>(printer, item);
}
inline ::_::ROut& operator<<(::_::ROut& printer, ::_::AErrorf item) {
  return ::_::TPrint<::_::ROut>(printer, item);
}

inline ::_::ROut& operator<<(::_::ROut& printer, ::_::ATypeValue item) {
  return ::_::TPrint<::_::ROut>(printer, item);
}

inline ::_::ROut& operator<<(::_::ROut& printer, ::_::ATypePtr item) {
  return ::_::TPrint<::_::ROut>(printer, item);
}

template<typename T, typename CH>
inline ::_::ROut& operator<<(::_::ROut& printer, ::_::TSizeCodef<T, CHA> item) {
  return ::_::TPrint<::_::ROut>(printer, item);
}

::_::ROut& operator<<(::_::ROut& o, ::_::ROut& item) {
  return o;
}
#endif
#endif

#endif  //< _CrabsROutHXX
