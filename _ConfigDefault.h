// Copyright AStarship <https://astarship.net>.

#define _CrabsRunTests
#define _CrabsRoomBytes (1 << 20)
#define _CrabsRoomNameLengthMax  16

#define _CrabsUsing1ByteTypes
#define _CrabsUsing2ByteTypes
#define _CrabsUsing4ByteTypes
#define _CrabsUsing8ByteTypes

#if defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 2)
#define _CPUSize _CPU16Bit
#elif defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 4)
#define _CPUSize _CPU32Bit
#else
#define _CPUSize _CPU8Byte
#endif
#define _CPUEndian _CPUEndianLittle

#define _DefaultIntSize _CPUSize

#define _LargestChar _StringTypeA
#define _LargestSTR _StringTypeA

#define _FPNative FPD

#define _UsingSoftwareFPE

#define _Using16ByteTypes

#define _BOLSize 4 //< Size of boolean in bytes.

// Last 128-bit custom type; to disable define as 19 (ASCII BOL).
#define _ETEStop 19

// Last 64-bit custom type; to disable define as _CT5Stop.
#define _ETDStop _ETEStop

// Last 32-bit custom type; to disable define as _CT4Stop.
#define ETCStop _ETDStop

// Last 16-bit custom type; to disable define as _CT3Stop.
#define _ETBStop ETCStop

#define _AContextHandlerInit ACTXHandlerDefault
