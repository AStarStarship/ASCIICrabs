// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#ifndef CRABS_STRINGF_DECL
#define CRABS_STRINGF_DECL
#include "AType.h"
namespace _ {

/* @defgroup Stringf
@brief String formatting utilities for the Uniprinter.
*/

// Returns true if the item is printable.
BOL IsPrintable(CHA item);
BOL IsPrintable(CHB item);
BOL IsPrintable(CHC item);

// Returns true if the item is not printable or is whitespace.
BOL IsntPrintable(CHA item);
BOL IsntPrintable(CHB item);
BOL IsntPrintable(CHC item);

BOL IsWhitespace(CHC item);

BOL IsDigit(CHC c);

BOL IsLowerCase(CHC c);

BOL IsUpperCase(CHC c);

/* @defgroup StringC
@brief In-house C-string primitives (contiguous, safe-access).
@details Consumers must NOT #include <cstring>/<cstdlib>; they use these `_::`
functions. Thin wrappers over the C runtime, ISC/ISD/IUD/CHA-typed, kept here
so the stdlib boundary stays inside ASCIICrabs. (Memory copy/fill/move use the
Array module's ArrayCopy/ArrayFill/ArrayShiftUp/ArrayShiftDown, not this file.)
@see ./Stringf.hxx
*/

/* C-string (NUL-terminated, CHA-typed). */
ISC    StrLen(const CHA* s);
ISC    StrCmp(const CHA* a, const CHA* b);
ISC    StrToInt(const CHA* s);   /* like atoi, ISC-typed */
ISD    StrToISD(const CHA* s, CHA** end);  /* like strtoll, ISD (signed 64-bit) */
IUD    StrToIUD(const CHA* s, CHA** end);  /* like strtoul, IUD (unsigned 64-bit) */

/* C-string search (pointer-returning; returns NILP if not found). */
const CHA* StrStr(const CHA* haystack, const CHA* needle);  /* like strstr */
const CHA* StrChr(const CHA* s, CHA value);                /* like strchr */

/* Gets the header to print for PrintChars(const void*, const void*). */
const CHA* STAPrintCharsHeader();

/* Gets the header to print for PrintChars(const void*, const void*). */
const CHA* STAPrintCharsBorder();

/* Gets the header to print for PrintHex(const void*, const void*). */
const CHA* STAPrintHexHeader();

/* Gets the header to print for PrintHex(const void*, const void*). */
const CHA* STAPrintHexBorder();

/* Converts a IUA into a two-IUA hex representation.
@return Returns -1 if c is not a hex IUA. */
ISC HexToByte(IUB hex);

/* Attempts to scan a character from the given string.
@return  Nil upon failure or a pointer to the end of the UTF-8 CHA upon
success. */
const CHA* SScan(const CHA* start, CHA& item);
CHA* SScan(CHA* start, CHA& item);

const CHA* SScan(const CHA* start, CHC& item);
CHA* SScan(CHA* start, CHC& item);


const CHA* SScan(const CHA* start, ISA& item);
CHA* SScan(CHA* start, ISA& item);
const CHA* SScan(const CHA* start, IUA& item);
CHA* SScan(CHA* start, IUA& item);
const CHA* SScan(const CHA* start, ISB& item);
CHA* SScan(CHA* start, ISB& item);
const CHA* SScan(const CHA* start, IUB& item);
CHA* SScan(CHA* start, IUB& item);
const CHA* SScan(const CHA* start, ISC& item);
CHA* SScan(CHA* start, ISC& item);
const CHA* SScan(const CHA* start, IUC& item);
CHA* SScan(CHA* start, IUC& item);
const CHA* SScan(const CHA* start, ISD& item);
CHA* SScan(CHA* start, ISD& item);
const CHA* SScan(const CHA* start, IUD& item);
CHA* SScan(CHA* start, IUD& item);

const CHB* SScan(const CHB* start, CHC& item);
CHB* SScan(CHB* start, CHC& item);
const CHB* SScan(const CHB* start, ISA& item);
CHB* SScan(CHB* start, ISA& item);
const CHB* SScan(const CHB* start, IUA& item);
CHB* SScan(CHB* start, IUA& item);
const CHB* SScan(const CHB* start, ISB& item);
CHB* SScan(CHB* start, ISB& item);
const CHB* SScan(const CHB* start, IUB& item);
CHB* SScan(CHB* start, IUB& item);
const CHB* SScan(const CHB* start, ISC& item);
CHB* SScan(CHB* start, ISC& item);
const CHB* SScan(const CHB* start, IUC& item);
CHB* SScan(CHB* start, IUC& item);
const CHB* SScan(const CHB* start, ISD& item);
CHB* SScan(CHB* start, ISD& item);
const CHB* SScan(const CHB* start, IUD& item);
CHB* SScan(CHB* start, IUD& item);


const CHC* SScan(const CHC* start, CHC& item);
CHC* SScan(CHC* start, CHC& item);
const CHC* SScan(const CHC* start, ISA& item);
CHC* SScan(CHC* start, ISA& item);
const CHC* SScan(const CHC* start, IUA& item);
CHC* SScan(CHC* start, IUA& item);
const CHC* SScan(const CHC* start, ISB& item);
CHC* SScan(CHC* start, ISB& item);
const CHC* SScan(const CHC* start, IUB& item);
CHC* SScan(CHC* start, IUB& item);
const CHC* SScan(const CHC* start, ISC& item);
CHC* SScan(CHC* start, ISC& item);
const CHC* SScan(const CHC* start, IUC& item);
CHC* SScan(CHC* start, IUC& item);
const CHC* SScan(const CHC* start, ISD& item);
CHC* SScan(CHC* start, ISD& item);
const CHC* SScan(const CHC* start, IUD& item);
CHC* SScan(CHC* start, IUD& item);

/* Attempts to scan the previous character from the given ASCII string.
@return NILP upon invalid Unicode character or invalid start. */
const CHA* SScanBack(const CHA* start, CHA& item);
const CHA* SScanBack(const CHA* start, CHC& item);
const CHB* SScanBack(const CHB* start, CHC& item);
const CHC* SScanBack(const CHC* start, CHC& item);

/* Prints a item to the string.
@return  Nil upon failure or a pointer to the nil-term CHS upon success.
@param start The beginning of the string socket.
@param stop  The last character in the string boofer.
@param item  The character to print. */
CHA* SPrint(CHA* start, CHA* stop, IUA item);
CHA* SPrint(CHA* start, ISW bytes, IUA item);
CHA* SPrint(CHA* start, CHA* stop, ISA item);
CHA* SPrint(CHA* start, ISW bytes, ISA item);
CHA* SPrint(CHA* start, CHA* stop, IUB item);
CHA* SPrint(CHA* start, ISW bytes, IUB item);
CHA* SPrint(CHA* start, CHA* stop, ISB item);
CHA* SPrint(CHA* start, ISW bytes, ISB item);
CHA* SPrint(CHA* start, CHA* stop, IUC item);
CHA* SPrint(CHA* start, ISW bytes, IUC item);
CHA* SPrint(CHA* start, CHA* stop, ISC item);
CHA* SPrint(CHA* start, ISW bytes, ISC item);
CHA* SPrint(CHA* start, CHA* stop, IUD item);
CHA* SPrint(CHA* start, ISW bytes, IUD item);
CHA* SPrint(CHA* start, CHA* stop, ISD item);
CHA* SPrint(CHA* start, ISW bytes, ISD item);
CHA* SPrint(CHA* start, CHA* stop, FPC item);
CHA* SPrint(CHA* start, ISW bytes, FPC item);
CHA* SPrint(CHA* start, CHA* stop, FPD item);
CHA* SPrint(CHA* start, ISW bytes, FPD item);
CHA* SPrint(CHA* start, CHA* stop, CHA item);
CHA* SPrint(CHA* start, ISW bytes, CHA item);
CHA* SPrint(CHA* start, CHA* stop, CHC item);
CHA* SPrint(CHA* start, ISW bytes, CHC item);
CHA* SPrint(CHA* start, CHA* stop, const CHA* item);
CHA* SPrint(CHA* start, ISW bytes, const CHA* item);
CHA* SPrint(CHA* start, CHA* stop, const CHB* item);
CHA* SPrint(CHA* start, ISW bytes, const CHB* item);
CHA* SPrint(CHA* start, CHA* stop, const CHC* item);
CHA* SPrint(CHA* start, ISW bytes, const CHC* item);

#if USING_STB == YES_0
/* Prints a character to the string.
@return  Nil upon failure or a pointer to the nil-term CHS upon success. */
CHB* SPrint(CHB* start, CHB* stop, IUA item);
CHB* SPrint(CHB* start, ISW bytes, IUA item);
CHB* SPrint(CHB* start, CHB* stop, ISA item);
CHB* SPrint(CHB* start, ISW bytes, ISA item);
CHB* SPrint(CHB* start, CHB* stop, IUB item);
CHB* SPrint(CHB* start, ISW bytes, IUB item);
CHB* SPrint(CHB* start, CHB* stop, ISB item);
CHB* SPrint(CHB* start, ISW bytes, ISB item);
CHB* SPrint(CHB* start, CHB* stop, IUC item);
CHB* SPrint(CHB* start, ISW bytes, IUC item);
CHB* SPrint(CHB* start, CHB* stop, ISC item);
CHB* SPrint(CHB* start, ISW bytes, ISC item);
CHB* SPrint(CHB* start, CHB* stop, IUD item);
CHB* SPrint(CHB* start, ISW bytes, IUD item);
CHB* SPrint(CHB* start, CHB* stop, ISD item);
CHB* SPrint(CHB* start, ISW bytes, ISD item);
CHB* SPrint(CHB* start, CHB* stop, FPC item);
CHB* SPrint(CHB* start, ISW bytes, FPC item);
CHB* SPrint(CHB* start, CHB* stop, FPD item);
CHB* SPrint(CHB* start, ISW bytes, FPD item);
CHB* SPrint(CHB* start, CHB* stop, CHB item);
CHB* SPrint(CHB* start, ISW bytes, CHB item);
CHB* SPrint(CHB* start, CHB* stop, CHB item);
CHB* SPrint(CHB* start, ISW bytes, CHB item);
CHB* SPrint(CHB* start, CHB* stop, CHC item);
CHB* SPrint(CHB* start, ISW bytes, CHC item);
CHB* SPrint(CHB* start, CHB* stop, const CHA* item);
CHB* SPrint(CHB* start, ISW bytes, const CHA* item);
CHB* SPrint(CHB* start, CHB* stop, const CHB* item);
CHB* SPrint(CHB* start, ISW bytes, const CHB* item);
CHB* SPrint(CHB* start, CHB* stop, const CHC* item);
CHB* SPrint(CHB* start, ISW bytes, const CHC* item);
#endif

#if USING_STC == YES_0
/* Prints a character or string to the string.
@return  Nil upon failure or a pointer to the nil-term CHS upon success. */
CHC* SPrint(CHC* start, CHC* stop, IUA item);
CHC* SPrint(CHC* start, ISW bytes, IUA item);
CHC* SPrint(CHC* start, CHC* stop, ISA item);
CHC* SPrint(CHC* start, ISW bytes, ISA item);
CHC* SPrint(CHC* start, CHC* stop, IUB item);
CHC* SPrint(CHC* start, ISW bytes, IUB item);
CHC* SPrint(CHC* start, CHC* stop, ISB item);
CHC* SPrint(CHC* start, ISW bytes, ISB item);
CHC* SPrint(CHC* start, CHC* stop, IUC item);
CHC* SPrint(CHC* start, ISW bytes, IUC item);
CHC* SPrint(CHC* start, CHC* stop, ISC item);
CHC* SPrint(CHC* start, ISW bytes, ISC item);
CHC* SPrint(CHC* start, CHC* stop, IUD item);
CHC* SPrint(CHC* start, ISW bytes, IUD item);
CHC* SPrint(CHC* start, CHC* stop, ISD item);
CHC* SPrint(CHC* start, ISW bytes, ISD item);
CHC* SPrint(CHC* start, CHC* stop, FPC item);
CHC* SPrint(CHC* start, ISW bytes, FPC item);
CHC* SPrint(CHC* start, CHC* stop, FPD item);
CHC* SPrint(CHC* start, ISW bytes, FPD item);
CHC* SPrint(CHC* start, CHC* stop, CHA item);
CHC* SPrint(CHC* start, ISW bytes, CHA item);
CHC* SPrint(CHC* start, CHC* stop, CHC item);
CHC* SPrint(CHC* start, ISW bytes, CHC item);
CHC* SPrint(CHC* start, CHC* stop, const CHA* item);
CHC* SPrint(CHC* start, ISW bytes, const CHA* item);
CHC* SPrint(CHC* start, CHC* stop, const CHB* item);
CHC* SPrint(CHC* start, ISW bytes, const CHB* item);
CHC* SPrint(CHC* start, CHC* stop, const CHC* item);
CHC* SPrint(CHC* start, ISW bytes, const CHC* item);
#endif

/* Utility class for printing numbers. */
struct Valuef {
  ISW count;         //< Width of the item in bytes or columns.
  ATypeValue value;  //< The type and value.

  /* Constructs a NIL item. */
  Valuef();

  /* Constructs a Valuef from the given item. */
  Valuef(const void* item, ISW count = 0);
  Valuef(void* item, ISW count = 0);
#if USING_STA == YES_0
  Valuef(CHA item, ISW count = 0);
  Valuef(const CHA* item, ISW count = 0);
#endif
#if USING_STB == YES_0
  Valuef(CHB item, ISW count = 0);
  Valuef(const CHB* item, ISW count = 0);
#endif
#if USING_STC == YES_0
  Valuef(CHC item, ISW count = 0);
  Valuef(const CHC* item, ISW count = 0);
#endif
  Valuef(ISA item, ISW count = 0);
  Valuef(IUA item, ISW count = 0);
  Valuef(ISB item, ISW count = 0);
  Valuef(IUB item, ISW count = 0);
  Valuef(ISC item, ISW count = 0);
  Valuef(IUC item, ISW count = 0);
  Valuef(ISD item, ISW count = 0);
  Valuef(IUD item, ISW count = 0);
#if USING_FPC == YES_0
  Valuef(FPC item, ISW count = 0);
#endif
#if USING_FPD == YES_0
  Valuef(FPD item, ISW count = 0);
#endif

  /* Gets the value.Type(). */
  ISW Type();

  /* Gets the count. */
  ISW Count();

  /* Gets the pointer to either the item.Ptr () or the pointer item stores. */
  void* Value();

  /* Gets the value::word_ as the specified type. */
  void* ToPtr();
  CHA* ToSTA();
  CHB* ToSTB();
  CHC* ToSTC();
  IUW ToWord();
};

/* Utility class for printing a POD type in hex.
@todo: Why is this storing the value as a negative?
*/
struct Hexf {
  Valuef element;  //< Stores the ASCII Type, it's value, and the byte count.

  Hexf(const void* item, ISW size);
  /* Stores the given item to the boofer and stores the size. */
  Hexf(const void* item);
  Hexf(ISA item);
  Hexf(IUA item);
  Hexf(CHA item);
  Hexf(ISB item);
  Hexf(IUB item);
  Hexf(CHB item);
  Hexf(ISC item);
  Hexf(IUC item);
  Hexf(CHC item);
  Hexf(ISD item);
  Hexf(IUD item);
#if USING_FPC == YES_0
  Hexf(FPC item);
#endif
#if USING_FPD == YES_0
  Hexf(FPD item);
#endif
};

/* Utility class for printing a POD type in binary. */
struct Binaryf {
  Valuef element;  //< Stores the ASCII Type, it's value, and the byte count.

  /* Stores the given item to the boofer and store the size. */
  Binaryf(const void* item);
  Binaryf(const void* item, ISW size);
  Binaryf(ISA item);
  Binaryf(IUA item);
  Binaryf(ISB item);
  Binaryf(IUB item);
  Binaryf(ISC item);
  Binaryf(IUC item);
  Binaryf(ISD item);
  Binaryf(IUD item);
#if USING_FPC == YES_0
  Binaryf(FPC item);
#endif
#if USING_FPD == YES_0
  Binaryf(FPD item);
#endif
};

/* Utility class for printing blocks of characters to the console with
operator<<. */
struct Charsf {
  Valuef element;  //< The type, value and CHA, CHB, or CHC count.

  /* Constructs the value from the delta between start and stop. */
  Charsf(const void* start, const void* stop);
  Charsf(const void* start, ISW count);
#if USING_STB == YES_0
  Charsf(const CHB* start, const CHB* stop);
  Charsf(const CHB* start, ISW count);
#endif
#if USING_STC == YES_0
  Charsf(const CHC* start, const CHC* stop);
  Charsf(const CHC* start, ISW count);
#endif

  // Returns the element.Type();
  DTW Type();
};

/* Utility class for formatting text with operator<<, most commonly right and
center aligned.
@warning Stringf cannot be const because we need to write to it on the stack 
frame in chained operator overloads. */
class Stringf {
 public:
  enum {
    // Max length of the boofer in ALU words.
    BooferWordCount = (sizeof(void*) == 2) ? 5
                    : (sizeof(void*) == 4) ? 6 
                    : (sizeof(void*) == 8) ? 5 : 1,
    // Max length of a string in characters.
    LengthMax = BooferWordCount * sizeof(void*) - 1,
  };

 private:
  const void* string_;  //< Pointer to a string or the boofer_.
  ISW count_;           //< The character count.
  DTW type_;            //< The ASCII string Type, STA, STB, or STC.
  IUW boofer_[BooferWordCount];  //< String boofer for the token.

 public:
  /* Default constructor sets the count but doesn't write a nil-term char
  to the boofer. */
  Stringf();

  /* Sets the String_ to the given pointer and stores the count. */
  Stringf(const CHA* item);
#if USING_STB == YES_0
  Stringf(const CHB* item);
#endif
#if USING_STC == YES_0
  Stringf(const CHC* item);
#endif

  /* Prints the item to the boofer_, stores the count, and sets the String_ to
  the boofer_. */
  Stringf(CHA item);
  Stringf(CHB item);
  Stringf(CHC item);
  Stringf(ISC item);
  Stringf(IUC item);
  Stringf(ISD item);
  Stringf(IUD item);
#if USING_FPC == YES_0
  Stringf(FPC item);
#endif
#if USING_FPD == YES_0
  Stringf(FPD item);
#endif

  /* Sets the String_ to the given pointer and stores the count. */
  Stringf(const CHA* item, ISW count);
#if USING_STB == YES_0
  Stringf(const CHB* item, ISW count);
#endif
#if USING_STC == YES_0
  Stringf(const CHC* item, ISW count);
#endif

  /* Prints the item to the boofer_, stores the count, and sets the String_ to
  the boofer_. */
  Stringf(CHA item, ISW count);
  Stringf(CHB item, ISW count);
  Stringf(CHC item, ISW count);
  Stringf(ISC item, ISW count);
  Stringf(IUC item, ISW count);
  Stringf(ISD item, ISW count);
  Stringf(IUD item, ISW count);
#if USING_FPC == YES_0
  Stringf(FPC item, ISW count);
#endif
#if USING_FPD == YES_0
  Stringf(FPD item, ISW count);
#endif
  //Stringf(ATypef item, ISW count = AConsoleWidth);
  //Stringf(TypeValue item, ISW count = AConsoleWidth);

  IUW Word();

  /* Gets a void pointer to the value_. */
  void* Value();

  /* Gets the pointer contained in value_[0]. */
  void* Ptr();

  /* Gets the String_. */
  const CHA* STA();
  const CHB* STB();
  const CHC* STC();

  /* Gets the type_. */
  ISW Type();

  /* Gets the count_. */
  ISW Count();

  /* Saves the pointer to the String_. */
  void Print(const CHA* item);
#if USING_STB == YES_0
  void Print(const CHB* item);
#endif
#if USING_STC == YES_0
  void Print(const CHC* item);
#endif

  /* Prints the given item to the boofer_. */
  void Print(CHA item);
//#if USING_STB == YES_0
//  void Print(CHB item);
//#endif
#if USING_STC == YES_0
  void Print(CHC item);
#endif
  void Print(ISC item);
  void Print(IUC item);
  void Print(ISD item);
  void Print(IUD item);
#if USING_FPC == YES_0
  void Print(FPC item);
#endif
#if USING_FPD == YES_0
  void Print(FPD item);
#endif

  /* Prints a timestamp to the boofer_. */
  //void PrintTMC(TMC item);
  //void PrintTME(TMC item, IUC subsecond_tick);
  //void PrintTMD(TMD item);

  //void Print(TypeValue item);

  /* Stores the item to the first word of the boofer and the negative of the
  count. */
  void Hex(CHA item, ISW count = 80);
#if USING_STB == YES_0
  void Hex(CHB item, ISW count = 80);
#endif
#if USING_STC == YES_0
  void Hex(CHC item, ISW count = 80);
#endif
  void Hex(ISA item, ISW count = 80);
  void Hex(IUA item, ISW count = 80);
  void Hex(ISB item, ISW count = 80);
  void Hex(IUB item, ISW count = 80);
  void Hex(ISC item, ISW count = 80);
  void Hex(IUC item, ISW count = 80);
  void Hex(ISD item, ISW count = 80);
  void Hex(IUD item, ISW count = 80);
#if USING_FPC == YES_0
  void Hex(FPC item, ISW count = 80);
#endif
#if USING_FPD == YES_0
  void Hex(FPD item, ISW count = 80);
#endif
};

/* Utility class for printing hex with operator<<. */
struct Centerf {
  Stringf element;  //< Pointer to a pointer to utf.

  /* Does nothing. */
  Centerf();

  /* Prints the item to the value. */
  Centerf(CHA item, ISW count = AConsoleWidth);
  Centerf(const CHA* start, ISW count = AConsoleWidth);
#if USING_STB == YES_0
  Centerf(CHB item, ISW count = AConsoleWidth);
  Centerf(const CHB* item, ISW count = AConsoleWidth);
#endif
#if USING_STC == YES_0
  Centerf(CHC item, ISW count = AConsoleWidth);
  Centerf(const CHC* item, ISW count = AConsoleWidth);
#endif
  Centerf(ISC item, ISW count = AConsoleWidth);

  Centerf(IUC item, ISW count = AConsoleWidth);
  Centerf(ISD item, ISW count = AConsoleWidth);
  Centerf(IUD item, ISW count = AConsoleWidth);

#if USING_FPC == YES_0
  Centerf(FPC item, ISW count = AConsoleWidth);
#endif
#if USING_FPD == YES_0
  Centerf(FPD item, ISW count = AConsoleWidth);
#endif

  /* Stores the item to the first word of the boofer and the negative of the
  count. */
  Centerf& Hex(CHA item, ISW count = 80);
#if USING_STB == YES_0
  Centerf& Hex(CHB item, ISW count = 80);
#endif
#if USING_STC == YES_0
  Centerf& Hex(CHC item, ISW count = 80);
#endif
  Centerf& Hex(ISA item, ISW count = 80);
  Centerf& Hex(IUA item, ISW count = 80);
  Centerf& Hex(ISB item, ISW count = 80);
  Centerf& Hex(IUB item, ISW count = 80);
  Centerf& Hex(ISC item, ISW count = 80);
  Centerf& Hex(IUC item, ISW count = 80);
  Centerf& Hex(ISD item, ISW count = 80);
  Centerf& Hex(IUD item, ISW count = 80);
#if USING_FPC == YES_0
  Centerf& Hex(FPC item, ISW count = 80);
#endif
#if USING_FPD == YES_0
  Centerf& Hex(FPD item, ISW count = 80);
#endif

  // Returns the element.Type();
  DTW Type();
};

/* Utility class for printing hex with operator<<. */
struct Rightf {
  Stringf element;  //< Pointer to a pointer to utf.

  /* Does nothing. */
  Rightf();

  /* Prints the item to the value. */
  Rightf(CHA item, ISW count = AConsoleWidth);
  Rightf(const CHA* item, ISW count = AConsoleWidth);
#if USING_STB == YES_0
  Rightf(CHB item, ISW count = AConsoleWidth);
  Rightf(const CHB* item, ISW count = AConsoleWidth);
#endif
#if USING_STC == YES_0
  Rightf(CHC item, ISW count = AConsoleWidth);
  Rightf(const CHC* item, ISW count = AConsoleWidth);
#endif
  Rightf(ISC item, ISW count = AConsoleWidth);
  Rightf(IUC item, ISW count = AConsoleWidth);
  Rightf(ISD item, ISW count = AConsoleWidth);
  Rightf(IUD item, ISW count = AConsoleWidth);

#if USING_FPC == YES_0
  Rightf(FPC item, ISW count = AConsoleWidth);
#endif
#if USING_FPD == YES_0
  Rightf(FPD item, ISW count = AConsoleWidth);
#endif

  /* Stores the item to the first word of the boofer and the negative of the
  count. */
  Rightf& Hex(CHA item, ISW count = 80);
#if USING_STB == YES_0
  Rightf& Hex(CHB item, ISW count = 80);
#endif
#if USING_STC == YES_0
  Rightf& Hex(CHC item, ISW count = 80);
#endif
  Rightf& Hex(ISA item, ISW count = 80);
  Rightf& Hex(IUA item, ISW count = 80);
  Rightf& Hex(ISB item, ISW count = 80);
  Rightf& Hex(IUB item, ISW count = 80);
  Rightf& Hex(ISC item, ISW count = 80);
  Rightf& Hex(IUC item, ISW count = 80);
  Rightf& Hex(ISD item, ISW count = 80);
  Rightf& Hex(IUD item, ISW count = 80);
#if USING_FPC == YES_0
  Rightf& Hex(FPC item, ISW count = 80);
#endif
#if USING_FPD == YES_0
  Rightf& Hex(FPD item, ISW count = 80);
#endif

  // Returns the element.Type();
  DTW Type();
};

/* Utility class for printing a horizontal line with operator<<. */
struct Linef {
  Valuef element;     //< Pointer to a pointer to utf.
  CHA format[5];      //< Used to create a string in Linef(CHA,ISW).

  /* Constructors a horizontal line of the character. */
  Linef(CHA item, ISW count = AConsoleWidth);

  /* Constructors a horizontal line of the given string. */
  Linef(const CHA* start = NILP, ISW count = AConsoleWidth);

  /* Constructors a horizontal line of the given string. */
  Linef(const CHB* start = NILP, ISW count = AConsoleWidth);

  /* Constructors a horizontal line of the given string. */
  Linef(const CHC* start = NILP, ISW count = AConsoleWidth);

  // Returns the element.Type();
  DTW Type();
};

/* Utility class to print a string repeating a given number of times. */
struct Repeatf {
  Stringf element;
  ISW count;

  Repeatf(const CHA* element, ISW count);
  Repeatf(const CHB* element, ISW count);
  Repeatf(const CHC* element, ISW count);
};

/* Utility class for printing a Heading with formatting with operator<<. */
struct Headingf {
  Stringf element;
  const CHA* style,     //< Printf-style string.
           * caption2,  //< Line under the style.
           * caption3;  //< Line under the caption2.

  /* Saves the parameters to the corresponding data members. */
  Headingf(const CHA* caption);

  /* Saves the parameters to the corresponding data members. */
  Headingf(const CHA* caption, const CHA* caption2);

  /* Saves the parameters to the corresponding data members. */
  Headingf(const CHA* caption, const CHA* caption2,
           const CHA* caption3 = NILP, const CHA* style = NILP,
           ISW count = AConsoleWidth);

  // Returns the element.Type()
  DTW Type();
};

/* Utility class for indenting text with operator<<. */
struct Indentf {
  ISW indent_count;  //< Number of spaces to indent.

  /* Constructs the value from the delta between start and stop. */
  Indentf(ISW indent_count);
};

/* Utility class for indicating the size or count of an item. */
struct Sizef {
  ISW size;  //< The size of the item.

  Sizef(ISA size);
  Sizef(ISB size);
  Sizef(ISC size);
  Sizef(ISD size);
};

/* Utility class for printing an ASCII type-value one-liner. */
struct ATypef {
  DTW type,   //< The item type.
      count;  //< The number of chars to print.

  ATypef(DTB type, Sizef count = { -1 });
  ATypef(DTC type, Sizef count = { -1 });
  ATypef(DTD type, Sizef count = { -1 });
  //ATypef(DTE type, Sizef count = { -1 });
  ATypef(DTB pod, DTB vt, Sizef count = { -1 });
  ATypef(DTB pod, DTB vt, DTB sw, Sizef count = { -1 });
  ATypef(DTB pod, DTB vt, DTB sw, DTB map, Sizef count = { -1 });
  ATypef(DTB pod, DTB vt, DTB sw, DTB map, DTB mod, Sizef count = { -1 });

  // Center aligns this to the given character count.
  Centerf Center(ISW count);

  // Right aligns this to the given character count.
  Rightf Right(ISW count);
};

/* Utility class for printing an ASCII type-value one-liner. */
struct AValuef {
  ATypeValue value;  //< The item type and value.
};

// Utility class for printing an ASCII Error Code.
struct AErrorf {
  ISW error;            //< Error code.
  const CHA* message;   //< Error message.

  AErrorf(ISW error, const CHA* message = NILP);
};
}  //< namespace _
#endif
// @} Stringf