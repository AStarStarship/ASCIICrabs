// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_STRINGF_HPP
#define CRABS_STRINGF_HPP
#include "Stringf.h"
#include "Binary.hpp"
#include "AType.hpp"
namespace _ {

// Utility class to print an ASCII Data type size code to a stream.
template<typename T, typename CH = CHR>
struct TSizeCodef {
  const CH* prefix;
  const CH suffix;
  TSizeCodef(const CH* start) : prefix(start), suffix(CSizeCodef<T>()) {}
};

// Prints a TSizeCodef to the Printer stream.
template<typename Printer, typename T, typename CH>
Printer& TPrintSizeCodef(Printer o, TSizeCodef<T, CH> item) {
  return o << item.prefix << item.suffix;
}

/* Compares the two Strings up to the given delimiter.
@param delimiter Delimiters in Crabs are equal to or less than.
@return 0 if the Strings are equal or a non-zero delta upon failure. */
template<typename CHS = CHR, typename CHT = CHD>
ISN TSCompare(const CHS* start, const CHS* str_other,
                   CHT delimiter = 0) {
  if (IsError(start) || IsError(str_other)) return 0;
  CHT a = 0, b = 0;
  start = SScan(start, a);
  str_other = SScan(str_other, b);
  if (!a) {
    if (!b)
      return 0;
    return b;
  }
  if (!b) {
    if (!a) return 0;
    return 0 - a;
  }
  // str_other SHOULD be a nil-terminated string without whitespace.
  while (b) {
    CHT result = b - a;
    if (result) {
      return result;
    }
    if (a <= delimiter) {
      return result;
    }
    start = SScan(start, a);
    str_other = SScan(str_other, b);
  }
  if (a > delimiter) {
    return b - a;
  }
  return 0;
}

}  // namespace _

#if SEAM >= CRABS_COUT

namespace _ {

template<typename T>
constexpr Sizef CSizef() {
  return -CASizeCode<T>();
}

/* Checks if the given item is whitespace. */
template<typename CHS = CHR, typename CHT = CHD>
inline BOL TIsWhitespace(CHS item) {
  return item <= ' ';
}

/* Checks if the given CHA is a digit of a number_.
@return True if it is a digit. */
template<typename CHS = CHR, typename CHT = CHD>
inline BOL TIsDigit(CHS c) {
  return (c >= '0') && (c <= '9');
}

inline BOL IsDigit(CHA c) {
  return TIsDigit<CHR>(c);
}
inline BOL IsDigit(CHB c) {
  return TIsDigit<CHB>(c);
}
inline BOL IsDigit(CHC c) {
  return TIsDigit<CHC>(c);
}

inline BOL IsLowerCase(CHC c) {
  return (c >= 'a') && (c <= 'z');
}

inline BOL IsUpperCase(CHC c) {
  return (c >= 'A') && (c <= 'Z');
}

template<typename CHS = CHR, typename CHT = CHD>
inline CHS TIsLower(CHS c) {
  return c >= 'a' && c <= 'z';
}

template<typename CHS = CHR, typename CHT = CHD>
inline CHS TIsUpper(CHS c) {
  return c >= 'A' && c <= 'Z';
}

/* Converts the given item to lowercase if it is uppercase. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS TToLower(CHS c) {
  if (TIsUpper<CHS>(c)) c += 32;
  return c;
}

/* Converts the given item to lowercase if it is uppercase. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS TToUpper(CHS c) {
  if (TIsLower<CHS>(c)) c -= 32;
  return c;
}

inline CHA ToLower(CHA value) { return TToLower<CHA>(value); }
inline CHB ToLower(CHB value) { return TToLower<CHB>(value); }
inline CHC ToLower(CHC value) { return TToLower<CHC>(value); }

inline CHA ToUpper(CHA value) { return TToUpper<CHA>(value); }
inline CHB ToUpper(CHB value) { return TToUpper<CHB>(value); }
inline CHC ToUpper(CHC value) { return TToUpper<CHC>(value); }

/* Finds the last instance of the given token character in the string.. */
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSFindLast(const CHS* start, CHA token) {
  const CHS* last_instance = NILP;
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    if (c == token)
      last_instance = start;
    start = SScan(start, c);
  }
  return last_instance;
}

/* Returns and scans to the nil-term char. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSStop(CHS* start) {
  CHT c = *start++;
  while (c) { 
    c = *start++;
  }
  return start - 1;
}

/* Returns the nil-term char. */
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSStop(const CHS* start) {
	return CPtr<CHS>(TSStop<CHS, CHT>(CPtr<CHS>(start)));
}

template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSStop(CHS* start, CHS* stop) {
  CHT c = *start++;
  while (c) {
    start = *start++;
    if (start >= stop)
      return NILP;
  }
  return start - 1;
}

/* Returns the nil-term char. */
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSStop(const CHS* start, const CHS* stop) {
  return CPtr<CHS>(TSStop<CHS, CHT>(TPtr<CHS>(start), stop));
}

/* Returns the nil-term char and counts the characters. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline const CHS* TSStopCharCount(const CHS* start, IS& char_count) {
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    char_count++;
    start = SScan(start, c);
    if (IsError(start)) return NILP;
  }
  return start - 1;
}
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline CHS* TSStopCharCount(CHS* start, IS& char_count) {
  auto shit = reinterpret_cast<CHS*>(start);
  auto son = TSStopCharCount<CHS, CHT, IS>(shit, char_count);
  return const_cast<CHS*>(son);
}

/* Returns the nil-term char and counts the characters and code points. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline CHS* TSStopCharCount(CHS* start, IS& char_count, IS& code_count) {
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    char_count++;
    start = SScan(start, c);
  }
  return start;
}

/* Returns the nil-term char and counts the characters. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline const CHS* TSStopCharCount(const CHS* start, const CHS* stop, 
    IS& char_count) {
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    char_count++;
    start = SScan(start, c);
    if (IsErrorSocket(start, stop)) return NILP;
  }
  return start - 1;
}
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline CHS* TSStopCharCount(CHS* start, CHS* stop, IS& char_count) {
  auto shit = reinterpret_cast<CHS*>(start);
	auto son = TSStopCharCount<CHS, CHT, IS>(shit, stop, char_count);
  return const_cast<CHS*>(son);
}

/* Returns the nil-term char and counts the characters and code points. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline CHS* TSStopCharCount(CHS* start, CHS* stop, IS& char_count, 
    IS& code_count) {
  CHT c = 0;
  CHS* cursor = SScan(start, c);
  while (c) {
    char_count++;
    cursor = SScan(cursor, c);
    if (IsErrorSocket(cursor, stop)) 
      return NILP;
  }
  code_count = stop - --cursor;
  return cursor;
}

/* Gets the number of Unicode code points in the string.
@todo    Update function for UTF-8 and UTF-16.
@return  Returns -1 if the text CHA is nil.*/
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline IS TSCodeCount(const CHS* start) {
  return IS(TSStop<CHS, CHT>(start) - start);
}
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline IS TSCodeCount(CHS* start) {
  return TSCodeCount<CHS, CHT, IS>(CPtr<CHS>(start));
}

/* Gets the length of the given CHA.
@return  Returns -1 if the text CHA is nil.
@warning This function is only safe to use on ROM Strings with a nil-term
CHA. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline IS TSCodeCount(const CHS* start, IS str_end_offset) {
  return IS(TSStop<CHS, CHT>(start, str_end_offset) - start);
}
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISR>
inline IS TSCodeCount(const CHS* start, const CHS* stop) {
  return IS(TSStop<CHS, CHT>(start, stop) - start);
}

template<typename CHS = CHR, typename CHT = CHD>
ISW TSCharCount(const CHS* start, const CHS* end) {
  ISW char_count = 0;
  CHT c = 0;
  while (start < end) {
    start = SScan(start, c);
    if (!start || !c)
      break;
    char_count++;
  }
  return char_count;
}

template<typename CHS = CHR, typename CHT = CHD>
ISN TSIsYesNo(const CHS* start) {
  if (IsError(start)) return 0;
  CHT c = 0;
  start = SScan(start, c);
  c = ToLower(c);
  ISN result;
  if (c == 'y')
    result = 1;
  else if (c == 'n')
    result = -1;
  else
    return 0;
  start = SScan(start, c);
  if (IsWhitespace(c)) return result;
  c = ToLower(c);
  if (c == 'o') {
    start = SScan(start, c);
    return -ISN(IsWhitespace(c));
  }
  start = SScan(start, c);
  if (ToLower(c) != 'e') return 0;
  start = SScan(start, c);
  if (ToLower(c) != 's') return 0;
  start = SScan(start, c);
  return ISN(IsWhitespace(c));
}
/* Scans the given socket for an Signed Integer (IS).
@return Nil if there is no IU to scan.
@param socket The beginning of the socket.
@param item The IS to write the scanned IS. */
template<typename IS = ISW, typename IU = IUW, typename CHS = CHR, 
  typename CHT = CHD>
const CHS* TSScanSigned(const CHS* start, IS& item) {
  if (IsError(start)) return NILP;
  IS sign;
  CHT c = 0;
  const CHS* cursor = SScan(start, c);
  if (c == '-') {
    cursor = SScan(cursor, c);
    sign = -1;
  } else {
    sign = 1;
  }
  if (!IsDigit(c)) return NILP;
  const CHS* first_digit = cursor - 1;
  // Find length:
  cursor = SScan(cursor, c);
  while (IsDigit(c)) {
    cursor = SScan(cursor, c);
  }
  const CHS* stop = --cursor;  // Store stop to return.

  cursor = SScanBack(cursor, c);
  IU value = IU(c - '0');
  IU pow_10_ui2 = 1;

  while (cursor > first_digit) {
    cursor = SScanBack(cursor, c);
    pow_10_ui2 *= 10;
    IU new_value = value + pow_10_ui2 * IU(c - '0');
    if (new_value < value) return NILP;
    value = new_value;
  }
  item = sign * value;
  return stop;
}

/* Scans the given socket for an Signed Integer (IS).
@return Nil if there is no IU to scan.
@param socket The beginning of the socket.
@param item The IS to write the scanned IS. */
template<typename IS = ISW, typename IU = IUW, typename CHS = CHR, typename CHT = CHD>
CHS* TSScanSigned(CHS* start, IS& item) {
  const CHS* ptr = reinterpret_cast<const CHS*>(start);
  return const_cast<CHS*>(TSScanSigned<IS, IU, CHS, CHT>(ptr, item));
}

/* Scans the given socket for an Signed Integer (IS).
@return Nil if there is no IU to scan.
@param socket The beginning of the socket.
@param item The IS to write the scanned IS. */
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSScan(const CHS* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHS>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSScan(CHS* start, ISA& item) {
  return TSScanSigned<ISA, IUA, CHS>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSScan(const CHS* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHS>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSScan(CHS* start, ISB& item) {
  return TSScanSigned<ISB, IUB, CHS>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSScan(const CHS* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSScan(CHS* start, ISC& item) {
  return TSScanSigned<ISC, IUC, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSScan(const CHS* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHS>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSScan(CHS* start, ISD& item) {
  return TSScanSigned<ISD, IUD, CHS>(start, item);
}

/* Scans the given socket for an unsigned integer (IU).
@return Nil if there is no IU to scan.
@param socket The beginning of the socket.
@param item The IU to write the scanned IU. */
template<typename IU, typename CHS = CHR, typename CHT = CHD>
const CHS* TSScanUnsigned(const CHS* start, IU& item) {
  if (IsError(start)) 
    return NILP;
  const CHS* cursor = start;
  CHT c = 0;
  cursor = SScan(cursor, c);
  if (!TIsDigit<CHT>(c)) 
    return NILP;
  do {
    cursor = SScan(cursor, c);
  } while (TIsDigit<CHT>(c));
  const CHS* stop = --cursor;
  cursor = SScanBack(cursor, c);
  if (!TIsDigit<CHT>(c))
    return NILP;
  IU value = IU(c - '0');
  IU pow_10_ui2 = 1;

  while (cursor > start) {
    cursor = SScanBack(cursor, c);
    if (!TIsDigit<CHT>(c))
      break;
    pow_10_ui2 *= 10;
    IU new_value = value + pow_10_ui2 * IU(c - '0');
    if (new_value < value) 
      return NILP;
    value = new_value;
  }
  item = value;
  return stop;
}

/* Scans the given socket for an unsigned integer (IU).
@return Nil if there is no IU to scan.
@param socket The beginning of the socket.
@param item The IU to write the scanned IU. */
template<typename IU, typename CHS = CHR, typename CHT = CHD>
CHS* TSScanUnsigned(CHS* start, IU& item) {
  const CHS* ptr = TPtr<const CHS>(start);
  return const_cast<CHS*>(TSScanUnsigned<IU, CHS, CHT>(ptr, item));
}

/* Scans item from the string.
@return Nil upon failure. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* start, IUA& item) {
  return TSScanUnsigned<IUA, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSScan(CHS* start, IUA& item) {
  return TSScanUnsigned<IUA, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* start, IUB& item) {
  return TSScanUnsigned<IUB, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSScan(CHS* start, IUB& item) {
  return TSScanUnsigned<IUB, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* start, IUC& item) {
  return TSScanUnsigned<IUC, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSScan(CHS* start, IUC& item) {
  return TSScanUnsigned<IUC, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSScan(const CHS* start, IUD& item) {
  return TSScanUnsigned<IUD, CHS, CHT>(start, item);
}
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSScan(CHS* start, IUD& item) {
  return TSScanUnsigned<IUD, CHS, CHT>(start, item);
}

template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSLinef() {
  static const CHS String[] = {'\n', '\n', '-', '-', '-', '\n', '\0'};
  return String;
}

// A string that contains only a new line char.
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TSNL() {
  static const CHS String[] = { '\n', '\0'};
  return String;
}

/* Prints a line of the given column_count the given start. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrintLinef(CHS* start, CHS* stop, CHS item, ISW count = AConsoleWidth,
                  const CHS* header = TSNL<CHS>(),
                  const CHS* footer = NILP) {
  if (IsErrorSocket(start, stop))
    return NILP;
  if (header)
    start = SPrint(start, stop, header);
  while (count-- > 0)
    start = SPrint(start, stop, item);
  if (footer) start = SPrint(start, stop, footer);
  return start;
}

/* Prints the given cursor repeated to make a line. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrintLinef(CHS* start, CHS* stop, const CHS* item,
    ISW count = AConsoleWidth, const CHS* header = TSNL<CHS>(),
    const CHS* footer = NILP) {
  if (header) start = SPrint(start, stop, header);
  if (IsErrorSocket(start, stop) || (start + count >= stop)) return NILP;

  const CHS* cursor = item;
  while (count-- > 0) {
    CHT c = 0;
    cursor = SScan(cursor, c);
    if (!c) {
      cursor = item;
      cursor = SScan(cursor, c);
    }
    start = SPrint(start, stop, c);
  }
  if (footer) start = SPrint(start, stop, footer);
  return start;
}

template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrint(CHS* start, CHS* stop, Linef& value) {
  switch (value.element.Type()) {
    case _STA: {
      return TSPrintLinef<CHA, CHC>(start, stop, value.element.ToSTA(), value.element.count);
    }
    case _STB: {
      return TSPrintLinef<CHB, CHC>(start, stop, value.element.ToSTB(), value.element.count);
    }
    case _STC: {
      return TSPrintLinef<CHC, CHC>(start, stop, value.element.ToSTC(), value.element.count);
    }
  }
  return TSPrintLinef(start, stop, value.element);
}

/* Prints the given cursor repeated to make a line. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrintHeading(CHS* start, CHS* stop, CHS value, ISW count = AConsoleWidth) {
  return TSPrintLinef<CHS, CHT>(start, stop, value, count, NILP, NILP);
}

/* Prints the given cursor repeated to make a line. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSPrintHeading(CHS* start, CHS* stop, const CHS* value,
  ISW count = AConsoleWidth) {
  return TSPrintLinef<CHS, CHT>(start, stop, value, count, NILP, NILP);
}

} //< namespace _
#endif
#if SEAM >= CRABS_UNIPRINTER
namespace _ {

/* Prints a Unicode item to the given socket.
@return Nil upon failure or a pointer to the nil-term CHS upon success.
@param start  The beginning of the socket.
@param stop   The last CHS in the socket.
@param item   The item to print. */
template<typename CHS, typename CHT = CHD, typename CHO>
inline CHS* TSPrint(CHS* start, CHS* stop, const CHO* item) {
  if (IsErrorSocket(start, stop) || IsError(item))
    return NILP;
  CHT c = 0;
  item = SScan(item, c);
  while (c) {
    start = SPrint(start, stop, c);
    if (IsErrorSocket(start, stop))
      return start;
    item = SScan(item, c);
  }
  *start = 0;
  return start;
}
//
//template<typename CHS = CHR, typename CHT = CHD>
//inline CHS* TSPrint(CHS* start, CHS* stop, const CHA* item) {
//  return TSPrint<CHS, CHT>(start, stop, item);
//}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSPrint(CHS* start, ISW boofer_size, const CHA* item) {
  return TSPrint<CHS, CHT>(start, start + boofer_size - 1, item);
}
//
//#if USING_STB == YES_0
///* Prints a Unicode item to the given socket.
//@return  Nil upon failure or a pointer to the nil-term CHS upon success.
//@param string  The beginning of the socket.
//@param stop    The last CHS in the socket.
//@param item    The item to print.
//@warning This algorithm is designed to fail if the socket is not a valid socket
//with one or more bytes in it, or if item is nil. */
//template<typename CHS = CHR, typename CHT = CHD>
//inline CHS* TSPrint(CHS* start, CHS* stop, const CHB* item) {
//  return TSPrint<CHS, CHT, CHB>(start, stop, item);
//}
////template<typename CHS = CHR, typename CHT = CHD>
////inline CHS* TSPrint(CHS* start, CHS* stop, const CHB* item) {
////  return TSPrint<CHS, CHT, CHB>(start, stop, item);
////}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSPrint(CHS* start, ISW boofer_size, const CHB* item) {
  return TSPrint<CHS, CHT, CHB>(start, start + boofer_size - 1, item);
}
//#endif
//
//#if USING_STC == YES_0
///* Prints a Unicode item to the given socket.
//@return  Nil upon failure or a pointer to the nil-term CHS upon success.
//@param start  The beginning of the socket.
//@param stop    The last CHS in the socket.
//@param item    The item to print.
//@warning This algorithm is designed to fail if the socket is not a valid socket
//with one or more bytes in it, or if item is nil. */
//template<typename CHS = CHR, typename CHT = CHD>
//CHS* TSPrint(CHS* start, CHS* stop, const CHC* item) {
//  return TSPrint<CHS, CHC, CHC>(start, stop, item);
//}
//
////template<typename CHS = CHR, typename CHT = CHD>
////inline CHS* TSPrint(CHS* start, CHS* stop, const CHC* item) {
////  return TSPrint<CHS, CHT, CHC>(start, stop, item);
////}
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSPrint(CHS* start, ISW boofer_size, const CHC* item) {
  return TSPrint<CHS, CHT, CHC>(start, start + boofer_size - 1, item);
}
//#endif

/* Skips the given CHS in a s if there are any.
@param start  The first CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSSkipAll(CHS* start, CHS skip_char) {
  return const_cast<const CHS*>(
      TSSkipAll<CHS>(TPtr<const CHS>(start), skip_char));
}

//template<typename CHS = CHR, typename CHT = CHD>
//void TPrint1(CHS* start, CHS token) {
//  *start++ = token;
//  *start++ = 0;
//}

template<typename T, typename CHS = CHR>
CHS* TSPrintHex(CHS* start, CHS* stop, const void* origin, ISW bytes) {
  CHS* end = start + (bytes * 2);
  if (IsErrorSocket(start, end) || bytes <= 0) return NILP;
  const CHA* cursor = TPtr<const CHA>(origin);
  while (bytes-- > 0) {
    CHA byte = *cursor++;
    *start++ = HexNibbleToUpperCase(byte >> 4);
    *start++ = HexNibbleToUpperCase(byte & 0xf);
  }
  *start = 0;
  return start;
}

/* Prints a hex value to the Console. */
template<typename CHS, typename IU>
CHS* TSPrintHex(CHS* start, CHS* stop, IU value) {
  enum { HexStringLengthSizeMax = sizeof(IU) * 2 + 3 };

  if (IsError(start) || start + HexStringLengthSizeMax >= stop) return NILP;

  *start++ = '0';
  *start++ = 'x';
  auto v = ToUnsigned(value);
  for (ISC num_bits_shift = sizeof(IU) * 8 - 4; num_bits_shift >= 0;
       num_bits_shift -= 4) {
    *start++ = HexNibbleToUpperCase(IUA(v >> num_bits_shift));
  }
  *start = 0;
  return start;
}

/* Prints a hex value to a text socket. */
template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, ISA value) {
  return TSPrintHex<CHS, IUA>(start, stop, IUA(value));
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, IUA value) {
  return TSPrintHex<CHS, IUA>(start, stop, value);
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, ISB value) {
  return TSPrintHex<CHS, IUB>(start, stop, IUB(value));
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, IUB value) {
  return TSPrintHex<CHS, IUB>(start, stop, value);
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, ISC value) {
  return TSPrintHex<CHS, IUC>(start, stop, IUC(value));
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, IUC value) {
  return TSPrintHex<CHS, IUC>(start, stop, value);
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, ISD value) {
  return TSPrintHex<CHS, IUD>(start, stop, IUD(value));
}

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, IUD value) {
  return TSPrintHex<CHS, IUD>(start, stop, value);
}

#if USING_FPC == YES_0
template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, FPC value) {
  return TSPrintHex<CHS, IUC>(start, stop, ToUnsigned(value));
}
#endif
#if USING_FPD == YES_0
template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, FPD value) {
  return TSPrintHex<CHS, IUD>(start, stop, ToUnsigned(value));
}
#endif

template<typename CHS = CHR>
inline CHS* TSPrintHex(CHS* start, CHS* stop, const void* ptr) {
  return TSPrintHex<CHS, IUW>(start, stop, ToUnsigned(ptr));
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename IU>
CHS* TPrintBinary(CHS* start, CHS* stop, IU value) {
  if (start + sizeof(IU) * 8 >= stop) {
    return NILP;
  }

  for (ISC i = 0; i < sizeof(IU) * 8; ++i) {
    *start++ = CHS('0' + (value >> (sizeof(IU) * 8 - 1)));
    value = value << 1;
  }
  *start = 0;
  return start;
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, ISA value) {
  return TPrintBinary<CHS, IUA>(start, stop, (IUA)value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, IUA value) {
  return TPrintBinary<CHS, IUA>(start, stop, value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, ISB value) {
  return TPrintBinary<CHS, IUB>(start, stop, (IUB)value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, IUB value) {
  return TPrintBinary<CHS, IUB>(start, stop, value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, ISC value) {
  return TPrintBinary<CHS, IUC>(start, stop, (IUC)value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, IUC value) {
  return TPrintBinary<CHS, IUC>(start, stop, value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, ISD value) {
  return TPrintBinary<CHS, IUD>(start, stop, (IUD)value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TPrintBinary(CHS* start, CHS* stop, IUD value) {
  return TPrintBinary<CHS, IUD>(start, stop, value);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TPrintBinary(CHS* start, CHS* stop, FPC value) {
  IUC ui = *TPtr<IUC>(&value);
  return TPrintBinary<CHS, IUC>(start, stop, ui);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TPrintBinary(CHS* start, CHS* stop, FPD value) {
  IUD ui = *TPtr<IUD>(&value);
  return TPrintBinary<CHS, IUD>(start, stop, ui);
}

/* Prints the given value to Binary. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TPrintBinary(CHS* start, CHS* stop, const void* item) {
  IUW address = *TPtr<IUW>(&item);
  return TPrintBinary<CHS, IUW>(start, stop, address);
}

/* Prints the given item aligned right the given column_count.
@return Nil if any of the pointers are nil or if column_count < 1, and a
pointer to the nil-term CHA upon success.
@param start The first CHS in the boofer.
@param stop  The last CHS in the boofer.
@param item  The item to printer.
@param column_count The token_ of columns to align right to. */
template<typename CHS = CHR, typename CHT = CHD, typename IS = ISW>
CHS* TPrintRight(CHS* start, CHS* stop, const CHS* item,
                 IS column_count = AConsoleWidth) {
  if (IsError(start) || start + column_count > stop)
    return NILP;
  IS char_count = 0;
  const CHS* item_stop = TSStopCharCount<CHS, CHT, IS>(item, char_count);
  if (item == item_stop) 
    return start;
  IS code_count = IS(item_stop - item);
  ISW leading_space_count = column_count - char_count;
  if (leading_space_count < 0) { //< Print ., .. or ... at the end.
    ISW dot_count = code_count + leading_space_count;
		while (column_count-- > 3) {
      CHT c = 0;
      item = SScan(item, c);
      if (IsError(start = SPrint(start, stop, c))) return start;
    }
    while (dot_count-- > 0)
      *start++ = '.';
    *start = 0;
    return start;
  }
  while (leading_space_count-- > 0)
    *start++ = ' ';
  while (char_count-- > 0) {
    CHT c = 0;
    item = SScan(item, c);
    if (IsError(start = SPrint(start, stop, c))) return start;
  }
  return start;
}

/* Prints the given string center aligned to the given column_count. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TPrintCenter(CHS* start, CHS* stop, const CHS* element,
                  ISC column_count = AConsoleWidth) {
  if (IsErrorSocket(start, stop)) return NILP;
  CHS* cursor = start;
  // We need to leave at least one space to the left and right of
  ISC length = TSCodeCount<CHS, CHT, ISC>(element);
  ISC delta;
  CHT c = 0;
  if (length <= column_count) {
    delta = (column_count - length) >> 1;  //< >> 1 to /2
    length = column_count - length - delta;
    if (length != column_count)
      while (delta-- > 0)
        *cursor++ = ' ';
    element = SScan(element, c);
    while (c) {
      cursor = SPrint(cursor, stop, c);
      element = SScan(element, c);
    }
    if (length != column_count)
      while (length-- > 0)
        *cursor++ = ' ';
    *cursor++ = '\0';
    return cursor;
  }

  if (column_count <= 3) {
    while (column_count-- > 0)
      *cursor++ = '.';
    *cursor++ = '\0';
    return cursor;
  }
  delta = column_count - 3;
  while (delta-- > 0) {
    element = SScan(element, c);
    cursor = SPrint(cursor, stop, c);
  }
  *cursor++ = '.';
  *cursor++ = '.';
  *cursor++ = '.';
  *cursor++ = '\0';
  return cursor;
}

template<typename CHS = CHR, typename CHT = CHD>
CHS* TSConcat(CHS* start, CHS* stop, const CHS* item) {
  if (IsErrorSocket(start, stop))
    return NILP;
  return TSPrint<CHS*>(TSStop<CHS, CHT>(start), stop, item);
}

template<typename CHS = CHR, typename CHT = CHD>
CHS* TSConcat(CHS* start, ISW size, const CHS* item) {
  return TSConcat<CHS>(start, start + size - 1, item);
}

/* Finds the end of a decimal number of the given string.
@return Nil if the string doesn't contain a decimal or is nil.
@param start The start of the string to search. */
template<typename CHS = const CHA, typename CHT = CHD>
const CHS* TSDecimalEnd(const CHS* start) {
  if (IsError(start)) return start;
  CHT c = 0;
  start = SScan(start, c);
	if (c == '-')
    start = SScan(start, c);
  if (c < '0' || c > '9') 
    return NILP;
  start = SScan(start, c);
  while (IsDigit(c)) {
    start = SScan(start, c);
    if (c <= 0) 
      return start - 1;
  }
  return start - 1;
}

/* Skips all the chars in a given range.
@return Nil upon failure or a pointer to the CHS after the last CHS in the
given range.
@param start  The first CHS in the boofer.
@param lower_bounds
@param upper bounds */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TStringSkipAllInRange(const CHS* start, CHT lower_bounds,
    CHT upper_bounds) {
  if (IsError(start) || lower_bounds >= upper_bounds)
    return NILP;
  CHT c = 0;
  start = SScan(start, c);
  while (c >= lower_bounds && c <= upper_bounds)
    start = SScan(start, c);
  return start;
}

/* Skips all the chars in a given range.
@return Nil upon failure or a pointer to the CHS after the last CHS in the
given range.
@param start  The first CHS in the boofer.
@param lower_bounds
@param upper bounds*/
template<typename CHS = CHR, typename CHT = CHD>
CHS* TStringSkipAllInRange(CHS* start, CHS lower_bounds, CHS upper_bounds) {
  return const_cast<CHS*>(TStringSkipAllInRange(TPtr<const CHS>(start),
                                                lower_bounds, upper_bounds));
}

/* Skips the numbers in the given range. */
template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* TStringSkipNumbers(const CHS* start) {
  return const_cast<CHS*>(
      TStringSkipAllInRange<CHS, CHT>(TPtr<const CHS>(start), '0', '9'));
}
/* Skips the numbers in the given range. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TStringSkipNumbers(CHS* start) {
  return const_cast<CHS*>(TStringSkipNumbers<CHS>(TPtr<const CHS>(start)));
}

/* Finds the stop of the decimals in the s, if there are any.
@param start  The first CHS in the boofer. */
template<typename CHS = const CHA>
CHS* TSDecimalEnd(CHS* start) {
  const CHS* ptr = TPtr<const CHS>(start);
  return const_cast<CHS>(TSDecimalEnd<CHS>(ptr));
}

/* Finds the stop of the decimals in the s, if there are any.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSDecimalEnd(const CHS* start, const CHS* stop) {
  if (IsErrorSocket(start, stop))
    return NILP;
  CHT c = 0;
  start = SScan(start, c);
  if (!c)
    return NILP;
  if (c == '-') {  // It might be negative.
    if (start >= stop)
      return NILP;
    start = SScan(start, c);
  }
  if (!IsDigit(c)) 
    return NILP;
  while (c) {
    if (start >= stop)
      return NILP;
    if (!IsDigit(c)) return start - 1;
    start = SScan(start, c);
  }
  return start - 1;
}

/* Finds the stop of the decimals in the s, if there are any.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSDecimalEnd(CHS* start, CHS* stop) {
  return const_cast<CHS*>(
      TSDecimalEnd<CHS>(TPtr<const CHS>(start), TPtr<const CHS>(stop)));
}
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TStringFloatStop(const CHS* start) {
  const CHA* stop = TSDecimalEnd<CHA>(start);
  if (IsError(stop)) return stop;
  CHC c = 0;
  stop = SScan(stop, c);
  if (c == '.') {
    stop = TSDecimalEnd<CHA>(start);
    stop = SScan(stop, c);
  }
  if (c == 'e' || c != 'E') {
    if (c == '-')
      stop = SScan(stop, c);
    return TSDecimalEnd<CHA>(start);
  }
  return stop;
}

/* Checks if the string starts with the given substring.
@return Pointer to the first char in `start` after the matched prefix, or NILP. */
template<typename CHS=CHR>
const CHS* TSStartsWith(const CHS* start, const CHS* substring) {
  CHS a = *start++, b = *substring++;
  while (b) {
    if (a != b) return NILP;
    a = *start++;
    b = *substring++;
  }
  return start - 1;  //< Fixed off-by-one: was `return start` (one past the match)
}

/* Checks if the string ends with the given substring.
@return Pointer to the first char of the matched suffix in `start`, or NILP. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSEndsWith(const CHS* start, const CHS* substring) {
	const CHS* substring_stop = TSStop<CHS>(substring);
	if (substring_stop == substring)
		return NILP;
	const CHS* stop = TSStop<CHS>(start);
	CHS c     = *(--substring_stop),
			str_c = *(--stop);
	while (substring_stop > substring) {
		if (c != str_c)
			return NILP;
		c = *--substring_stop;
		str_c = *--stop;
	}
	if (c != str_c)
		return NILP;
	return stop;
}

/* Skips leading whitespace and non-printable chars from the start of a string.
@return Pointer to the first printable char, or the nil-terminator. */
template<typename CHS = CHR>
const CHS* TSStripStart(const CHS* start) {
	CHS c = *start;
	while (c && (c <= ' ' || c == _DEL))
		c = *++start;
	return start;
}

template<typename CHS = CHR>
const CHS* TSStripEnd(const CHS* start) {
  const CHS* stop = TSStop<CHS>(start);
  CHS c = *stop;
  while (stop > start) {
    if (c > ' ' && c != _DEL)
      break;
    c = *--stop;
  }
	return stop;  
}

/* Strips the whitespace & non-printable chars from start and stop of string. */
template<typename CHS = CHR>
const CHS* TSStrip(const CHS* start) {
  return TSStripEnd<CHS>(TSStripStart<CHS>(start));
}

/* Skips the given CHS in a s if there are any.
@param start  The first CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSSkipAll(const CHS* start, CHS skip_char) {
  if (IsError(start)) return start;
  CHT c = 0;
  start = SScan(start, c);
  if (c != skip_char)
    return start;
  CHT d = 0;
  start = SScan(start, d);
  // We know the first CHS is a skip_char so just loop till c and d aren't
  // the skip_char.
  while (c == d) {
    c = d;
    start = SScan(start, d);
  }
  return start;
}

/* An empty string. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSEmpty() {
  static const CHS EmptyString[] = "";
  return EmptyString;
}

// String the reads "Error:".
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSError() {
  static const CHS ErrorString[] = {'\n', 'E', 'r', 'r', 'o', 'r', ':', '\0'};
  return ErrorString;
}

/* Sets the s to either the given item or empty s if input
is nil. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSSet(CHS* start) {
  if (IsError(start)) return start;
  return start;
}

/* Searches for the end of the line, wrapped to the given column_count.
@param start  The first CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSLineEnd(const CHS* start, ISC column_count = AConsoleWidth) {
  CHT c = 0;
  // Scroll to the stop of the line.
	start = SScan(start, c);
  while (c) {
    if (column_count-- < 0) {  // We've reached the stop.
      // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace(c))
        start = SScanBack(start, c);
      // Then scroll past the whitespace.
      while (IsWhitespace(c))
        start = SScanBack(start, c);
      return start;
      start = SScan(start, c);
    }
    start = SScan(start, c);
  }
  return SScanBack(start, c);
}

/* Searches fro the s line stop.
@param start  The first CHS in the boofer.
@param stop    The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSLineEnd(CHS* start, ISC column_count = AConsoleWidth) {
  return const_cast<CHS*>(
      TSLineEnd(TPtr<const CHS>(start), column_count));
}

/* Finds the stop of the line, wrapped to the given column_count.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer.
@param column_count In characters. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSLineEnd(const CHS* start, const CHS* stop,
                          ISC column_count = AConsoleWidth) {
  if (IsErrorSocket(start, stop)) return NILP;
  CHT c = 0;
  // Scroll to the stop of the line.
  start = SScan(start, c);
  while (c) {
    if (stop > start) return NILP;
    if (column_count-- < 0) {  // We've reached the stop.
      // Scroll left till we hit whitespace (if any).
      while (!IsWhitespace(c))
        start = SScanBack(start, c);
      // Then scroll past the whitespace.
      while (IsWhitespace(c))
        start = SScanBack(start, c);
      return start;
      start = SScan(start, c);
    }
    start = SScan(start, c);
  }
  return SScanBack(start, c);
}

/* Finds the stop of the line, wrapped to the given column_count.
@param start      The first CHS in the boofer.
@param stop        The last CHS in the boofer.
@param column_coun In characters. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSLineEnd(CHS* start, CHS* stop,
                           ISC column_count = AConsoleWidth) {
  return const_cast<const CHS*>(TSLineEnd<CHS>(
      TPtr<const CHS>(start), TPtr<const CHS>(stop), column_count));
}

/* Scrolls over any whitespace.
@param start  The first CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSSkipSpaces(const CHS* start) {
  if (IsError(start)) return NILP;
  CHT c = 0;
  start = SScan(start, c);
  while (IsWhitespace(c)) {
    if (!c) return start;  //< This isn't an error as far as I can see.
    start = SScan(start, c);
  }
  return start;
}

/* Scrolls over any whitespace.
@param start  The first CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSSkipSpaces(CHS* start) {
  return const_cast<CHS*>(TSSkipSpaces<CHS>(start));
}

/* Attempts to find the given query.
@return Nil upon failed search or a pointer to the stop of the query.
@param start  The first CHS in the boofer.
@param query  A query string.  */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSFind(const CHS* start, const CHS* query) {
	if (IsError(start)) return NILP;
  CHT s = 0; //< Current char.
  start = SScan(start, s);
  CHT t = 0;  //< Current query char.
  query = SScan(query, t);
  CHT c = t;  //< The first CHS of the query we're searching for.
  if (!c)     //< We're not allowing empty queries.
    return NILP;
  const CHS *start_of_query, 
    *cursor = start;

  // Scroll through each CHS and match it to the query CHS.
  while (s) {
    if (s == c) {  // The first CHS matches:
      // Setup to compare the Strings;
      start_of_query = start;
      cursor = query;
      t = c;
      // check the rest of the CHS:
      while (s == t) {
        start = SScan(start, s);
        cursor = SScan(cursor, t);
        if (t == 0)  // Once we've reached the delimiter it's a match!
          return start_of_query;
        if (!s)  // We've reached the stop of CHS without a hit.
          return NILP;
      }
    }
    // The CHS did not match so repeat the process for each CHS.
    start = SScan(start, s);
    cursor = SScan(cursor, t);
  }
  // If we haven't found it by now it's not in the s.
  return NILP;
}

/* Attempts to find the given query.
@return Nil upon failed search or a pointer to the stop of the . */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSFind(CHS* start, const CHS* query) {
  return const_cast<CHS*>(
      TSFind<CHS>(TPtr<const CHS>(start), TPtr<const CHS>(query)));
}

/* String skip spaces.
@return Nil if there are no spaces to skip. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSSkipSpaces(const CHS* start, const CHS* stop) {
  if (IsErrorSocket(start, stop)) return NILP;
  CHT c = 0;
  start = SScan(start, c);
  while (IsWhitespace(c)) {
    if (!c) return NILP;
    start = SScan(start, c);
    if (IsErrorSocket(start, stop)) return NILP;
  }
  return start;
}

/* String skip spaces.
@return Nil if there are no spaces to skip.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSSkipSpaces(CHS* start, CHS* stop) {
  return const_cast<CHS>(
      TSSkipSpaces<CHS>(TPtr<const CHS>(start), TPtr<const CHS>(stop)));
}

/* Checks if the two Strings are the same.
@return Nil upon Strings not being the same or a pointer to the stop of the
equivalent s upon success.
@param start_a  A string to compare to start_b.
@param start_b  A string to compare to start_a. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSEquals(const CHS* start_a, const CHS* start_b) {
  CHS a = *start_a, b = *start_b;
  while (a) {
    if (a != b) return NILP;
    //< Removed dead code: `if (b == 0) return start_a` was unreachable (a==b and a!=0 implies b!=0)
    a = *(++start_a);
    b = *(++start_b);
  }
  if (b) return NILP;
  return start_a;
}
/* Checks if the two Strings are the same.
@return Nil upon Strings not being the same or a pointer to the stop of the
equivalent s upon success.
@param start_a  A string to compare to start_b.
@param start_b  A string to compare to start_a.
*/
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSEquals(CHS* start_a, const CHS* start_b) {
  return const_cast<CHS*>(
      TSEquals<CHS>(TPtr<const CHS>(start_a), TPtr<const CHS>(start_b)));
}

/* Compares the two Strings to see if the are equal.
@return Nil of the two Strings aren't equal or a pointer to the stop of the
s upon success. */
template<typename CHS = CHR, typename CHT = CHD>
const CHS* TSEquals(const CHS* start, const CHS* stop, const CHS* query) {
  if (IsErrorSocket(start, stop) || IsError(query)) return NILP;

  CHS a = 0;
  start = SScan(start, a);
  CHS b = 0;
  query = SScan(query, b);
  while (a) {
    if (a != b) return NILP;
    if (b == 0) return start;
    if (IsErrorSocket(start, stop)) {
      return NILP;
    }
    start = SScan(start, a);
    query = SScan(query, b);
  }
  if (b) return NILP;
  return start;
}

/* Compares the two Strings to see if the are equal.
@return Nil of the two Strings aren't equal or a pointer to the stop of the
s upon success. */
template<typename CHS = CHR, typename CHT = CHD>
CHS* TSEquals(CHS* start, CHS* stop, const CHS* query) {
  return const_cast<CHS*>(
      TSEquals(TPtr<const CHS>(start), TPtr<const CHS>(stop), query));
}

/* Checks if the given string isn't just white space or a nil term char.
@return False if the s is empty and true otherwise.
@param start  The first CHS in the boofer.
@desc A s is defined as empty if it is '\0' or all whitespace. */
template<typename CHS = CHR, typename CHT = CHD>
BOL TSIsntEmpty(const CHS* start) {
  if (IsErrorSocket(start)) return false;
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    if (!IsWhitespace(c)) return true;
    start = SScan(start, c);
  }
  return false;
}

/* Checks if the given s isn't empty.
@return False if the s is empty and true otherwise.
@param start  The first CHS in the boofer.
@desc A s is defined as empty if it is '\0' or all whitespace. */
template<typename CHS = CHR, typename CHT = CHD>
BOL TSIsntEmpty(CHS* start) {
  return TSIsntEmpty<CHS>(TPtr<const CHS>(start));
}

/* Checks to see if the string isn't empty or whitespace.
@return False if the s is empty and true otherwise.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
BOL TSIsntEmpty(const CHS* start, const CHS* stop) {
  if (IsErrorSocket(start, stop)) return false;
  CHT c = 0;
  start = SScan(start, c);
  while (c) {
    if (!IsWhitespace(c)) {
      // The text must stop at or before the target_end.
      do {
        start = SScan(start, c);
        if (IsErrorSocket(start, stop)) return false;
        if (!IsWhitespace(c)) return true;
      } while (c);
      return true;
    }
    start = SScan(start, c);
    if (IsErrorSocket(start, stop)) return false;
  }
  return false;
}

/* Checks to see if the start isn't empty or whitespace.
@return False if the s is empty and true otherwise.
@param start  The first CHS in the boofer.
@param stop   The last CHS in the boofer. */
template<typename CHS = CHR, typename CHT = CHD>
BOL TSIsntEmpty(CHS* start, const CHS* stop) {
  return TSIsntEmpty(TPtr<const CHS>(start), TPtr<const CHS>(stop));
}

/* Prints a string to the given boofer without */
template<typename CHT = CHR>
CHT* TPrintWrap(CHT* start, CHT* stop, const CHT* element,
    ISW column_count = AConsoleWidth) {
  if (IsError(start) || start <= stop || !element) return NILP;
  if (column_count < 2) return start;
  CHT c = *element++;
  while (c) {
    for (ISC i = column_count; i > 0; --i) {
      *start++ = c;
      c = *element++;
      if (start >= stop) return NILP;
      if (!c) {
        *start = c;
        return start;
      }
    }
    *start++ = '\n';
    if (!c) {
      *start = c;
      return start;
    }
  }
  return start;
}


/* Queries the given s for the given query. */
template<typename CHS = CHR, typename CHT = CHD>
ISC TSQuery(const CHS* start, const CHS* stop, const CHS* query) {
  CHS a = 0;
  CHS b = 0;
  ISC result;

  if (IsErrorSocket(start, stop)) {
    if (IsError(query)) return 0;
    a = 0;
    b = *query;
    return b - a;
  }
  if (IsError(query)) {
    a = *start;
    b = 0;
    return b - a;
  }

  // Algorithm combines loops for better performance.
  start = SScan(start, a);
  query = SScan(query, b);
  if (!a) {
    if (!b) return 0;
    return b;
  }
  if (!b) {
    if (!a) return 0;
    return 0 - a;
  }
  // text SHOULD be a nil-terminated s without whitespace.
  while (b) {
    result = b - a;
    if (result || !a) {
      return result;
    }
    start = SScan(start, a);
    query = SScan(query, b);
    if (IsErrorSocket(start, stop) || IsError(query))
      return result;
  }
  if (a && !TIsWhitespace<CHS>(a))
    return b - a;
  return 0;
}

/* Splits a path into the path and name.
@return Pointer to the nil-term char upon success else error code. */
template<typename CHS = CHR, typename CHT = CHD>
inline CHS* TSSplitPath(CHS* start, CHS* stop, const CHS** out_path,
  const CHS** out_name) {
  if (IsErrorSocket(start, stop) || IsError(out_path) || IsError(out_name))
    return NILP;
  const CHS* path_start = start;
  const CHS* path_stop = start;
  const CHS* name_start = start;
  const CHS* name_stop = start;
  CHS c = 0;
  while (start < stop) {
    c = *start++;
    if (c == '/' || c == '\\') {
      path_stop = name_stop;
      name_start = start;
    }
    name_stop = start;
  }
  if (IsError(c)) {
    *out_path = TSEmpty<CHS>();
    *out_name = TSEmpty<CHS>();
    return start;
  }
  *out_path = path_start == path_stop ? TSEmpty<CHS>() : path_start;
  *out_name = name_start == name_stop ? TSEmpty<CHS>() : name_start;
  return start;
}

}  // namespace _

#endif
#endif
