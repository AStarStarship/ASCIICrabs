// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TYPES_DECL
#define CRABS_TYPES_DECL
#include <_Config.h>

/* Function that remaps a Standard Data Type to an Extended Plain or Extended
Custom Type or prints it to a SPrinter or writes it to a boofer.
Control Bits:
I have no clue what we're going to do with the control bits right now but we
have a word, we only need 16-bits, so why not reserve the entire word?
@param  begin The first byte in the CHR boofer.
@param  end   The CHR after the last char in the boofer.
@param  type  A 16-bit ASCII Type in the LSB and control bits in the MSb.
@param  value A pointer to a value to print.
@return If start is nil the remapped Extended Plain Type.
        If start and stop are not nil a pointer to the CHR after the nil term
        char upon success. 
*/
typedef DTW(*ACTXHandler)(void* begin, void* end, DTW type, IUW value,
                          IUW vmsb);

#if USING_VISUAL_CPP_X64
// A 128-bit unsigned integer.
struct IUE {
  IUD lsb,
      msb;
};
#endif

namespace _ {

/* Remaps a Plain Context Type to another POD type 1-19.
Store the 5-bit remapped type for _PCa in b0:b4, _PCb in b5:b9, etc. */
DTB ATypeRemapEP(DTW sw_vt_bits, DTW pod_type, DTD ep_remap);

static DTW ACTXHandlerDefault(void* begin, void* end, DTW type, IUW value,
                              IUW vmsb);

/* A single cache-line-aligned variable set for controlling the Chinese Room.
*/
struct alignas(ACPUCacheLineSize) ACTXFrame {
  enum {
    CBytes = 256 + ACPUCacheLineSize,
    BytesReserved = (CBytes - 2 * sizeof(IUW)) >> ACPUBytesLog2,
  };
  ACTXHandler handler;            //< ASCII Context Handler.
  ISW epoch;                      //< 32-bit Unix timestamp epoch.
#if CPU_SIZE != CPU_2_BYTE
  IUW scratchpad[BytesReserved] = {0};  //< Cache line alignment words.
#endif

  ACTXFrame();
};

// Returns true if the given type is a POD type.
BOL ATypeIsPOD(DTB type);

/* Returns the the Global Chinese Room Entry. */
ACTXFrame* ACTX();

// Maps a type to ACTXHandler.
DTW ACTXHandle(ACTXHandler actxh, void* begin, void* end, DTW type, 
                      IUW value, IUW vmsb);
// Maps a type to ACTXHandler.
DTW ACTXHandle(void* begin, void* end, DTW type, IUW value, IUW vmsb);

// Returns true if the given type is not statically allocated.
BOL ATypeIsVarLength(DTB type);

// Deasserts the MD bits if it's now CTX type.
DTB ATypeMDDeassert(DTW type);

// Asserts the Pointer modifier bits.
DTB ATypeMakePtr(DTW type);

// Asserts the Crabs index modifier bits.
DTB ATypeMakeCrabsIndex(DTW type);

// Asserts the Context Modifier bits.
DTB ATypeMDC(DTW type);

// Asserts the Context Modifier bits.
DTB ATypeCNS_MDC(DTW type);

// Asserts the Constant bit.
DTB ATypeMakeCNS(DTW type);

// Asserts the Pointer modifier bits.
DTB ATypeMakePtrCNS(DTW type);

// Asserts the Crabs index modifier bits.
DTB ATypeCrabsIndexCNS(DTW type);

// A 128-bit unsigned integer.
struct IUE {
  IUD lsb;    //< Least significant 64 bits.
  IUD msb;    //< Most significant 64 bits.

  IUE(IUB v1, IUB v2, IUB v3, IUB v4, IUB v5, IUB v6, IUB v7, IUB v8);

  IUE(IUC v1, IUC v2, IUC v3, IUC v4);

  IUE(IUD lsb, IUD msb = 0);
};

// A 128-bit signed integer.
struct ISE {
  IUD lsb;    //< Least significant 64 bits.
  IUD msb;    //< Most significant 64 bits.

  ISE(IUB v1, IUB v2, IUB v3, IUB v4, IUB v5, IUB v6, IUB v7, IUB v8);

  ISE(IUC v1, IUC v2, IUC v3, IUC v4);

  ISE(IUD lsb, IUD msb = 0);
};

#if USING_SOFTWARE_FPC == YES_0
#if CPU_SIZE == CPU_2_BYTES
// A 32-bit floating-point number.
struct FPC {
  IUB lsb;    //< Least significant 16 bits.
  IUB msb;    //< Most significant 16 bits.

  FPC(IUB lsb, IUB msB = 0);
};
#else
// A 32-bit floating-point number.
struct FPC {
  IUB lsb;    //< Least significant 16 bits.
  IUB msb;    //< Most significant 16 bits.

  FPC(IUB lsb, IUB msB = 0);
};
#endif
#endif
#if CPU_SIZE < CPU_8_BYTES
// A 64-bit floating-point number.
struct FPD {
  IUC lsb;    //< Least significant 32 bits.
  IUC msb;    //< Most significant 32 bits.

  FPD(IUB v1, IUB v2, IUB v3, IUB v4);

  FPD(IUC lsb, IUC msb = 0);
};
#endif

// A 128-bit floating-point number.
struct FPE {
  IUD lsb;    //< Least significant 64 bits.
  IUD msb;    //< Most significant 64 bits.

  FPE(IUB v1, IUB v2, IUB v3, IUB v4, IUB v5, IUB v6, IUB v7, IUB v8);

  FPE(IUC v1, IUC v2, IUC v3, IUC v4);

  FPE(IUD lsb, IUD msb = 0);
};

// Packs the given ASCII Type bitfield values into a DTB.
DTB ATypePack(DTB pod, DTB vt);
DTB ATypePack(DTB pod, DTB vt, DTB sw);
DTB ATypePack(DTB pod, DTB vt, DTB sw, DTB mt);
DTB ATypePack(DTB pod, DTB vt, DTB sw, DTB mt, DTB mod);

// Returns true if the given type is a CHA, CHB, or CHC.
BOL ATypeIsCH(DTB type);

// Returns the size of the given type in bytes.
ISA ATypeSizeOfPOD(DTB type);

/* Gets the alignment mask of the given type. */
DTW AlignmentMask(CHA item);
DTW AlignmentMask(ISA item);
DTW AlignmentMask(IUA item);
DTW AlignmentMask(CHB item);
DTW AlignmentMask(ISB item);
DTW AlignmentMask(IUB item);
DTW AlignmentMask(CHC item);
DTW AlignmentMask(ISC item);
DTW AlignmentMask(IUC item);
DTW AlignmentMask(FPC item);
DTW AlignmentMask(ISD item);
DTW AlignmentMask(IUD item);
DTW AlignmentMask(FPD item);
DTW AlignmentMask(void* item);
DTW AlignmentMask(const void* item);

/* Gets the type of the given item. */
DTW TypeOf(CHA item);
DTW TypeOf(ISA item);
DTW TypeOf(IUA item);
DTW TypeOf(CHB item);
DTW TypeOf(ISB item);
DTW TypeOf(IUB item);
DTW TypeOf(CHC item);
DTW TypeOf(ISC item);
DTW TypeOf(IUC item);
DTW TypeOf(FPC item);
DTW TypeOf(ISD item);
DTW TypeOf(IUD item);
DTW TypeOf(FPD item);
DTW TypeOf(CHA* item);
DTW TypeOf(const CHA* item);
DTW TypeOf(CHB* item);
DTW TypeOf(const CHB* item);
DTW TypeOf(CHC* item);
DTW TypeOf(const CHC* item);
DTW TypeOf(void* item);
DTW TypeOf(const void* item);

/* Returns the size of the given type in bytes.
@return the size bytes of the value. */
ISW ATypeBytes(const void* value, DTB type);
ISW ATypeBytes(const void* value_base, ISA bytes, DTB type);

// Returns the end (byte after last byte) of the given type-value.
void* ATypeValueEnd(void* value, DTB type);

// Returns an array of the customizable POD type sizes.
//const ISA* ATypeCustomSize();

// Returns an array of the user-defined type alignment masks.
//const ISA* ATypeCustomAlignMask();

// Returns the custom time alignment mask for the given type.
//ISA ATypeCustomAlignMask(DTA type);

// Converts a 16-bit ASCII Data Type bit pattern to Extended ASCII Type mapping.
DTB ATypeToEXT_NC(DTW type_msb, DTW type);
DTB ATypeToEXT(DTW type);
DTB ATypeToEXT(DTW type_msb, DTW type_pod);

// Checks if the given type is a Extended Plain or Extended Context Type.
BOL ATypeIsCTX(DTW sw_vt, DTW pod);

// Checks if the given type is a Extended Plain or Extended Context Type.
BOL ATypeIsCTX(DTW type);

/* Converts an ASCII Type to Context Type without checking.
@pre Dev shall check if the type is a CTX Type before calling. */
DTW ATypeToCTX_NC(DTW sw_vt, DTW pod);

/* Remaps EPa-EPl to POD types 0-19 using 12 5-bit maps in an 8-byte type.
@return (type >> (type - _EPa) * 5) & ATypePODMask */
DTB ATypeRemapEP_NC(DTW pod_type, DTW ep_remap);

/* Stores a pointer to the ASCII data type and it's value. */
struct ATypePtr {
  DTW   type;         //< ASCII data type word.
  const void* value;  //< Pointer to the value of the type.
};

// Gets the Unicode format: 0=UTF-8, 1=UTF-16, 2=UTF-32.
ISA ATypeTextFormat(DTW type);

class Nil {
public:
  /* Constructures nothing. */
  Nil();

  /* Gets the size of the socket. */
  static constexpr ISW Size();

  /* Gets the size of the socket. */
  static constexpr ISW CBytes();

  /* Gets the size of the socket. */
  static constexpr ISW SizeWords();

  /* Gets the nil origin word address. */
  IUW* Words();
};

/* An ASCII Type-Value tuple.
An TypeValue can only work with up to two CPU ALU words because when a CPU
multiplies two integers together the result is two ALU-sized words and thus the
CPU can only address 2 words at a time. For types that are larger that two ALU
words wide the word_ stores a pointer to that type in memory and a struct will
be needed to work with the type. */
class ATypeValue {
  DTW type_;    //< Type of the value.
  IUW value_,   //< First or only word of the value.
      vmsb_;    //< Most significant bits of two word type.

 public:
  /* Stores a NIL type with indeterminate word_. */
  ATypeValue();

#if USING_STA == YES_0
  explicit ATypeValue(CHA value);
  explicit ATypeValue(const CHA* value);
#endif
#if USING_STB == YES_0
  explicit ATypeValue(CHB value);
  explicit ATypeValue(const CHB* value);
#endif
#if USING_STC == YES_0
  explicit ATypeValue(CHC value);
  explicit ATypeValue(const CHC* value);
#endif
  explicit ATypeValue(ISA value);
  explicit ATypeValue(IUA value);
  explicit ATypeValue(ISB value);
  explicit ATypeValue(IUB value);
  explicit ATypeValue(ISC value);
  explicit ATypeValue(IUC value);
  explicit ATypeValue(ISD value);
  explicit ATypeValue(IUD value);
#if USING_FPC == YES_0
  explicit ATypeValue(FPC value);
#endif
#if USING_FPD == YES_0
  explicit ATypeValue(FPD value);
#endif

  /* Stores the value to the word_ and the ASCII type of the value. */
  explicit ATypeValue(void* value);
  explicit ATypeValue(const void* value);

  explicit ATypeValue(void* base_ptr, ISW offset);
  explicit ATypeValue(const void* base_ptr, ISW offset);

  explicit ATypeValue(DTW type, void* value);
  explicit ATypeValue(DTW type, const void* value);

  explicit ATypeValue(DTW type, void* base_ptr, ISW offset);
  explicit ATypeValue(DTW type, const void* base_ptr, ISW offset);

  explicit ATypeValue(DTW type, void* value, void* value_end);
  explicit ATypeValue(DTW type, const void* value, const void* value_end);

  explicit ATypeValue(DTW type, void* base_ptr, ISW offset, void* value_end);
  explicit ATypeValue(DTW type, const void* base_ptr, ISW offset, const void* value_end);

  explicit ATypeValue(DTW type, void* base_ptr, ISW offset, ISW base_bytes);
  explicit ATypeValue(DTW type, const void* base_ptr, ISW offset, ISW base_bytes);

  /* Gets the type_. */
  DTW Type();

  /* Gets the Unicode string format if any. */
  DTW UnicodeFormat();

  /* Gets a pointer to the word_. */
  void* WordPTR();

  /* Gets the value as the return type. */
  void* ToPTR();
  CHA* ToSTA();
  CHB* ToSTB();
  CHC* ToSTC();
  IUA  ToIUA();
  IUB  ToIUB();
  IUN  ToIUN();
  IUC  ToIUC();
  IUD  ToUID();

  /* Gets the first word of the word_. */
  IUW Value();

  /* Gets the second word of the value if the type is two words wide. */
  IUW MSB();

  /* Sets the word to the value. */
  void SetWord(IUW value);

  /* Sets the second word to the value. */
  void SetWord2(IUW value);

  /*Sets the type_ to _NIL with an indeterminate value. */
  void SetNIL();

  /*Sets the type_ to _NIL with an indeterminate value. */
  void SetNIL(IUW value);

  void* Set(void* value);
  const void* Set(const void* value);

  void* Set(void* base_ptr, ISW voffset);
  const void* Set(const void* base_ptr, ISW voffset);

  void* Set(DTW type, void* value);
  const void* Set(DTW type, const void* value);

  void* Set(DTW type, void* base_ptr, ISW voffset);
  const void* Set(DTW type, const void* base_ptr, ISW voffset);

  void* Set(DTW type, void* value, void* value_end);
  const void* Set(DTW type, const void* value, const void* value_end);

  void* Set(DTW type, void* base_ptr, ISW offset, void* value_end);
  const void* Set(DTW type, const void* base_ptr, ISW offset, 
                         const void* value_end);

  void* Set(DTW type, void* base_ptr, ISW offset, ISW base_byes);
  const void* Set(DTW type, const void* base_ptr, ISW offset, 
                         ISW base_byes);

  /*Sets the word_ to the given value and updates the type. */
#if USING_STA == YES_0
  void Set(CHA value);
  void Set(const CHA* value);
#endif
#if USING_STB == YES_0
  void Set(CHB value);
  void Set(const CHB* value);
#endif
#if USING_STC == YES_0
  void Set(const CHC* value);
  void Set(CHC value);
#endif
  void Set(ISA value);
  void Set(IUA value);
  void Set(ISB value);
  void Set(IUB value);
  void Set(BOL value);
  void Set(ISC value);
  void Set(IUC value);
  void Set(ISD value);
  void Set(IUD value);
#if USING_FPC == YES_0
  void Set(FPC value);
#endif
#if USING_FPD == YES_0
  void Set(FPD value);
#endif
};

}  //< namespace _
#endif
