// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _ConfigFooterH
#define _ConfigFooterH

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// The below macros can be override in the _Config.h file.

#if _CrabsParamsMax < 0
#error _CrabsErrorsMax must be greater than 0
#else
#ifndef _CrabsRoomNameLengthMax
#define _CrabsRoomNameLengthMax 32
#endif
enum {
  CrabsParamsMax = 420,  //< Max items in a BSQ in a Crabs expression.
	CrabsRoomNameLengthMax = _CrabsRoomNameLengthMax
};
#endif

#undef _CrabsRoomNameLengthMax

#ifndef _CrabsWallsMax
#define _CrabsWallsMax 32767
#endif
#ifndef _CrabsMaxErrors
#define _CrabsMaxErrors 32767
#endif
#ifndef _CrabsParamsMax
#define _CrabsParamsMax 32767
#endif
#ifndef _CrabsComTimeoutTicks
#define _CrabsComTimeoutTicks 32767
#else
#if _CrabsComTimeoutTicks < 0
#error _CrabsErrorsMax must be greater than 0!
#endif
#endif
#ifndef _CrabsAddressLengthMax
#define _CrabsAddressLengthMax 32767
#endif
#ifndef _CrabsOpNameLengthMax
#define _CrabsOpNameLengthMax 32767
#endif
#ifndef _CrabsOpDescriptionLengthMax
#define _CrabsOpDescriptionLengthMax 32767
#endif
#ifndef _CrabsTextLength
#define _CrabsTextLength 32767
#endif
#ifndef _CrabsBufferSizeRX
#define _CrabsBufferSizeRX 32767
#endif
#ifndef _CrabsBufferSizeTX
#define _CrabsBufferSizeTX 32767
#endif
#ifndef _CrabsWindowSizeMin
#define _CrabsWindowSizeMin 512
#endif
#ifndef _CPUCacheLineSize
#define _CPUCacheLineSize 64
#endif
#ifndef _CrabsBufferCountDefault
#define _CrabsBufferCountDefault 1024
#else
#if _CrabsBufferCountDefault < 1
#define _CrabsBufferCountDefault 512
#endif
#endif
#ifndef _CrabsStackCountMaxDefault
#define _CrabsStackCountMaxDefault 32
#endif
#ifndef _CrabsFloorSize
#define _CrabsFloorSize 1024
#endif
#ifndef _CrabsStackCountMaxDefault
#define _CrabsStackCountMaxDefault 16
#endif
#ifndef _CrabsObjectSizeDefault
#define _CrabsObjectSizeDefault 256
#endif
#ifndef _CrabsTokenCount
#define _CrabsTokenCount 32
#endif
#ifndef _CrabsStringCountDefault
#define _CrabsStringCountDefault 28
#endif
#ifndef _CrabsRefInBufferSize
#define _CrabsRefInBufferSize 81
#endif
#ifndef _CrabsUnicodeVersionMajor
#define _CrabsUnicodeVersionMajor 12
#endif
#ifndef _CrabsUnicodeVersionMinor
#define _CrabsUnicodeVersionMinor 1
#endif
#ifndef _CrabsUnicodeAssignedCodePoints
#define _CrabsUnicodeAssignedCodePoints 277576
#endif
#ifndef _RefOutWidth
#define _RefOutWidth 80
#endif
#ifndef _CrabsDataWidth
#define _CrabsDataWidth 0
#endif
#ifndef _ClockEpochYearBase  
#define _ClockEpochYearBase 1970
#endif
#ifndef _CrabsSlotSizeMin
#define _CrabsSlotSizeMin _CPUCacheLineSize
#endif

#ifndef _CrabsContextHandlerInitialize
#define _CrabsContextHandlerInitialize ACTXHandlerDefault
#endif

// End overridable `_Config.h` parameters.
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#if _CrabsPlatform == _OSWindows
#define _ApplicationExitSuccess 0
#define _ApplicationExitFailure 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '\\',  //< The file path delimiter char.
  FilenamePad = 2,            //< extra chars for the "\\*" mask.
};
#elif _CrabsPlatform == _OSLinux
#define _ApplicationExitSuccess 0
#define _ApplicationExitFailure 1
enum {
  URIPathLengthMax = 4047,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#elif _CrabsPlatform == _OSApple
#define _ApplicationExitSuccess 0
#define _ApplicationExitFailure 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#else
#define _ApplicationExitSuccess 0
#define _ApplicationExitFailure 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#endif

// 128-bit integer support for GCC/Clang on x86_64
// Only define if not already defined (e.g., by _ConfigHeader.h)
// Note: ISE/IUE may be typedef'd in Header, so we use a macro guard
#if defined(__GNUC__) && !defined(CRABS_ISE_DEFINED) && !defined(_CrabsUsing16ByteTypes)
typedef __int128 ISE;
typedef unsigned __int128 IUE;
#define CRABS_ISE_DEFINED
#define _CrabsUsing16ByteTypes
#define HAS_128_BIT_INT
#endif

// FPW = the native floating point type (FPC=4-byte, FPD=8-byte). Branch on
// sizeof rather than _FPNative, which is the *type name* (a typedef), not an
// integer width — `#if _FPNative == 8` was always false, so FPW was undefined.
#if _CPUSize == _CPU16Bit
typedef FPC FPW;
#else
typedef FPD FPW;
#endif

#if _CPUSize == _CPU16Bit
#define _CPUIsLessThan32Bit 1
#define _CPUIsLessThan64Bit 1
#define _CPUIsGreaterThan16Bit 0
#define _CPUIsGreaterThan32Bit 0
#elif _CPUSize == _CPU32Bit
#define _CPUIsLessThan32Bit 0
#define _CPUIsLessThan64Bit 1
#define _CPUIsGreaterThan16Bit 1
#define _CPUIsGreaterThan32Bit 0
#elif _CPUSize == _CPU8Byte
#define _CPUIsLessThan32Bit 0
#define _CPUIsLessThan64Bit 0
#define _CPUIsGreaterThan16Bit 1
#define _CPUIsGreaterThan32Bit 1
#endif

#if _LargestSTR == _StringTypeA
typedef CHA CHR;
typedef CHC CHD;
typedef CHA* STR;
#elif _LargestSTR == _StringTypeB
typedef CHB CHR;
typedef CHB* STR;
typedef CHC CHD;
#elif _LargestSTR == _StringTypeC
typedef CHC CHR;
typedef CHC* STR;
typedef CHC CHD;
#else
typedef CHA CHR;
typedef CHA CHD;
typedef CHA* STR;
#endif

#if _DefaultIntSize == _AType1Byte || _DefaultIntSize == _AType2Byte
typedef ISB ISR;
typedef IUB IUR;
typedef float FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#elif _DefaultIntSize == _AType4Byte
typedef ISC ISR;
typedef IUC IUR;
typedef float FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#elif _DefaultIntSize == ATYPE_64BIT
typedef ISD ISR;
typedef IUD IUR;
typedef double FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#endif

// ISQ and IUQ are arbitrary-precision integers (JavaScript BigInt-style).
// Defined via BigInt.h / BigInt.hxx — included by Crabs.h.
// Forward-declared here to keep the config chain intact.
namespace _ { class ISQ; class IUQ; }

typedef IUD TMS;  //< Seconds-since-epoch timestamp (4,8 bytes).


//< The Largest char, ASCII or Unicode.
#if _LargestChar == _StringTypeA
typedef CHA CHL;
#else
typedef CHC CHL;
#endif

#if _CPUSize == _CPU8Byte
typedef ISB ISM;  //< Half of ISN type
typedef IUB UIL;  //< Half of IUN type
typedef ISW ISX;  //< Signed double-word int.
typedef IUW IUX;  //< Unsigned double-word int.
typedef ISW ISV;  //< Signed half-word int.
typedef IUW IUV;  //< Unsigned half-word int.
#define _LargestPODType 16
#elif _CPUSize == _CPU32Bit
typedef ISC ISX;  //< Signed double-word int.
typedef IUC IUX;  //< Unsigned double-word int.
typedef ISB ISM;  //< Signed half-word int.
typedef ISB ISV;  //< Signed half-word int.
typedef IUB UIL;  //< Unsigned half-word int.
typedef IUB IUV;  //< Unsigned half-word int.
#define _LargestPODType 8
#elif _CPUSize == _CPU16Bit
typedef ISB ISO;  //< Signed double-word int.
typedef ISB ISX;  //< Signed double-word int.
typedef IUB IUO;  //< Unsigned double-word int.
typedef IUB IUX;  //< Unsigned double-word int.
typedef ISA ISV;  //< Signed half-word int.
typedef ISA ISM;  //< Signed half-word int.
typedef IUA IUV;  //< Unsigned half-word int.
typedef IUA UIL;  //< Unsigned half-word int.
#else
#define _LargestPODType 4
#endif

#ifdef _CrabsTME4Byte
typedef TMC TME;
#else
typedef TMD TME;
#endif

namespace _ {
// Default character type.
#if _LargestSTR == _StringTypeA
enum {
  // Default String type is UTF-8.
  STR_ = (_ARY << (ATypePODBits - 1)) | _CHA,
};
#elif _LargestSTR == _StringTypeB
enum {
  // Default String type is UTF-8.
  STR_ = (_ARY << (TypePODBitCount - 1)) | _CHB,
};
#elif _LargestSTR == _StringTypeC
enum {
  // Default String type is UTF-8.
  STR_ = (_ARY << (TypePODBitCount - 1)) | _CHC,
};
#else
#error Invalid string type.
#endif
enum {
  // Address type, which is the same as the default string type for the system.
  _ADR = STR_,
  _STR = 1,     //< ASCII, UTF-8, UTF-16, or UTF-32 string.
};

#if _ETEStop > 19 && _ETEStop < 32
#if _CT5Stop > _CT4Stop
#error CT5 is invalid.
#endif
#define USING_CT5 1
typedef IUE CT5;
enum {
  _CT5 = _CT5Stop
};
#undefine _CT5Stop
#undefine _CT5Stop
#else
enum {
  _ECE = 19
};
#endif

#if _ETDStop != _ETEStop
#if _CT4Stop > _CT5Stop
#error CT4 is invalid.
#endif
#define USING_CT4 1
typedef IUD CT4;
enum {
  _CT4 = _CT4Stop
};
#undefine _CT4Stop
#else
enum {
  _ECD = _ECE
};
#endif

#if ETCStop != _ETDStop
#if _CT3Stop > _CT4Stop
#error CT3 is invalid.
#endif
#define _UsingCT3 1
typedef IUC CT3;
enum {
  _CT3 = _CT3Stop
};
#undefine _CT3Stop
#else
enum {
  _ECC = _ECD,
  BOLSizeLog2 = (sizeof(BOL) == 2) ? 1
              : (sizeof(BOL) == 4) ? 2
              : (sizeof(BOL) == 8) ? 3
              : 0
};
#endif

#if _ETBStop != ETCStop
#if _ETBStop > _CT3Stop
#error CT2 is invalid.
#endif
#define _UsingCT2 1
typedef IUB CT2;
enum {
  _CT2 = _ETBStop
};
#undefine _ETBStop
#else
enum {
  _ECB = _ECC
};
#endif
enum {
  _ECA = 31
};

#define _PrimeLargestIUB 65521
#define _PrimeLargestIUC 4294967291
#define _PrimeLargestIUD 18446744073709551557

#define _SizeOfArray(type) (ISW(&type + 1) - ISW(&type))

enum {
  ACRBytes = _CrabsRoomBytes,          //< Size of the Chinese Room in bytes.
  CRSlotSizeMin = _CrabsSlotSizeMin,        //< Min size of a Slot.
  AClockEpochYearInit = _ClockEpochYearBase,   //< Timestamp epoch year, default: 1970.
  ACPUCacheLineSize = _CPUCacheLineSize,    //< CPU Cache line size.
  CRMaxFloorsCount = _CrabsWallsMax, //< Size of the Room Floor (socket).
  CRErrorTotal = _CrabsMaxErrors, //< Max errors before blowing up.
  CRParamTotal = _CrabsParamsMax, //< Max number_ of parameters.
  CRTimeoutMicroseconds = _CrabsComTimeoutTicks,  //< Sub-second ticks.
  CRAddressLengthMax =
      _CrabsAddressLengthMax,  //< Max address (_ADR) length.
  CrabsOpNameLengthMax = _CrabsOpNameLengthMax,
  // Max length of a Op description .
  CrabsOpDescriptionLengthMax = _CrabsOpDescriptionLengthMax,
  // Max length of a Text.
  ACharCount = _CrabsTextLength,

  // Size of the Display Print Slot.
  ASlotBufferSizeRx = _CrabsBufferSizeRX,

  // Size of the KeyboardInBuffer.
  ASlotBufferSizeTx = _CrabsBufferSizeTX,

  AWindowSizeMin = _CrabsWindowSizeMin,

  ABufferSizeDefault = _CrabsBufferCountDefault,
  AFloorSize = _CrabsFloorSize,  //< Size, or initial size, of the Floor.
  AStackTotalDefault = _CrabsStackCountMaxDefault,
  AObjSizeDefault = _CrabsObjectSizeDefault,

  // Extra reserved memory at the stop of TBOut.
  ABOutOverflowSize = 32,

  APrintC0Offset = 176,  //< Value add to values 0-32 when printing.
  ATokenLongest  = 31,   //< Max length of a token plus one.
  AMinStackSize  = 1,    //< Min Crabs stack size.
  ABufferSizeDefaultWords =
      ABufferSizeDefault / sizeof(ISW) + ABufferSizeDefault % sizeof(ISW) ? 1
                                                                          : 0,
  ASTRCount = _CrabsStringCountDefault,
  AConsoleWidth = _RefOutWidth,
  RInBufferSize =
      _CrabsRefInBufferSize,         //< Preallocated RIn buffer char count.
  AKeyEnter = 0,                       //< The keyboard value for enter.
  ATypeLargestPOD = _LargestPODType,  //< The largest POD type.
};


enum {
  SZA     = 0,
  SZB     = 1,
  SZC     = 2,
  SZD     = 3,
  SZE     = 4,
};
}  //< namespace _

#undef _CrabsRoomBytes
#undef _ErrorsMax
#undef _ParamCountMax
#undef _StringLengthMax
#undef _ComTimeoutTicks
#undef _CrabsLogSize
#undef _OperationNameLengthMax
#undef _OperationDescriptionLengthMax
#undef _CrabsTextLength
#undef _CrabsBufferSizeRX
#undef _CrabsBufferSizeTX
#undef _CrabsWindowSizeMin
#undef _CrabsBufferCountDefault
#undef _CrabsStackCountMaxDefault
#undef _CrabsFloorSize
#undef _CrabsStackCountMaxDefault
#undef _CrabsObjectSizeDefault
#undef _CrabsTokenCount
#undef _CPUCacheLineSize
#undef _CrabsStringCountDefault

#endif  //< _ConfigFooterH

