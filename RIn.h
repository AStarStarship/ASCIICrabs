// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsRInH
#define _CrabsRInH
#include "ROut.h"
#if _Seam >= _CrabsCore
namespace _ {

/* Checks if the given string is yes or no.
@return -1 if no, 1 if yes, and 0 if neither. */
ISN IsYesNo(const CHA* string);
ISN IsYesNo(const CHB* string);
ISN IsYesNo(const CHC* string);

/* Utility class for scanning types with operator overloads.
@code
ISC a;
FL4 b;
RIn cin;
ROut("\n\nEnter a value 0-1,000,000").Star() << cin.SScan(a)
@endcode
*/
class RIn {
  CHA buffer_[24];  //< Buffer for scanning numbers and short Strings.

 public:
  enum {
    StateSuccess = 0,     //< State 0: Completed scanning.
    StateBaseSign,        //< State 1: Scanning base sign.
    StateBaseValue,       //< State 2: Scanning base value.
    StateDotOrExponent,   //< State 3: Scanning '.', 'e', or 'E'.
    StateFractionalPart,  //< State 4: Scanning the fraction part.
    StateExponentSign,    //< State 4: Scanning exponent sign.
    StateExponentValue,   //< State 5: Scanning exponent value.
  };

  enum {
    BufferSizeMin = 2,  //< This min size of a string buffer.
  };

  /* Pauses until any key is pressed. */
  RIn();

  RIn(CHA& result);
  /* RIns a string with a larger buffer than the */
  RIn(CHA* result, ISW buffer_size);
  RIn(CHB& result);
  RIn(CHB* result, ISW buffer_size);
  RIn(CHC& result);
  RIn(CHC* result, ISW buffer_size);

  /* Scans the given item to the RIn stream. */
  RIn(ISA& result);
  RIn(IUA& result);
  RIn(ISB& result);
  RIn(IUB& result);
  RIn(ISC& result);
  RIn(IUC& result);
  RIn(ISD& result);
  RIn(IUD& result);
  RIn(FPC& result);
  RIn(FPD& result);

  /* Reads the character from the RIn stream. */
  static BOL SScan(CHA& result);

  /* Reads the string with the given buffer_size from the RIn stream. */
  static BOL SScan(CHA* result, ISW buffer_size);

  /* Reads the character from the RIn stream. */
  static BOL SScan(CHB& result);

  /* Reads the string with the given buffer_size from the RIn stream. */
  static BOL SScan(CHB* result, ISW buffer_size);
  /* Reads the character from the RIn stream. */
  static BOL SScan(CHC& result);

  /* Reads the string with the given buffer_size from the RIn stream. */
  static BOL SScan(CHC* result, ISW buffer_size);

  /* Reads the value from the RIn stream. */
  static BOL SScan(ISA& result);
  static BOL SScan(IUA& result);
  static BOL SScan(ISB& result);
  static BOL SScan(IUB& result);
  static BOL SScan(ISC& result);
  static BOL SScan(IUC& result);
  static BOL SScan(ISD& result);
  static BOL SScan(IUD& result);
  static BOL SScan(FPC& result);
  static BOL SScan(FPD& result);

  /* Reads the current key depressed on the keyboard.
  @return A negative number_ (typically 1) if no keys are pressed. */
  static ISN GetKey();

  /* Spin waits for the user to press a keyboard key.
  @return The key the user pressed. */
  static ISN ScanKey();
};

/* Prints a formatted string to the console and then pauses for the user to
press any key to continue. */
struct Pausef {
  Pausef(const CHA* message = "\nPress any key to continue...");
};

/* Gets the names of the KeyId. */
inline const CHA* KeyIdName();

/* Gets the array of KeyId offsets. */
inline const ISB* KeyIdOffset();

/* Gets the array of KeyId order. */
inline const IUA* KeyIdOrder();

/* Gets the name from the VK code. */
inline const CHA* KeyIdNameFromCode(ISC code);

/* Gets the VK code from the name of the key. */
inline ISC KeyIdCodeFromName(const CHA* name);

/* Convert a key id to an HID code.
@return Pointer to the a LUT.   */
const IUA* KeyboardNativeToHID();

/* Convert an HID code to a key id.
@return Pointer to the a LUT. */
const IUA* KeyboardHIDToNative();

typedef ISC VKCode;

enum {
  cVKA = 4,
  cVKB = 5,
  cVKC = 6,
  cVKD = 7,
  cVKE = 8,
  cVKF = 9,
  cVKG = 10,
  cVKH = 11,
  cVKI = 12,
  cVKJ = 13,
  cVKK = 14,
  cVKL = 15,
  cVKM = 16,
  cVKN = 17,
  cVKO = 18,
  cVKP = 19,
  cVKQ = 20,
  cVKR = 21,
  cVKS = 22,
  cVKT = 23,
  cVKU = 24,
  cVKV = 25,
  cVKW = 26,
  cVKX = 27,
  cVKY = 28,
  cVKZ = 29,
  cVK1 = 30,
  cVK2 = 31,
  cVK3 = 32,
  cVK4 = 33,
  cVK5 = 34,
  cVK6 = 35,
  cVK7 = 36,
  cVK8 = 37,
  cVK9 = 38,
  cVK0 = 39,
  cVKEnter = 40,
  cVKEscape = 41,
  cVKDelete = 42,
  cVKTab = 43,
  cVKSpace = 44,
  cVKMinus = 45,
  cVKEquals = 46,
  cVKLeftBracket = 47,
  cVKRightBracket = 48,
  cVKBackslash = 49,
  cVKSemicolon = 51,
  cVKQuote = 52,
  cVKGrave = 53,
  cVKComma = 54,
  cVKPeriod = 55,
  cVKSlash = 56,
  cVKCapsLock = 57,
  cVKF1 = 58,
  cVKF2 = 59,
  cVKF3 = 60,
  cVKF4 = 61,
  cVKF5 = 62,
  cVKF6 = 63,
  cVKF7 = 64,
  cVKF8 = 65,
  cVKF9 = 66,
  cVKF10 = 67,
  cVKF11 = 68,
  cVKF12 = 69,
  cVKPrintScreen = 70,
  cVKScrollLock = 71,
  cVKPause = 72,
  cVKInsert = 73,
  cVKHome = 74,
  cVKPageUp = 75,
  cVKDeleteForward = 76,
  cVKEnd = 77,
  cVKPageDown = 78,
  cVKRight = 79,
  cVKLeft = 80,
  cVKDown = 81,
  cVKUp = 82,
  cKPNumLock = 83,
  cKPDivide = 84,
  cKPMultiply = 85,
  cKPSubtract = 86,
  cKPAdd = 87,
  cKPEnter = 88,
  cKP1 = 89,
  cKP2 = 90,
  cKP3 = 91,
  cKP4 = 92,
  cKP5 = 93,
  cKP6 = 94,
  cKP7 = 95,
  cKP8 = 96,
  cKP9 = 97,
  cKP0 = 98,
  cKPPoint = 99,
  cVKNonUSBackslash = 100,
  cKPEquals = 103,
  cVKF13 = 104,
  cVKF14 = 105,
  cVKF15 = 106,
  cVKF16 = 107,
  cVKF17 = 108,
  cVKF18 = 109,
  cVKF19 = 110,
  cVKF20 = 111,
  cVKF21 = 112,
  cVKF22 = 113,
  cVKF23 = 114,
  cVKF24 = 115,
  cVKHelp = 117,
  cVKMenu = 118,
  cVKLeftControl = 224,
  cVKLeftShift = 225,
  cVKLeftAlt = 226,
  cVKLeftGUI = 227,
  cVKRightControl = 228,
  cVKRightShift = 229,
  cVKRightAlt = 230,
  cVKRightGUI = 231
};

/* Gets the next key pressed in the keyboard event cue. */
inline ISC RInKey();

/* Gets the pressed state of the vk_code. */
inline BOL RInState(ISC vk_code);

struct KeyId {
  ISC vk_code;
  const CHA* label;
  const CHA* description;

  KeyId(VKCode code, const CHA* label);

  void ReprogramConsole();
};

}  //< namespace _

::_::ROut& operator<<(::_::ROut& o, ::_::RIn i);
::_::ROut& operator<<(::_::ROut& o, ::_::RIn& i);

::_::ROut& operator<<(::_::ROut& o, ::_::Pausef& i);

#endif
#endif
