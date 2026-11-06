// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsROutH
#define _CrabsROutH
#include <_Config.h>
#if _Seam >= _CrabsCore
#include "Stringf.hpp"
namespace _ {

/* Utility class for printing to the stdout. */
class ROut {
 public:
  /* Prints nothing. */
  ROut();

  /* Prints the given item to the ROut stream. */
  ROut(CHA item);
  ROut(const CHA* item);

  ROut(CHB item);
  ROut(const CHB* item);
  ROut(CHC item);
  ROut(const CHC* item);
  ROut(ISA item);
  ROut(IUA item);
  ROut(ISB item);
  ROut(IUB item);
  ROut(ISC item);
  ROut(IUC item);
  ROut(ISD item);
  ROut(IUD item);
  ROut(FPC item);
  ROut(FPD item);
  ROut(Hexf item);
  ROut(Binaryf item);
  ROut(Centerf item);
  ROut(Rightf item);
  ROut(Linef item);
  ROut(Repeatf item);
  ROut(Headingf item);
  ROut(Indentf item);
  ROut(Charsf item);

  /* Prints the given items to the ROut stream. */
  ROut(CHA item, CHA second_item);
  ROut(CHA item, const CHA* second_item);
  ROut(const CHA* item, CHA second_item);
  ROut(const CHA* item, const CHA* second_item);
  ROut(CHA item, CHA second_item, CHA third_item);
  ROut(CHA item, CHA second_item, const CHA* third_item);
  ROut(const CHA* item, const CHA* second_item, CHA third_item);
  ROut(const CHA* item, const CHA* second_item, const CHA* third_item);

  /* Returns reference to this. */
  ROut& Star();

  /* Prints the given item to the stdout. */
  ROut& Print(CHA item);
  ROut& Print(const CHA* item);
  ROut& Print(CHB item);
  ROut& Print(const CHB* item);
  ROut& Print(CHC item);
  ROut& Print(const CHC* item);
  ROut& Print(ISC item);
  ROut& Print(IUC item);
  ROut& Print(ISD item);
  ROut& Print(IUD item);
  ROut& Print(FPC item);
  ROut& Print(FPD item);
  ROut& Print(Hexf& item);
  ROut& Print(Binaryf& item);
  ROut& Print(Centerf& item);
  ROut& Print(Rightf& item);
  ROut& Print(Linef& item);
  ROut& Print(Repeatf& item);
  ROut& Print(Headingf& item);
  ROut& Print(Indentf& item);
  ROut& Print(Charsf& item);
  //ROut& Print(TypeWordValue item);

  /* Prints a new line followed by the given item to the stdout. */
  ROut& NL();

  /* Prints a new line followed by the given item to the stdout. */
  ROut& NL(CHA item);
  ROut& NL(const CHA* item);
  ROut& NL(CHB item);
  ROut& NL(const CHB* item);
  ROut& NL(CHC item);
  ROut& NL(const CHC* item);
  ROut& NL(ISC item);
  ROut& NL(IUC item);
  ROut& NL(ISD item);
  ROut& NL(IUD item);
  ROut& NL(FPC item);
  ROut& NL(FPD item);

  /* Stores the given item to the buffer and store the size. */
  ROut& Hex(const void* item);
  ROut& Hex(const void* item, ISW size);
  ROut& Hex(const void* item, const void* item_stop);
  ROut& Hex(ISA item);
  ROut& Hex(IUA item);
  ROut& Hex(ISB item);
  ROut& Hex(IUB item);
  ROut& Hex(ISC item);
  ROut& Hex(IUC item);
  ROut& Hex(ISD item);
  ROut& Hex(IUD item);
  ROut& Hex(FPC item);
  ROut& Hex(FPD item);

  /* Stores the given item to the buffer and store the size. */
  ROut& Binary(const void* item);
  ROut& Binary(const void* item, ISW size);
  ROut& Binary(const void* item, const void* item_stop);
  ROut& Binary(ISA item);
  ROut& Binary(IUA item);
  ROut& Binary(ISB item);
  ROut& Binary(IUB item);
  ROut& Binary(ISC item);
  ROut& Binary(IUC item);
  ROut& Binary(ISD item);
  ROut& Binary(IUD item);
  ROut& Binary(FPC item);
  ROut& Binary(FPD item);

  ISN PrintAndCount(const CHA* string);
  ISN PrintAndCount(const CHB* string);
  ISN PrintAndCount(const CHC* string);
};

// Returns the standard console out.
inline ROut& RefOut();

/* Wrapper function for ROut so you can use it more than once in a lexical
scope without declaring ROut cout.
inline ROut CPrint() { return ROut(); } */

/* Prints the given item to the ROut. */
ROut CPrint(CHA item);
ROut CPrint(const CHA* item);
ROut CPrint(CHB item);
ROut CPrint(const CHB* item);
ROut CPrint(CHC item);
ROut CPrint(const CHC* item);

ROut CPrint(ISC item);
ROut CPrint(IUC item);
ROut CPrint(ISD item);
ROut CPrint(IUD item);
ROut CPrint(FPC item);
ROut CPrint(FPD item);
ROut CPrint(Hexf& item);
ROut CPrint(Binaryf& item);
ROut CPrint(Centerf& item);
ROut CPrint(Rightf& item);
ROut CPrint(Linef& item);
ROut CPrint(Headingf& item);
ROut CPrint(Indentf& item);

}  //< namespace _

/* Prints the given item to the ROut. */
inline ::_::ROut& operator<<(::_::ROut& o, CHA item);
inline ::_::ROut& operator<<(::_::ROut& o, const CHA* item);

inline ::_::ROut& operator<<(::_::ROut& o, CHB item);
inline ::_::ROut& operator<<(::_::ROut& o, const CHB* item);
inline ::_::ROut& operator<<(::_::ROut& o, CHC item);
inline ::_::ROut& operator<<(::_::ROut& o, const CHC* item);
inline ::_::ROut& operator<<(::_::ROut& o, ISC item);
inline ::_::ROut& operator<<(::_::ROut& o, IUC item);
inline ::_::ROut& operator<<(::_::ROut& o, ISD item);
inline ::_::ROut& operator<<(::_::ROut& o, IUD item);
inline ::_::ROut& operator<<(::_::ROut& o, FPC item);
inline ::_::ROut& operator<<(::_::ROut& o, FPD item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Hexf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Binaryf item);

inline ::_::ROut& operator<<(::_::ROut& o, ::_::Headingf item);
#if _Seam >= _CrabsUniprinter
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Centerf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Rightf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Linef item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Repeatf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Indentf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Charsf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::Sizef item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::AErrorf item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::ATypef item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::ATypeValue item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::ATypePtr item);
inline ::_::ROut& operator<<(::_::ROut& o, ::_::ROut& item);
template<typename T, typename CH>
inline ::_::ROut& operator<<(::_::ROut& o, ::_::TSizeCodef<T, CH> item);
#endif
#endif
#endif  //< _CrabsROutH
