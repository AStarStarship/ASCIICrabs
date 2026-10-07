// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_UNIPRINTER_HPP
#define CRABS_UNIPRINTER_HPP
#include "Stringf.hpp"
#if SEAM >= CRABS_COUT
namespace _ {

/* @module Uniprinter
A collection of abstract print functions that can be remapped to print to any source.
*/

/* Prints the given string to the Printer. */
template<typename Printer, typename CHS = CHR, typename CHT = CHD>
Printer& TPrint(Printer& p, const CHS* element) {
  if (IsError(element)) return p;
  CHT c = 0;
  element = SScan(element, c);
  while (c) {
    p << c;
    element = SScan(element, c);
  }
  return p;
}

/* Prints the given string to the Printer and returns the count of characters
printed. */
template<typename Printer, typename CHS = CHR, typename CHT = CHD>
ISN TPrintAndCount(Printer& p, const CHS* element) {
  if (IsError(element)) return 0;
  ISN print_count = 0;
  CHT c = 0;
  element = SScan(element, c);
  while (c) {
    p << c;
    ++print_count;
    element = SScan(element, c);
  }
  return print_count;
}

/* Prints the following value to the console in Hex. */
template<typename Printer, typename IU>
Printer& TPrintHex(Printer& p, IU value) {
  enum { HexStringLengthSizeMax = sizeof(IU) * 2 + 3 };
  auto ui = ToUnsigned(value);
  for (ISC num_bits_shift = sizeof(IU) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    p << HexNibbleToUpperCase(IUA(ui >> num_bits_shift));
  }
  return p;
}

/* Prints the following value to the console in Hex. */
template<typename Printer>
Printer& TPrintHex(Printer& p, const void* value) {
  IUW ptr = IUW(value);
  return TPrintHex<Printer, IUW>(p, ptr);
}
template<typename Printer, typename IS, typename IU>
Printer& TPrintHex(Printer& p, IS value) {
  return TPrintHex<Printer, IU>(p, IU(value));
}
#if USING_FPC == YES_0
template<typename Printer>
Printer& TPrintHex(Printer& p, FPC value) {
  return TPrintHex<Printer, IUC>(p, *TPtr<IUC>(&value));
}
#endif
#if USING_FPD == YES_0
template<typename Printer>
Printer& TPrintHex(Printer& p, FPD value) {
  return TPrintHex<Printer, IUD>(p, *TPtr<IUD>(&value));
}
#endif
/* Prints the given hex memory block or POD value depending on the sign of the
byte_count.

If the byte_count is less than zero the value will be treated like a POD type,
which will need to get printed backwards on Big-endian systems. The purpose of
little endian is that it's easier to read the hex values when you print them,
which is frustrating because of how simple the conversion code is. If the
byte_count is greater than zero then the memory will be printed sequentially
one byte at a time. */
template<typename Printer>
Printer& TPrintHex(Printer& p, const void* origin, ISW byte_count) {
  if (IsError(origin)) return p;
  ISW delta;
  const IUA* cursor = TPtr<const IUA>(origin);
#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
  // Print the hex value backwards.
  if (byte_count < 0) {
    byte_count = -byte_count;
    delta = -1;
    cursor += byte_count - 1;
  } else {
    delta = 1;
  }
#else
  if (byte_count < 0) byte_count = -byte_count;
#endif
  while (--byte_count >= 0) {
#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
    IUA byte = *cursor;
    cursor += delta;
#else
    IUA byte = *cursor++;
#endif
    p << HexNibbleToUpperCase(byte >> 4) 
      << HexNibbleToUpperCase(byte & 0xf);
  }
  return p;
}

template<typename Printer>
Printer& TPrintHex(Printer& p, const void* start, const void* stop) {
  ISW delta = ISW(stop) - ISW(start);
  return TPrintHex<Printer>(p, start, delta);
}

template<typename Printer>
Printer& TPrint(Printer& p, Hexf& value) {
  return TPrintHex<Printer>(p, value.element.Value(), value.element.count);
}
/* Prints the memory beginning at start to the Printer. */
template<typename Printer>
Printer& TPrintBinary(Printer& p, const void* start, ISW byte_count) {
  if (IsError(start)) return p;
  const IUA* cursor = TPtr<const IUA>(start);
  ISA delta = 1;
#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
  if (byte_count < 0) {
    delta = -1;
    byte_count = -byte_count;
    cursor += byte_count - 1;
  }
#endif
  while (--byte_count >= 0) {
    IUA c = *cursor;
    cursor += delta;
    for (ISW i = 8; i > 0; --i) {
      p << CHA('0' + (c >> 7));
      c = c << 1;
    }
  }
  return p;
}

template<typename Printer>
inline Printer& TPrintBinary(Printer& p, const void* start, const void* stop) {
  ISW delta = ISW(stop) - ISW(start);
  return TPrintBinary<Printer>(p, start, delta);
}

template<typename Printer, typename IU>
Printer& TPrintBinary(Printer& p, IU value) {
  enum { Size = sizeof(IU) * 8 };
  auto ui = ToUnsigned(value);
  for (ISC i = Size; i > 0; --i) {
    CHA c = CHA('0' + (ui >> (Size - 1)));
    p << c;
    ui = ui << 1;
  }
  return p;
}

template<typename Printer, typename IS, typename IU>
Printer& TPrintBinary(Printer& p, IS value) {
  return TPrintBinary<Printer, IU>(p, (IU)value);
}
#if USING_FPC == YES_0
template<typename Printer>
Printer& TPrintBinary(Printer& p, FPC value) {
  return TPrintBinary<Printer, IUC>(p, *TPtr<IUC>(&value));
}
#endif
#if USING_FPD == YES_0
template<typename Printer>
Printer& TPrintBinary(Printer& p, FPD value) {
  return TPrintBinary<Printer, IUD>(p, *TPtr<IUD>(&value));
}
#endif

template<typename Printer>
Printer& TPrint(Printer& p, Binaryf& value) {
  return TPrintBinary<Printer>(p, value.element.Value(), value.element.count);
}

template<typename Printer>
Printer& TPrintAligned(Printer& p, const CHA* element, ISW char_count,
                       ISW left_count, ISW dot_count, ISW right_count) {
  while (--left_count > 0) p << ' ';
  while (--char_count > 0) {
#if LARGEST_CHAR == 1 || SEAM < CRABS_COUT
    p << *element++;
#else
    CHC c;
    str = SScan(str, c);
    if (IsError(str)) 
      return p;
    p << c;
#endif
  }
  while (--dot_count > 0) p << ' ';
  while (--right_count > 0) p << ' ';
  return p;
}

template<typename Printer>
Printer& TPrintAlignedHex(Printer& p, const void* origin, ISW byte_count,
                          ISW left_count, ISW dot_count, ISW right_count) {
  const CHA* cursor = TPtr<const CHA>(origin);
  while (--left_count > 0) p << ' ';
  // TPrintHex<Printer>(p, origin, byte_count >> 1);
  TPrintHex<Printer>(p, cursor, -byte_count);
  while (--dot_count > 0) p << ' ';
  while (--right_count > 0) p << ' ';
  return p;
}

template<typename Printer, typename T, typename CHS>
inline Printer& TPrint(Printer& p, TSizeCodef<T, CHS>& value) {
  return TPrintSizeCodef<Printer>(p, value);
}

/* Prints the given token aligned center the given column_count.
@return Nil if any of the pointers are nil or if column_count < 1, and a
pointer to the nil-term CHA upon success.
@param token  The token to utf.
@param column_count The number_ of columns to align right to. */
template<typename Printer, typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
Printer& TPrintCenter(Printer& p, const CHS* element, IS column_count = 80) {
  if (IsError(element) || column_count < 1) 
    return p;
  IS char_count = 0;
  const CHS* token_end = TSStopCharCount<CHS, CHT>(element, char_count);
  if (IsError(token_end)) 
    return p;
  if (element == token_end) 
    return p;
  IS code_count = token_end - element, 
    space_count = column_count - char_count;

  if (space_count > 0) {
    IS half_count = space_count >> 1;
    space_count -= half_count;
    while (half_count-- > 0) p << ' ';
    p << element;
    while (space_count-- > 0) p << ' ';
    return p;
  }
  char_count = (-char_count) - 3;
  if (char_count < 0) {
    while (--char_count > 0) p << '.';
  } else {
    while (char_count > 0) {
      CHT c = 0;
			element = SScan(element, c);  
      p << c;
    }
    p << "...";
  }
  return p;
}

/* Prints th given value centered unless it's count is less then 0, in which
case it will print the POD value stored in the first Word of the string. */
template<typename Printer>
Printer& TPrintCenter(Printer& p, Stringf& value) {
  ISW column_count = value.Count();
  if (column_count < 0) {  // Print hex.
    column_count = -column_count;
    ISW byte_count = ATypeSizeOfPOD(DTB(value.Type())), left_count, dot_count;
    if (byte_count < 0) return p;
    byte_count = byte_count << 1;
    ISW right_count;
    if (byte_count > column_count) {
      while (--column_count > 0) p << ' ';
      return p;
    } else {
      left_count = (column_count - byte_count) >> 1;
      dot_count = 0;
      right_count = column_count - left_count - byte_count;
    }
    return TPrintAlignedHex<Printer>(p, value.Value(), byte_count >> 1,
                                     left_count, dot_count, right_count);
  }
  switch (ATypeTextFormat(value.Type())) {
#if USING_STA == YES_0
    case 1: {
      return TPrintCenter<Printer, CHA>(p, value.STA(), column_count);
    }
#endif
#if USING_STB == YES_0
    case 2: {
      return TPrintCenter<Printer, CHB>(p, value.STB(), column_count);
    }
#endif
#if USING_STC == YES_0
    case 3: {
      return TPrintCenter<Printer, CHC>(p, value.STC(), column_count);
    }
#endif
  }
  return p;
}

/* Prints the given cursor center aligned to the given column_count. */
template<typename Printer>
inline Printer& TPrint(Printer& p, Centerf& value) {
  return TPrintCenter<Printer>(p, value.element);
}

/* Prints the given str aligned right the given column_count.
@return Nil if any of the pointers are nil or if column_count < 1, and a
pointer to the nil-term CHA upon success.
@param str  The string to print.
@param column_count The number_ of columns to align right to. */
template<typename Printer, typename CHS = CHR, typename CHT = CHD>
Printer& TPrintRight(Printer& p, const CHS* element, ISW column_count = 80) {
  if (IsError(element) || column_count < 1)
    return p;

  // const CHS* token_end = TSStop<CHS, CHT>(value);
  const CHS* token_end = element;
  while (token_end++);
  if (element == token_end)
    return p;
  ISW length = token_end - element, space_count = column_count - length;

  if (space_count > 0) {
    while (space_count-- > 0) p << ' ';
    p << element;
    return p;
  }
  length = (-length) - 3;
  if (length < 0) {
    switch (length) {
      case 1:
        p << '.';
        break;
      case 2:
        p << "..";
        break;
      case 3:
        p << "...";
        break;
    }
  } else {
    while (length > 0) {
      CHT c = 0;
      element = SScan(element, c);
      p << c;
    }
    p << "...";
  }
  return p;
}

/* Prints the given value centered, printing it as hex if the value.Count() is
less than 0. */
template<typename Printer>
Printer& TPrintRight(Printer& p, Stringf& value) {
  ISW column_count = value.Count();
  if (column_count < 0) {  // Print hex.
    column_count = -column_count;
    ISW char_count = ATypeSizeOfPOD(DTB(value.Type()));
    if (char_count < 0) return p;

    ISW left_count, dot_count;
    if (char_count > column_count) {
      left_count = 0;
      if (column_count < 3) {
        char_count = 0;
        dot_count = column_count;
      } else {
        char_count = column_count - 3;
        dot_count = 3;
      }
    } else {
      left_count = column_count - char_count;
      dot_count = 0;
    }
    return TPrintAlignedHex<Printer>(p, value.STA(), char_count, left_count,
                                     dot_count, 0);
  }
  ISW count = value.Count();
  switch (ATypeTextFormat(value.Type())) {
#if USING_STA == YES_0
    case 1: {
      return TPrintRight<Printer, CHA>(p, value.STA(), count);
    }
#endif
#if USING_STB == YES_0
    case 2: {
      return TPrintRight<Printer, CHB>(p, value.STB(), count);
    }
#endif
#if USING_STC == YES_0
    case 3: {
      return TPrintRight<Printer, CHC>(p, value.STC(), count);
    }
#endif
  }
  return p;
}

template<typename Printer>
Printer& TPrint(Printer& p, Rightf& value) {
  return TPrintRight<Printer>(p, value.element);
}

template<typename Printer, typename CHS>
Printer& TPrintRepeat(Printer& p, CHS c, ISW count) {
  while (--count >= 0) p << c;
  return p;
}

template<typename Printer, typename CHS = CHR, typename CHT = CHD>
Printer& TPrintLine(Printer& p, CHS token = '-', ISW column_count = 80) {
  p << '\n';
  TPrintRepeat<Printer, CHS>(p, token, column_count);
  return p << '\n';
}

/* Prints a formatted lines.
The first two chars of the string are going to be the corner and margin chars.
It's easiest to show with the examples below. Like their names implies, the
corner char is for the edges and the margin is for inside of a header block.

@code
TPrintBreak<CHA>("\n+---\n\n| Foo\n\n+---\n", 10);

TPrintBreak<CHA>("\n+---\n\n| Foo\n\n+---\n", 10);
//>>>
//>>> +----------
//>>> | Foo *****
//>>> +----------
//>>>

TPrintBreak<CHA> ("- \n---\n---\n\n   Foo\n\n---\n---", 10);
//>>> -----------
//>>> -----------
//>>>
//>>>    Foo
//>>>
//>>> -----------
//>>> -----------
@endcode
*/
template<typename Printer, typename CHS = CHR, typename CHT = CHD>
const CHS* TPrintLinef(Printer& p, const CHS* style = NILP,
                       ISW column_count = 80) {
  enum {
    StateScanningDifferentChars = 0,
    StateStateDuplicateChar = 1,
    BreakCount = 3,
  };
  if (IsError(style)) style = TSLinef<CHS>();
  if (column_count < BreakCount) return NILP;

  ISW state = StateScanningDifferentChars;
  CHS current_char = *style++,  //
      prev_char = ~current_char;
  ISC hit_count = 0, column_index = 0;
  while (current_char) {
    p << current_char;
    if (current_char == '\n')
      column_index = 0;
    else
      ++column_index;
    switch (state) {
      case StateScanningDifferentChars: {
        if (current_char == prev_char && !TIsWhitespace<CHS>(current_char)) {
          state = StateStateDuplicateChar;
          hit_count = 1;
        }
        break;
      }
      case StateStateDuplicateChar: {
        if (current_char != prev_char)
          state = StateScanningDifferentChars;
        else if (++hit_count >= BreakCount - 1) {
          TPrintRepeat<Printer, CHS>(p, current_char,
                                     column_count - column_index);
          column_index = hit_count = 0;
        }
        break;
      }
    }
    prev_char = current_char;
    current_char = *style++;
  }
  return style;
}

template<typename Printer>
Printer& TPrint(Printer& p, Linef& value) {
  ISW type = value.element.value.Type(),  //
      format = ATypeTextFormat(type);
  switch (format) {
#if USING_STA == YES_0
    case _SWA: {
      TPrintLinef<Printer, CHA, CHC>(p, value.element.ToSTA(), value.element.count);
      break;
    }
#endif
#if USING_STB == YES_0
    case _SWB: {
      TPrintLinef<Printer, CHB, CHC>(p, value.element.ToSTB(), value.element.count);
      break;
    }
#endif
#if USING_STC == YES_0
    case _SWC: {
      TPrintLinef<Printer, CHC, CHC>(p, value.element.ToSTC(), value.element.count);
      break;
    }
#endif
    default: {
      switch (type & ATypePODMask) {
#if USING_STA == YES_0
        case _CHA: {
          CHA c = CHA(value.element.value.Value());
          TPrintLine<Printer, CHA>(p, c, value.element.count);
          break;
        }
#endif
#if USING_STB == YES_0
        case _CHB: {
          CHB c = CHB(value.element.value.Value());
          TPrintLine<Printer, CHB>(p, c, value.element.count);
          break;
        }
#endif
#if USING_STC == YES_0
        case _CHC: {
          CHC c = CHC(value.element.value.Value());
          TPrintLine<Printer, CHC>(p, c, value.element.count);
          break;
        }
#endif
      }
    }
  }
  return p;
}

template<typename Printer>
Printer& TPrint(Printer& p, Repeatf& value) {
  ISW type = value.element.Type(),  //
    format = ATypeTextFormat(type);
  switch (format) {
  #if USING_STA == YES_0
    case _SWA: {
      for (ISW i = 0; i < value.count; ++i)
        p << value.element.STA();
      break;
    }
  #endif
  #if USING_STB == YES_0
    case _SWB: {
      for (ISW i = 0; i < value.count; ++i)
        p << value.element.STB();
      break;
    }
  #endif
  #if USING_STC == YES_0
    case _SWC: {
      for (ISW i = 0; i < value.count; ++i)
        p << value.element.STC();
      break;
    }
  #endif
  }
  return p;
}

template<typename CHS>
const CHS* TStringHeadingf() {
  static const CHS String[] = {'\n', '\n', '+', '-', '-', '-', '\n', '|', ' ',
                              NIL,  '\n', '+', '-', '-', '-', '\n', NIL};
  return String;
}

/* Prints a heading with the */
template<typename Printer, typename CHS>
Printer& TPrintHeading(Printer& p, const CHS* element, const CHA* style = NILP,
                       ISW column_count = 80, const CHA* caption2 = NILP,
                       const CHA* caption3 = NILP) {
  if (IsError(style)) style = TStringHeadingf<CHA>();
  style = TPrintLinef<Printer, CHA, CHC>(p, style, column_count);
  if (IsError(style)) return p;
  p << element;
  if (caption2) p << caption2;
  if (caption3) p << caption3;
  TPrintLinef<Printer, CHA, CHC>(p, style, column_count);
  return p;
}

/* Prints the a formatted header. */
template<typename Printer>
Printer& TPrint(Printer& p, Headingf& value) {
  ISA encoding = ATypeTextFormat(value.Type());
  switch (encoding) {
#if USING_STA == YES_0
    case _SWA: {
      return TPrintHeading<Printer, CHA>(p, value.element.STA(), value.style,
        value.element.Count(), value.caption2, value.caption3);
    }
#endif
#if USING_STB == YES_0
    case _SWB: {
      return TPrintHeading<Printer, CHB>(p, value.element.STB(), value.style,
        value.element.Count(), value.caption2, value.caption3);
    }
#endif
#if USING_STC == YES_0
    case _SWC: {
      return TPrintHeading<Printer, CHC>(p, value.element.STC(), value.style,
        value.element.Count(), value.caption2, value.caption3);
    }
#endif
  }
  return p;
}

template<typename Printer, typename CHS = CHR, typename CHT = CHD>
Printer& TPrintChars(Printer& p, const CHS* start, const CHS* stop) {
  if (IsErrorSocket(start, stop)) return p;

  ISW bytes = stop - start + 1;

  p << STAPrintCharsHeader() << STAPrintCharsBorder() << Hexf(start);
  int i = 0;
  CHS c;
  const CHS* address_to_print = start;
  while (start <= stop) {
    p << '\n' << '|';
    for (ISC i = 0; i < 64; ++i) {
      c = *start;
      if (start++ > stop) {
        c = 'x';
      } else if (c < ' ') {
        address_to_print = start;
        c += APrintC0Offset;
      }
      if (c == 127) c = ' ';
      p << c;
    }
    p << "| " << Hexf(address_to_print - 1);
  }
  return p << STAPrintCharsBorder() << "Bytes printed:" << bytes;
}

template<typename Printer, typename CHS = CHR, typename CHT = CHD>
inline Printer& TPrintChars(Printer& p, const CHS* start, ISW count) {
  return TPrintChars<Printer, CHS>(p, start, start + count - 1);
}

template<typename Printer>
Printer& TPrint(Printer& p, Charsf& value) {
  auto element = value.element;
  ISW count = element.Count();
  switch (ATypeTextFormat(element.Type())) {
#if USING_STA == YES_0
    case 1: {
      return TPrintChars<Printer, CHA>(p, element.ToSTA(), count);
    }
#endif
#if USING_STB == YES_0
    case 2: {
      return TPrintChars<Printer, CHB>(p, element.ToSTB(), count);
    }
#endif
#if USING_STC == YES_0
    case 3: {
      return TPrintChars<Printer, CHC>(p, element.ToSTC(), count);
    }
#endif
  }
  return TPrintChars<Printer, CHA>(p, TPtr<CHA>(element.Value()), count);
}

template<typename Printer>
Printer& TPrintIndent(Printer& p, ISW indent_count) {
  p << '\n';
  while (--indent_count >= 0) p << ' ';
  return p;
}

template<typename Printer>
Printer& TPrint(Printer& p, const Indentf& value) {
  return TPrintIndent<Printer>(p, value.indent_count);
}

}  //< namespace _
#endif  //< #if SEAM >= CRABS_COUT

#include "Puff.hpp"

#if SEAM >= CRABS_UNIPRINTER
namespace _ {

/* Prints the given sizef to the printer. */
template<typename Printer>
Printer& TPrint(Printer& p, Sizef& value) {
  auto v = value.size;
  if (v < 0) return p << CHA('@' + (-v));
  return p << v;
}

// Prints an ASCII POD type to the printer.
template<typename Printer, typename DT = DTB>
Printer& TPrintATypePOD(Printer& p, DT type) {
  DT pod_type = type & ATypePODMask,        //
      vector_type = TTypeVector<DT>(type),  //
      map_type = TTypeMap<DT>(type);        //
}

/* Prints a string representation of an ASCII Data Type to the printer.
16-bit ASCII Bit Pattern:
| b15 | b14:b13 | b12:b9 | b8:b7 | b6:b5 | b4:b0 |
|:---:|:-------:|:------:|:-----:|:-----:|:-----:|
| CNS |    MD   |   MT   |  SW   |  VT   |  POD  | */
template<typename Printer>
Printer& TPrintAType(Printer& p, DTB type) {
  DTB cns_bit = DTB(type >> ATypeCNSBit);
  type ^= cns_bit << ATypeCNSBit;
  DTB md_bits = DTB(type >> ATypeMDBit0);
  if (md_bits) {
    if (md_bits >> 1) {  // Const.
      type ^= md_bits << ATypeMDBit0;
      p << TATypeMODs<>(md_bits) << '_';
    }
  }
  if (type < ATypePODTotal) return p << TATypePODs<>(type);  // POD Type
  // Processing from MOD to POD/left to right.
  auto map_type = type >> ATypeMTBit0;
  type ^= map_type << ATypeMTBit0;
  auto size_width = type >> ATypeSWBit0;
  type ^= size_width << ATypeSWBit0;
  auto vector_type = type >> ATypeVTBit0;
  type ^= vector_type << ATypeVTBit0;
  // type is now the 5-bit pod type.
  DTB ext_type = ATypeToEXT(type);
  auto actxh = ACTX()->handler;
  if (ext_type > 0) {
    const CHR* type_string = NILP;
    if (ext_type >= ATypeCTXStart) ext_type -= ATypeCTXStart;
    if (actxh) {
      CHR boofer[ACTXLengthMax + 1];
      actxh(boofer, boofer + ACTXLengthMax + 1, type, 0, 0);
    }
    return p << type_string;
  }
  if (type >= _PCa && type <= _PCl)
    if (actxh) type = DTB(actxh(NILP, NILP, ext_type, 0, 0));

  if (map_type)
    return p << TATypeMaps<>(map_type >> 4) << '_'
             << TATypePODs<>(map_type & 0xf) << '_' << TATypePODs<>(type);
  if (vector_type || !vector_type && size_width)
    return p << TATypeVectors<>(size_width, vector_type) << '_'
             << TATypePODs<>(type);
  if (ATypeIsCH(type) && map_type == 0)  // Then it's a string, loom, or.
    return p << "ST" << ASizeCodef(size_width);
  return p << TATypePODs<>(type);
}
// NIL->CHA   -> 0x01351f =: "dez nutz!\0"
// ANY->NIL 16-bit ->
// 0x31E0 -> 5 LSb are 0 -> Shift over 5 bits to get number bytes.
// 16-bits - 5-bits = 11 => 2^11 = 2048
/**/
template<typename Printer>
Printer& TPrintAType(Printer& p, DTC type) {
  BOL first_time = true;
  ISN count = sizeof(DTC) / sizeof(DTB);
  while (count-- > 0) {
    if (!first_time) {
      p << ' ';
    } else {
      first_time = false;
    }
    TPrintAType<Printer>(p, DTB(type));
    type = type >> 16;
    if (type == 0) return p;
  }
  return p;
}
template<typename Printer>
Printer& TPrintAType(Printer& p, DTD type) {
  BOL first_time = true;
  ISN count = sizeof(DTD) / sizeof(DTB);
  while (count-- > 0) {
    if (!first_time) {
      p.Print('-');
    } else {
      first_time = false;
    }
    TPrintAType<Printer>(p, DTB(type));
    type = type >> 16;
    if (type == 0) return p;
  }
  return p;
}

template<typename Printer>
Printer& TPrintAError(Printer& p, ISW error, const CHR* message = NILP) {
  if (error >= 0) {
    if (message) return p << message;
    return p;
  }
  if (IsError(message)) return p << TAErrors<CHR, ISW>(error);
  return p << "Error " << TAErrors<CHR, ISW>(error) << ": " << message;
}

template<typename Printer>
Printer& TPrintFP(Printer& p, IUW value, IUW vmsb) {
  if (vmsb != 0) {
    return p << "0d" << ISW(value) << '_' << ISW(vmsb);
  }
  return p << FPD(value);
}

template<typename Printer>
inline Printer& TPrintIU(Printer& p, IUW value, IUW vmsb) {
  if (vmsb != 0) return p << "0d" << ISW(value) << '_' << ISW(vmsb);
#if CPU_SIZE == CPU_8_BYTE
  if (value >> 32 == 0) return p << IUC(value);
#endif
  return p << value;
}

template<typename Printer>
inline Printer& TPrintIS(Printer& p, IUW value, IUW vmsb) {
  if (vmsb != 0) return p << "0d" << value << '_' << vmsb;
#if CPU_SIZE == CPU_8_BYTE
  if (value >> 32 == 0) return p << ISC(value);
#endif
  return p << value;
}

template<typename Printer>
inline Printer& TPrintCH(Printer& p, IUW value) {
  return p << CHC(value);
}

template<typename Printer>
inline Printer& TPrintTMD(Printer& p, IUW value) {
  return p << "time:" << value;
}

template<typename Printer>
Printer& TPrintTME(Printer& p, IUW value, IUW vmsb) {
  return p << "time:" << value << '_' << vmsb;
}

/* Prints the value of the given type-value tuple
@pre Type must be less than _EPa. */
template<typename Printer>
Printer& TPrintPOD_NC(Printer& p, DTW type, IUW value, IUW vmsb = 0) {
  switch (type & 3) {
    case 0:
      return TPrintFP<Printer>(p, value, vmsb);
    case 1:
      return TPrintIU<Printer>(p, value, vmsb);
    case 2:
      return TPrintIS<Printer>(p, value, vmsb);
    case 3: {
      if (type >= _SSD) {
        if (type == _SSE) return TPrintTME<Printer>(p, value, vmsb);
        return TPrintTMD<Printer>(p, value);
      }
      return TPrintCH<Printer>(p, value);
    }
  }
  return p;
}

/* Prints the value of the given type-value tuple.
| b15 | b14:b13 | b12:b9 | b8:b7 | b6:b5 | b4:b0 |
|:---:|:-------:|:------:|:-----:|:-----:|:-----:|
| CNS |    MD   |   MT   |  SW   |  VT   |  POD  |*/
template<typename Printer>
Printer& TPrintValue(Printer& p, DTW type, IUW value, IUW vmsb = 0) {
  type &= DTB(-1) >> 1;  //< Don't care if it's const.
  DTW MD = (type >> ATypeMDBit0) & 0x3;
  ISW size_width = 0;
  ACTXHandler actxh = ACTX()->handler;
  if (MD == _MDP) {
    DTW type_new = type ^ MD;
    if (type_new < ATypePODTotal) {  // Dereference pointer to POD type.
      type = ATypeMDDeassert(type);
      if (type == 0) return p;
      if (type > _PCa) {
        if (IsError(actxh)) return p;
        type = actxh(NILP, NILP, type, 0, 0);
      }
      switch (type >> 2) {
        case 0: {
          value = *TPtr<IUA>(value);
          break;
        }
        case 1: {
          value = *TPtr<IUB>(value);
          break;
        }
        case 2: {
#if CPU_SIZE == CPU_2_BYTE
          IUD* ptr = TPtr<IUB>(value);
          value = *ptr++;
          vmsb = *ptr;
#else
          value = *TPtr<IUC>(value);
#endif
          break;
        }
        case 3: {
#if CPU_SIZE == CPU_8_BYTE
          value = *TPtr<IUD>(value);
#elif CPU_SIZE == CPU_4_BYTE
          IUC* ptr = TPtr<IUC>(value);
          value = *ptr++;
          vmsb = *ptr;
#else
          size_width = 8;
#endif
          break;
        }
        case 4: {
#if CPU_SIZE == CPU_8_BYTE
          IUD* ptr = TPtr<IUD>(value);
          value = *ptr++;
          value = *ptr;
#else
          size_width = 16;
#endif
          break;
        }
      }
    }
  }
  if (type < ATypePODTotal) {
    if (type > _PCa) {
      if (IsError(actxh)) return p;
      type = actxh(NILP, NILP, type, 0, 0);
    }
    return TPrintPOD_NC<Printer>(p, type, value, vmsb);
  }
  const DTW MT = type >> ATypeMTBit0;
  type ^= MT << ATypeMTBit0;
  const DTW SW = type >> ATypeSWBit0;
  type ^= SW << ATypeSWBit0;
  const DTW VT = type >> ATypeVTBit0;
  type ^= VT << ATypeVTBit0;
  if (MT > 0) {
    if (type < ATypePODTotal) {  // Map of one POD type to another.
      return p << TATypePODs<>(type);
    }
  }
  const void* value_ptr = TPtr<const void>(value);
  p << "total:";
  if (VT == _ARY) {
    if (SW == 0)
      p << *(ISA*)(value_ptr);
    else if (SW == 1)
      p << *(ISB*)(value_ptr);
    else if (SW == 2)
      p << *(ISC*)(value_ptr);
    else if (SW == 3)
      p << *(ISD*)(value_ptr);
    return p;
  } else if (VT == _SCK) {
    if (SW == 0) {
      auto cursor = TPtr<const ISA>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 1) {
      auto cursor = TPtr<const ISB>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 2) {
      auto cursor = TPtr<const ISC>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 3) {
      auto cursor = TPtr<const ISD>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    }
  } else if (VT == _MTX) {
    if (SW == 0) {
      auto cursor = TPtr<const ISA>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 1) {
      auto cursor = TPtr<const ISB>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 2) {
      auto cursor = TPtr<const ISC>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    } else if (SW == 3) {
      auto cursor = TPtr<const ISD>(value_ptr);
      p << *cursor++ << " count:" << *cursor;
    }
  }
  return p;
}
template<typename Printer>
Printer& TPrintValue(Printer& p, DTW type, const void* value, ISW vbytes) {
  return TPrintValue<Printer>(p, type, IUW(ISW(value) + vbytes));
}

// Prints ASCII type and the value tuple.
template<typename Printer>
Printer& TPrintATypeValue(Printer& p, DTB type, IUW value, IUW value_msb = 0) {
  TPrintAType<Printer>(p, type);
  p << ':';
  return TPrintValue<Printer>(p, type, value, value_msb);
}

// Prints ASCII type and the value tuple.
template<typename Printer>
Printer& TPrintATypeValue(Printer& p, DTB type, const void* value) {
  return TPrintATypeValue<Printer>(p, type, IUW(value));
}

// Prints ASCII type and the value tuple.
template<typename Printer, typename IS = ISW>
Printer& TPrintATypeValue(Printer& p, DTB type, const void* base_ptr,
                          IS offset) {
  return TPrintATypeValue<Printer>(p, type, IUW(ISW(base_ptr) + offset));
}

// Prints ASCII type and the value tuple.
template<typename Printer>
Printer& TPrintATypeValue(Printer& p, DTC type, const void* value) {
  p << TPrintATypeValue<Printer>(p, DTB(type >> 16), IUW(value));
  return p << TPrintATypeValue<Printer>(p, DTB(type), IUW(value));
}

// Prints ASCII type and the value tuple.
template<typename Printer, typename IS = ISW>
Printer& TPrintATypeValue(Printer& p, DTC type, const void* base_ptr,
                          IS offset) {
  const void* value = TPtr<void>(base_ptr, offset);
  p << TPrintATypeValue<Printer>(p, DTB(type >> 16), IUW(value));
  return p << TPrintATypeValue<Printer>(p, DTB(type), IUW(value));
}

// Prints ASCII type and the value tuple.
template<typename Printer>
Printer& TPrintATypeValue(Printer& p, DTD type, const void* value) {
  p << TPrintATypeValue<Printer>(p, DTB(type >> 48), IUW(value));
  p << TPrintATypeValue<Printer>(p, DTB(type >> 32), IUW(value));
  p << TPrintATypeValue<Printer>(p, DTB(type >> 16), IUW(value));
  return p << TPrintATypeValue<Printer>(p, DTB(type), IUW(value));
}

// Prints ASCII type and the value tuple.
template<typename Printer, typename IS = ISW>
Printer& TPrintATypeValue(Printer& p, DTD type, const void* base_ptr,
                          IS offset) {
  const void* value = TPtr<void>(base_ptr, offset);
  p << TPrintATypeValue<Printer>(p, DTB(type >> 48), IUW(value));
  p << TPrintATypeValue<Printer>(p, DTB(type >> 32), IUW(value));
  p << TPrintATypeValue<Printer>(p, DTB(type >> 16), IUW(value));
  return p << TPrintATypeValue<Printer>(p, DTB(type), IUW(value));
}

/* Prints ASCII type and the value tuple in CSV format. */
//template<typename Printer, typename CHS = CHR, typename CHT = CHD>
//Printer& TPrintATypeCSV(Printer& o, const CHS* types) {
//  const CHS* token_start = types;
//  ISW state = 0;
//  CHT c = 0;
//  types = SScan(types, c);
//  while (!IsntPrintable(c)) {
//  }
//  return o;
//}

/* A dumb fast templated String Printer.
Dumb means that it does not count up the number of Unicode characters. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISW>
struct TSPrinter {
  CHS* start,  //< Start address.
    * stop;    //< Stop address.

  /* Default constructor does nothing. */
  TSPrinter() : start(0), stop(0) {}

  /* Initializes the UTF& from the given origin pointers.
  @param start The origin of the socket.
  @param count The number of CHS in the boofer. */
  TSPrinter(CHS* start, ISA size) : start(start), stop(start + size - 1) {
    Reset();
  }

  /* Initializes the UTF& from the given origin pointers.
  @param start The origin of the socket.
  @param count The number of CHS in the boofer. */
  TSPrinter(CHS* start, ISB size) : start(start), stop(start + size - 1) {
    Reset();
  }

  /* Initializes the UTF& from the given origin pointers.
  @param start The origin of the socket.
  @param count The number of CHS in the boofer. */
  TSPrinter(CHS* start, ISC size) : start(start), stop(start + size - 1) {
    Reset();
  }

  /* Initializes the UTF& from the given origin pointers.
  @param start The origin of the socket.
  @param count The number of CHS in the boofer. */
  TSPrinter(CHS* start, ISD size) : start(start), stop(start + size - 1) {
    Reset();
  }

  /* Initializes the array pointers from the given start and stop pointers.
  @param start The start of the array.
  @param stop  The stop of the array. */
  TSPrinter(CHS* start, CHS* stop) : start(start), stop(stop) { Reset(); }

  /* Clones the other printer. */
  TSPrinter(const TSPrinter& other)
      : start(other.start), stop(other.stop) {}

  /* Points to the end of the contiguous linear socket. */
  IUA* End() { return TPtr<IUA>(stop) + sizeof(CHS); }

  IS CBytes() { return IS(stop - start + sizeof(CHS)); }

  void Wipe() { ArrayFill(start, stop); }

  /* Writes a nil-term CHA at the start of the string. */
  inline CHS* Reset() {
    *start = 0;
    return start;
  }

  /* Sets the start pointer to the new_pointer. */
  inline TSPrinter& Set(CHS* cursor) {
    start = cursor;
    return *this;
  }

  /* Sets the start pointer to the new_pointer. */
  inline TSPrinter& Set(IUW* boofer) {
    IS size = *TPtr<IS>(boofer);
    IUW ptr = IUW(boofer) + sizeof(IS);
    CHS* start_ptr = TPtr<CHS>(ptr);
    start = start_ptr;
    stop = start_ptr + size - 1;
    return *this;
  }

  /* Sets the start pointer to the new_pointer. */
  inline void Set(const TSPrinter<CHS, CHT, IS>& other) {
    start = other.start;
    stop = other.stop;
  }

  /* Finds the length of the STR in Chars. */
  inline IS SpaceLeft() { return (IS)(stop - start); }

  /* Calculates the max length of the string in Chars. */
  inline IS LengthMax() { return stop - start; }

  /* Prints a value to the string. */
  inline TSPrinter& PrintChar(CHA value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
  inline TSPrinter& PrintChar(CHB value) {
    return Set(TSPrint<CHS, CHT>(start, stop, value));
  }
  inline TSPrinter& PrintChar(CHC value) {
    return Set(TSPrint<CHS, CHT>(start, stop, value));
  }
  inline TSPrinter& Print(CHA value) { return PrintChar(value); }
  inline TSPrinter& Print(CHB value) { return PrintChar(value); }
  inline TSPrinter& Print(CHC value) { return PrintChar(value); }
  inline TSPrinter& Print(const CHA* value) {
    return Set(TSPrint<CHS, CHT>(start, stop, value));
  }
  inline TSPrinter& Print(const CHB* value) {
    return Set(TSPrint<CHS, CHT>(start, stop, value));
  }
  inline TSPrinter& Print(const CHC* value) {
    return Set(TSPrint<CHS, CHT>(start, stop, value));
  }
  inline TSPrinter& Print(ISC value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
  inline TSPrinter& Print(IUC value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
  inline TSPrinter& Print(ISD value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
  inline TSPrinter& Print(IUD value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
#if USING_FPC == YES_0
  inline TSPrinter& Print(FPC value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
#endif
#if USING_FPD == YES_0
  inline TSPrinter& Print(FPD value) {
    return Set(TSPrint<CHS>(start, stop, value));
  }
#endif

  /* Prints the given pointer as hex. */
  inline TSPrinter& Hex(ISA value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(IUA value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(ISB value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(IUB value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(ISC value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(IUC value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(ISD value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
  inline TSPrinter& Hex(IUD value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
#if USING_FPC == YES_0
  inline TSPrinter& Hex(FPC value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
#endif
#if USING_FPD == YES_0
  inline TSPrinter& Hex(FPD value) {
    return Set(TSPrintHex<CHS>(start, stop, value));
  }
#endif
  inline TSPrinter& Hex(const void* ptr) {
    return Set(TSPrintHex<CHS>(start, stop, ptr));
  }
  inline TSPrinter& Binary(ISA value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(IUA value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(ISB value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(IUB value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(ISC value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(IUC value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(ISD value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
  inline TSPrinter& Binary(IUD value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
#if USING_FPC == YES_0
  inline TSPrinter& Binary(FPC value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
#endif
#if USING_FPD == YES_0
  inline TSPrinter& Binary(FPD value) {
    return Set(TPrintBinary<CHS>(start, stop, value));
  }
#endif
  /* Prints the given pointer as binary. */
  inline TSPrinter& Binary(const void* ptr) {
    IUW address = IUW(ptr);
    return Set(TPrintBinary<CHS>(start, stop, address));
  }
  /*
  inline TSPrinter<CHS, CHT, IS>& Star() {} */

  template<typename Printer>
  inline Printer& PrintTo(Printer& p) {
    p << "\nTSPrinter<CHS" << sizeof(CHS) << ", IS" << sizeof(IS) << ">{ start:";
    TSPrintHex<Printer>(p, start);
    p << " stop:";
    TSPrintHex<Printer>(p, stop);

#ifdef D_THIS
// return TPrintChars<Printer, CHS>(p, start, stop);
#else
    return p << " }\n";
#endif
  }
};

}  //< namespace _

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          CHA value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          const CHA* value) {
  return p.Print(value);
}

#if USING_STB == YES_0
template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          CHB value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          const CHB* value) {
  return p.Print(value);
}
#endif
#if USING_STC == YES_0
template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          CHC value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          const CHC* value) {
  return p.Print(value);
}
#endif

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ISA value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          IUA value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ISB value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          IUB value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ISC value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          IUC value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ISD value) {
  return p.Print(value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          IUD value) {
  return p.Print(value);
}

#if USING_FPC == YES_0
template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          FPC value) {
  return p.Print(value);
}
#endif

#if USING_FPD == YES_0
template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          FPD value) {
  return p.Print(value);
}
#endif

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Binaryf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

namespace _ {

/* Prints a summary of the type-value tuple with word-sized Data Type. */
template<typename Printer>
Printer& TPrint(Printer& p, ATypeValue value) {
  return TPrintATypeValue<Printer>(p, DTB(value.Type()), value.Value(),
                                   value.MSB());
}

template<typename Printer>
Printer& TPrint(Printer& p, ATypePtr value) {
  return TPrintATypeValue<Printer>(p, DTB(value.type), IUW(value.value));
}

/* Prints an ASCII Type to the Printer. */
template<typename Printer>
Printer& TPrint(Printer& p, AErrorf value) {
  return TPrintAError<Printer>(p, value.error);
}

/* Prints an ASCII Type to the Printer. */
template<typename Printer>
Printer& TPrint(Printer& p, ATypef value) {
  return TPrintAType<Printer>(p, value.type);
}

// Prints a type and the binary with bit labels.
template<typename Printer>
Printer& TPrintATypeDebug(Printer& p, DTB type) {
  return p << "\nAType:" << ATypef(type) << "\n      0d" << type << "\n      0x"
           << Hexf(type) << "\n      0b" << Binaryf(type)
           << "\n        MD^MT ^SWVT^POD^";
}

}  //< namespace _

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Centerf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Rightf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Linef value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Hexf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Charsf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Sizef value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS> p,
                                          ::_::ATypef value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::ATypePtr value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::ATypeValue value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

template<typename CHS, typename CHT, typename IS>
inline ::_::TSPrinter<CHS, CHT, IS>& operator<<(::_::TSPrinter<CHS, CHT, IS>& p,
                                          ::_::Headingf value) {
  return ::_::TPrint<::_::TSPrinter<CHS, CHT, IS>>(p, value);
}

#endif  //< #if SEAM >= CRABS_UNIPRINTER
#endif  //< #ifndef CRABS_UNIPRINTER_HPP
