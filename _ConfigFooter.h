// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CONFIG_FOOTER_H
#define _CONFIG_FOOTER_H

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// The below macros can be override in the _Config.h file.

#ifndef SEAM
#define SEAM CRABS_RELEASE
#endif

#if CRABS_MAX_PARAMS < 0
#error MAX_ERRORS must be greater than 0
#else
#ifndef CRABS_ROOM_NAME_LENGTH_MAX
#define CRABS_ROOM_NAME_LENGTH_MAX 32
#endif
enum {
  CrabsParamsMax = 420,  //< Max items in a BSQ in a Crabs expression.
	CrabsRoomNameLengthMax = CRABS_ROOM_NAME_LENGTH_MAX
};
#endif

#undef CRABS_ROOM_NAME_LENGTH_MAX

#ifndef CRABS_MAX_WALLS
#define CRABS_MAX_WALLS 32767
#endif
#ifndef CRABS_MAX_ERRORS
#define CRABS_MAX_ERRORS 32767
#endif
#ifndef CRABS_MAX_PARAMS
#define CRABS_MAX_PARAMS 32767
#endif
#ifndef CRABS_COM_TIMEOUT_TICKS
#define CRABS_COM_TIMEOUT_TICKS 32767
#else
#if CRABS_COM_TIMEOUT_TICKS < 0
#error MAX_ERRORS must be greater than 0!
#endif
#endif
#ifndef CRABS_MAX_ADDRESS_LENGTH
#define CRABS_MAX_ADDRESS_LENGTH 32767
#endif
#ifndef CRABS_OP_MAX_NAME_LENGTH
#define CRABS_OP_MAX_NAME_LENGTH 32767
#endif
#ifndef CRABS_OP_MAX_DECRABSION_LENGTH
#define CRABS_OP_MAX_DECRABSION_LENGTH 32767
#endif
#ifndef CRABS_TEXT_LENGTH
#define CRABS_TEXT_LENGTH 32767
#endif
#ifndef CRABS_BOOFER_SIZE_RX
#define CRABS_BOOFER_SIZE_RX 32767
#endif
#ifndef CRABS_BOOFER_SIZE_TX
#define CRABS_BOOFER_SIZE_TX 32767
#endif
#ifndef CRABS_WINDOW_SIZE_MIN
#define CRABS_WINDOW_SIZE_MIN 512
#endif
#ifndef CRABS_CPU_CACHE_LINE_SIZE
#define CRABS_CPU_CACHE_LINE_SIZE 64
#endif
#ifndef CRABS_BOOFER_COUNT_DEFAULT
#define CRABS_BOOFER_COUNT_DEFAULT 1024
#else
#if CRABS_BOOFER_COUNT_DEFAULT < 1
#define CRABS_BOOFER_COUNT_DEFAULT 512
#endif
#endif
#ifndef CRABS_STACK_COUNT_MAX_DEFAULT
#define CRABS_STACK_COUNT_MAX_DEFAULT 32
#endif
#ifndef CRABS_FLOOR_SIZE
#define CRABS_FLOOR_SIZE 1024
#endif
#ifndef CRABS_STACK_COUNT_MAX_DEFAULT
#define CRABS_STACK_COUNT_MAX_DEFAULT 16
#endif
#ifndef CRABS_OBJ_SIZE_DEFAULT
#define CRABS_OBJ_SIZE_DEFAULT 256
#endif
#ifndef CRABS_TOKEN_COUNT
#define CRABS_TOKEN_COUNT 32
#endif
#ifndef CRABS_STRING_COUNT_DEFAULT
#define CRABS_STRING_COUNT_DEFAULT 28
#endif
#ifndef CRABS_CIN_boofer_SIZE
#define CRABS_CIN_boofer_SIZE 81
#endif
#ifndef CRABS_UNICODE_VERSION_MAJOR
#define CRABS_UNICODE_VERSION_MAJOR 12
#endif
#ifndef CRABS_UNICODE_VERSION_MINOR
#define CRABS_UNICODE_VERSION_MINOR 1
#endif
#ifndef CRABS_UNICODE_ASSIGNED_CODE_POINTS
#define CRABS_UNICODE_ASSIGNED_CODE_POINTS 277576
#endif
#ifndef CRABS_CONSOLE_WIDTH
#define CRABS_CONSOLE_WIDTH 80
#endif
#ifndef CRABS_DTA_WIDTH
#define CRABS_DTA_WIDTH 0
#endif
#ifndef CLOCK_EPOCH_YEAR  
#define CLOCK_EPOCH_YEAR 1970
#endif
#ifndef SLOT_SIZE_MIN
#define SLOT_SIZE_MIN CRABS_CPU_CACHE_LINE_SIZE
#endif

#ifndef CRABS_ACTX_HANDLER_INIT
#define CRABS_ACTX_HANDLER_INIT ACTXHandlerDefault
#endif

// End overridable `_Config.h` parameters.
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#if CRABS_PLATFORM == OS_WINDOWS
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '\\',  //< The file path delimiter char.
  FilenamePad = 2,            //< extra chars for the "\\*" mask.
};
#elif CRABS_PLATFORM == OS_LINUX
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
enum {
  URIPathLengthMax = 4047,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#elif CRABS_PLATFORM == OS_APPLE
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#else
#define APP_EXIT_SUCCESS 0
#define APP_EXIT_FAILURE 1
enum {
  URIPathLengthMax = 1023,
  URLFilenameLengthMax = 255,
  AFilePathDelimitir = '/',  //< The file path delimiter char.
  FilenamePad = 0,
};
#endif

#if CRABS_BYT == 0
typedef IUA BYT;  //< Byte type for boofering.
#else
typedef ISA BYT;  //< Byte type for boofering.
#endif

// 128-bit integer support for GCC/Clang on x86_64
// Only define if not already defined (e.g., by _ConfigHeader.h)
// Note: ISE/IUE may be typedef'd in Header, so we use a macro guard
#if defined(__GNUC__) && !defined(CRABS_ISE_DEFINED) && !defined(CRABS_IUE_DEFINED)
typedef __int128 ISE;
typedef unsigned __int128 IUE;
#define CRABS_ISE_DEFINED
#define CRABS_IUE_DEFINED
#define HAS_128_BIT_INT
#endif

#if SEAM < CRABS_COUT
#undef USING_STB
#undef USING_STC
#define USING_STB NO_0
#define USING_STC NO_0
#endif

#if SEAM < CRABS_FTOS
#undef USING_FPC
#undef USING_FPD
#define USING_FPC NO_0
#define USING_FPD NO_0
#endif

#if USING_FP == 4
typedef FPC FPW;
#elif USING_FP == 8
typedef FPD FPW;
#endif

#if CPU_SIZE == CPU_2_BYTE
#define CPU_IS_LESS_THAN_32_BIT 1
#define CPU_IS_LESS_THAN_64_BIT 1
#define CPU_IS_GREATER_THAN_16_BIT 0
#define CPU_IS_GREATER_THAN_32_BIT 0
#elif CPU_SIZE == CPU_4_BYTE
#define CPU_IS_LESS_THAN_32_BIT 0
#define CPU_IS_LESS_THAN_64_BIT 1
#define CPU_IS_GREATER_THAN_16_BIT 1
#define CPU_IS_GREATER_THAN_32_BIT 0
#elif CPU_SIZE == CPU_8_BYTE
#define CPU_IS_LESS_THAN_32_BIT 0
#define CPU_IS_LESS_THAN_64_BIT 0
#define CPU_IS_GREATER_THAN_16_BIT 1
#define CPU_IS_GREATER_THAN_32_BIT 1
#endif

#if USING_STR == STRING_TYPE_A
typedef CHA CHR;
typedef CHC CHD;
typedef CHA* STR;
#elif USING_STR == STRING_TYPE_B
typedef CHB CHR;
typedef CHB* STR;
typedef CHC CHD;
#elif USING_STR == STRING_TYPE_C
typedef CHC CHR;
typedef CHC* STR;
typedef CHC CHD;
#else
typedef CHA CHR;
typedef CHA CHD;
typedef CHA* STR;
#endif

#if DEFAULT_INT_SIZE == ATYPE_8BIT || DEFAULT_INT_SIZE == ATYPE_16BIT
typedef ISB ISR;
typedef IUB IUR;
typedef float FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#elif DEFAULT_INT_SIZE == ATYPE_32BIT
typedef ISC ISR;
typedef IUC IUR;
typedef float FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#elif DEFAULT_INT_SIZE == ATYPE_64BIT
typedef ISD ISR;
typedef IUD IUR;
typedef double FPR;  //< Floating-point number at least 16 bits wide (1,2,4,8).
#endif

// ISQ and IUQ are arbitrary-precision integers (JavaScript BigInt-style).
// Defined via BigInt.h / BigInt.hxx — included by Crabs.h.
// Forward-declared here to keep the config chain intact.
namespace _ { class ISQ; class IUQ; }

typedef uint64_t TMS;  //< Seconds-since-epoch timestamp (4,8 bytes).


//< The Largest char, ASCII or Unicode.
#if LARGEST_CHAR == STRING_TYPE_A
typedef CHA CHL;
#else
typedef CHC CHL;
#endif

#if CPU_SIZE == CPU_8_BYTE
typedef ISB ISM;  //< Half of ISN type
typedef IUB UIL;  //< Half of IUN type
typedef ISW ISX;  //< Signed double-word int.
typedef IUW IUX;  //< Unsigned double-word int.
typedef ISW ISV;  //< Signed half-word int.
typedef IUW IUV;  //< Unsigned half-word int.
#if COMPILER_SUPPORTS_16_BYTE_INTEGERS
#define LARGEST_POD_TYPE 16
#else
#define LARGEST_POD_TYPE 8
#endif
#elif CPU_SIZE == CPU_4_BYTE
typedef ISC ISX;  //< Signed double-word int.
typedef IUC IUX;  //< Unsigned double-word int.
typedef ISB ISM;  //< Signed half-word int.
typedef ISB ISV;  //< Signed half-word int.
typedef IUB UIL;  //< Unsigned half-word int.
typedef IUB IUV;  //< Unsigned half-word int.
#define LARGEST_POD_TYPE 8
#elif CPU_SIZE == CPU_2_BYTE
typedef ISB ISO;  //< Signed double-word int.
typedef ISB ISX;  //< Signed double-word int.
typedef IUB IUO;  //< Unsigned double-word int.
typedef IUB IUX;  //< Unsigned double-word int.
typedef ISA ISV;  //< Signed half-word int.
typedef ISA ISM;  //< Signed half-word int.
typedef IUA IUV;  //< Unsigned half-word int.
typedef IUA UIL;  //< Unsigned half-word int.
#else
#define LARGEST_POD_TYPE 4
#endif

#ifdef TME_32_BIT
typedef TMC TME;
#else
typedef TMD TME;
#endif

namespace _ {
// Default character type.
#if USING_STR == STRING_TYPE_A
enum {
  // Default String type is UTF-8.
  STR_ = (_ARY << (ATypePODBits - 1)) | _CHA,
};
#elif USING_STR == STRING_TYPE_B
enum {
  // Default String type is UTF-8.
  STR_ = (_ARY << (TypePODBitCount - 1)) | _CHB,
};
#elif USING_STR == STRING_TYPE_C
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

#if ETE_STOP > 19 && ETE_STOP < 32
#if CT5_STOP > CT4_STOP
#error CT5 is invalid.
#endif
#define USING_CT5 1
typedef IUE CT5;
enum {
  _CT5 = CT5_STOP
};
#undefine CT5_STOP
#undefine CT5_STOP
#else
enum {
  _ECE = 19
};
#endif

#if ETD_STOP != ETE_STOP
#if CT4_STOP > CT5_STOP
#error CT4 is invalid.
#endif
#define USING_CT4 1
typedef IUD CT4;
enum {
  _CT4 = CT4_STOP
};
#undefine CT4_STOP
#else
enum {
  _ECD = _ECE
};
#endif

#if ETC_STOP != ETD_STOP
#if CT3_STOP > CT4_STOP
#error CT3 is invalid.
#endif
#define USING_CT3 1
typedef IUC CT3;
enum {
  _CT3 = CT3_STOP
};
#undefine CT3_STOP
#else
enum {
  _ECC = _ECD,
  BOLSizeLog2 = (sizeof(BOL) == 2) ? 1
              : (sizeof(BOL) == 4) ? 2
              : (sizeof(BOL) == 8) ? 3
              : 0
};
#endif

#if ECB_STOP != ETC_STOP
#if ECB_STOP > CT3_STOP
#error CT2 is invalid.
#endif
#define USING_CT2 1
typedef IUB CT2;
enum {
  _CT2 = ECB_STOP
};
#undefine ECB_STOP
#else
enum {
  _ECB = _ECC
};
#endif
enum {
  _ECA = 31
};

#define PRIME_LARGEST_IUB 65521
#define PRIME_LARGEST_UI4 4294967291
#define PRIME_LARGEST_UI8 18446744073709551557

#define SIZEOF_ARRAY(type) (ISW(&type + 1) - ISW(&type))

enum {
  ACRBytes = CRABS_ROOM_BYTES,          //< Size of the Chinese Room in bytes.
  CRSlotSizeMin = SLOT_SIZE_MIN,        //< Min size of a Slot.
  AClockEpochYearInit = CLOCK_EPOCH_YEAR,   //< Timestamp epoch year, default: 1970.
  ACPUCacheLineSize = CRABS_CPU_CACHE_LINE_SIZE,    //< CPU Cache line size.
  CRMaxFloorsCount = CRABS_MAX_WALLS, //< Size of the Room Floor (socket).
  CRErrorTotal = CRABS_MAX_ERRORS, //< Max errors before blowing up.
  CRParamTotal = CRABS_MAX_PARAMS, //< Max number_ of parameters.
  CRTimeoutMicroseconds = CRABS_COM_TIMEOUT_TICKS,  //< Sub-second ticks.
  CRAddressLengthMax =
      CRABS_MAX_ADDRESS_LENGTH,  //< Max address (_ADR) length.
  CrabsOpNameLengthMax = CRABS_OP_MAX_NAME_LENGTH,
  // Max length of a Op description .
  CrabsOpDescriptionLengthMax = CRABS_OP_MAX_DECRABSION_LENGTH,
  // Max length of a Text.
  ACharCount = CRABS_TEXT_LENGTH,

  // Size of the Display Print Slot.
  ASlotBooferSizeRx = CRABS_BOOFER_SIZE_RX,

  // Size of the KeyboardInBoofer.
  ASlotBooferSizeTx = CRABS_BOOFER_SIZE_TX,

  AWindowSizeMin = CRABS_WINDOW_SIZE_MIN,

  ABooferSizeDefault = CRABS_BOOFER_COUNT_DEFAULT,
  AFloorSize = CRABS_FLOOR_SIZE,  //< Size, or initial size, of the Floor.
  AStackTotalDefault = CRABS_STACK_COUNT_MAX_DEFAULT,
  AObjSizeDefault = CRABS_OBJ_SIZE_DEFAULT,

  // Extra reserved memory at the stop of BOut.
  ABOutOverflowSize = 32,

  APrintC0Offset = 176,  //< Value add to values 0-32 when printing.
  ATokenLongest  = 31,   //< Max length of a token plus one.
  AMinStackSize  = 1,    //< Min Crabs stack size.
  ABooferSizeDefaultWords =
      ABooferSizeDefault / sizeof(ISW) + ABooferSizeDefault % sizeof(ISW) ? 1
                                                                          : 0,
  ASTRCount = CRABS_STRING_COUNT_DEFAULT,
  AConsoleWidth = CRABS_CONSOLE_WIDTH,
  CInBooferSize =
      CRABS_CIN_boofer_SIZE,         //< Preallocated CIn boofer char count.
  AKeyEnter = 0,                       //< The keyboard value for enter.
  ATypeLargestPOD = LARGEST_POD_TYPE,  //< The largest POD type.
};


enum {
  SZA     = 0,
  SZB     = 1,
  SZC     = 2,
  SZD     = 3,
  SZE     = 4,
};
}  //< namespace _

#undef CRABS_ROOM_BYTES
#undef MAX_ERRORS
#undef MAX_NUM_PARAMS
#undef MAX_STRING_LENGTH
#undef COM_TIMEOUT_TICKS
#undef CRABS_LOG_SIZE
#undef OPERATION_MAX_NAME_LENGTH
#undef OPERATION_MAX_DECRABSION_LENGTH
#undef CRABS_TEXT_LENGTH
#undef CRABS_BOOFER_SIZE_RX
#undef CRABS_BOOFER_SIZE_TX
#undef CRABS_WINDOW_SIZE_MIN
#undef CRABS_BOOFER_COUNT_DEFAULT
#undef CRABS_STACK_COUNT_MAX_DEFAULT
#undef CRABS_FLOOR_SIZE
#undef CRABS_STACK_COUNT_MAX_DEFAULT
#undef CRABS_OBJ_SIZE_DEFAULT
#undef CRABS_TOKEN_COUNT
#undef CRABS_CPU_CACHE_LINE_SIZE
#undef CRABS_STRING_COUNT_DEFAULT

#endif  // _CONFIG_FOOTER_H

