// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_BINARY_HPP
#define CRABS_BINARY_HPP
#include <_Config.h>
namespace _ {

/* Syntactical sugar for reinterpret_cast using templates. */
template<typename T = void>
inline T* TPtr(void* ptr) {
  return reinterpret_cast<T*>(ptr);
}

/* Syntactical sugar for reinterpret_cast using templates. */
template<typename T = void>
inline T* TPtr(ISW value) {
  return reinterpret_cast<T*>(value);
}

/* Syntactical sugar for reinterpret_cast using templates. */
template<typename T = void>
inline const T* TPtr(const void* ptr) {
  return reinterpret_cast<const T*>(ptr);
}

/* Checks if the pointer is an ASCII Error Code. */
inline BOL IsError(const void* ptr) {
  return IUW(ptr) < AErrorTotal;
}
/* Checks if the function pointer is an ASCII Error Code. */
template<typename R, typename... Args>
inline BOL IsError(R(*)(Args...)) {
  return false;
}
/* Checks if the start is an ASCII Error Code and start < stop. */
inline BOL IsErrorSocket(const void* start, const void* stop) {
  return IUW(start) < AErrorTotal || start >= stop;
}

// Checks if the input is aligned to the T word boundary.
template<typename T = void>
inline BOL TAlignIs(const T* input) {
  return (IUW(input) & (sizeof(T) - 1)) == 0;
}
template<typename T = IUW, typename I = T>
inline BOL TAlignIs(I input) { return (input & (sizeof(T) - 1)) == 0; }

/* Syntactical sugar for reinterpret_cast using templates. */
template<typename T = void>
inline T* CPtr(const void* ptr) {
  return const_cast<T*>(reinterpret_cast<const T*>(ptr));
}

/* Syntactical sugar for reinterpret_cast using templates. */
template<typename T = void>
inline const T* CPtr(void* ptr) {
  return reinterpret_cast<const T*>(ptr);
}

/* Creates a T pointer from a base pointer plus the offset.
@param base The base address.
@param offset The offset in bytes. */
template<typename T = void>
inline T* TPtr(void* origin, ISW offset) {
  return reinterpret_cast<T*>(ISW(origin) + offset);
}
template<typename T = void>
inline const T* TPtr(const void* origin, ISW offset) {
  return reinterpret_cast<const T*>(ISW(origin) + offset);
}

template<typename T, typename Class>
inline T* TPtr(void* origin) {
  return reinterpret_cast<T*>(ISW(origin) + sizeof(Class));
}
template<typename T, typename Class>
inline const T* TPtr(const void* origin) {
  return reinterpret_cast<const T*>(ISW(origin) + sizeof(Class));
}

/* Creates a T pointer from a base pointer plus the offset.
@param base The base address.
@param offset The offset in bytes. */
template<typename T = void>
inline T* TPtr(void* origin, ISW offset1, ISW offset2) {
  return reinterpret_cast<T*>(ISW(origin) + offset1 + offset2);
}
template<typename T = void>
inline const T* TPtr(const void* origin, ISW offset1, ISW offset2) {
  return reinterpret_cast<const T*>(ISW(origin) + offset1 + offset2);
}
template<typename T = void>
inline T* TPtr(void* origin, ISW offset1, ISW offset2, ISW offset3) {
  return reinterpret_cast<T*>(ISW(origin) + offset1 + offset2 + offset3);
}
template<typename T = void>
inline const T* TPtr(const void* origin, ISW offset1, ISW offset2, 
  ISW offset3) {
  return reinterpret_cast<const T*>(ISW(origin) + offset1 + offset2 + offset3);
}

/* @defgroup Binary
@brief binary and pointer utilities. */

/* Converts the given value to a signed value of the same width. */
inline ISA ToSigned(CHA value) { return ISA(value); }
inline ISB ToSigned(CHB value) { return ISB(value); }
inline ISC ToSigned(CHC value) { return ISC(value); }
inline ISN ToSigned(CHN value) { return ISN(value); }
inline ISA ToSigned(IUA value) { return ISA(value); }
inline ISB ToSigned(IUB value) { return ISB(value); }
inline ISC ToSigned(IUC value) { return ISC(value); }
inline ISD ToSigned(IUD value) { return ISD(value); }
inline ISA ToSigned(ISA value) { return ISA(value); }
inline ISB ToSigned(ISB value) { return ISB(value); }
inline ISC ToSigned(ISC value) { return ISC(value); }
inline ISD ToSigned(ISD value) { return ISD(value); }
inline ISW ToSigned(const void* value) { return ISW(value); }
#if USING_FPC == YES_0
inline ISC ToSigned(FPC value) { return *TPtr<ISC>(&value); }
#endif
#if USING_FPD == YES_0
inline ISD ToSigned(FPD value) { return *TPtr<ISD>(&value); }
#endif

/* Converts the given value to a unsigned value of the same width. */
inline IUA ToUnsigned(CHA value) { return IUA(value); }
inline IUB ToUnsigned(CHB value) { return IUB(value); }
inline IUC ToUnsigned(CHC value) { return IUC(value); }
inline IUN ToUnsigned(CHN value) { return IUN(value); }
inline IUA ToUnsigned(ISA value) { return IUA(value); }
inline IUB ToUnsigned(ISB value) { return IUB(value); }
inline IUC ToUnsigned(ISC value) { return IUC(value); }
inline IUN ToUnsigned(BOL value) { return IUN(value); }
inline IUD ToUnsigned(ISD value) { return IUD(value); }
inline IUA ToUnsigned(IUA value) { return IUA(value); }
inline IUB ToUnsigned(IUB value) { return IUB(value); }
inline IUC ToUnsigned(IUC value) { return IUC(value); }
inline IUD ToUnsigned(IUD value) { return IUD(value); }
inline IUW ToUnsigned(const void* value) {
  return IUW(value);
}
#if USING_FPC == YES_0
inline IUC ToUnsigned(FPC value) { return *TPtr<IUC>(&value); }
#endif
#if USING_FPD == YES_0
inline IUD ToUnsigned(FPD value) { return *TPtr<IUD>(&value); }
#endif

/* Converts the value to a floating-point number. */
inline FPC ToFloat(IUC value) { return *TPtr<FPC>(&value); }
inline FPD ToFloat(IUD value) { return *TPtr<FPD>(&value); }

/* Returns the maximum value of the given unsigned type. */
template<typename IU>
inline IU TNaNUnsigned() {
  return ~IU(0);
}

/* Returns the minimum value of the given signed type (i.e. 0x100...0). */
template<typename IS>
constexpr IS TNaNSigned() {
  return IS(1) << (sizeof(IS) * 8 - 1);
}

template<typename IS = ISW>
inline IS TDelta(const void* start) {
  ISW delta = ISW(start);
  return IS(delta);
}
template<typename IS = ISW>
inline IS TDelta(const void* start, const void* stop) {
  ISW delta = ISW(stop) - ISW(start);
  return IS(delta);
}

/* Calculates the delta between the start and stop minus the sizeof(Class. */
template<typename IS, typename Class>
inline IS TDelta(const void* start) {
  ISW delta = ISW(start) - sizeof(Class);
  return IS(delta);
}
template<typename IS, typename Class>
inline IS TDelta(const void* start, const void* stop) {
  ISW delta = ISW(stop) - ISW(start) - sizeof(Class);
  return IS(delta);
}

enum {
  _Stack = 0,  //< Flag for using stack memory.
  _Heap = 1,   //< Flag for using heap memory.
};

/* Calculates the number of bits in the given type T. */
template<typename T>
constexpr T TBitCount() {
  return sizeof(T) * 8;
}

/* Returns the Log_2(N) for the sizeof (I).
@code
ISN bytes = 32;
ISN size_words = bytes >> TBitCount<ISN> ()
@endcode
*/
template<typename T, typename IS = ISW>
constexpr IS CBitCount() {
  IS size = sizeof(IS), 
     count = 1,
     b = 1;
  while (size < b << count) ++count;
  return count;
  /*
  IS b = 1;
  if (size < b << 8) { // 1-8
    if (size < b << 4) { // 1-4
      if (size < b << 2) {
        if (size < b << 1) return 1;
        else return 2;
      } else {
        if (size < b << 3) return 3;
        else return 4;
      }
    }
    else { // 5-8
      if (size < b << 6) {
        if (size < b << 5) return 5;
        else return 6;
      } else {
        if (size < b << 7) return 7;
        else return 8;
      }
    }
  } else { // 9-16
    if (size < b << 12) { // 9-12
      if (size < b << 10) {
        if (size < b << 9) return 9;
        else return 10;
      }
      else {
        if (size < b << 11) return 11;
        else return 12;
      }
    }
    else { // 13-16
      if (size < b << 14) {
        if (size < b << 13) return 13;
        else return 14;
      }
      else {
        if (size < b << 15) return 15;
        else return 16;
      }
    }
  }
  return 0;
  */
}

template<typename DT = DTB, DT dt>
constexpr ISN TBitCode() {
  if (dt <= _CHA) return 0;
  if (dt <= _CHB) return 1;
  if (dt <= _CHC) return 2;
  if (dt <= _SSD) return 3;
  if (dt <= _ISE) return 4;
  if (dt <= _ECE) return 4;
  if (dt <= _ECD) return 3;
  if (dt <= _ECC) return 2;
  if (dt <= _ECB) return 1;
  return 0;
}

/* Aligns the given value up to a sizeof (T) boundary.
@return The aligned value.
@param value The value to align.
@param mask  The power of 2 to align to minus 1 (makes the mask).
@desc Algorithm works by inverting the bits, mask of the LSbs and adding 1.
This allows the algorithm to word align without any if statements. The
algorithm works the same for all memory widths as proven by the truth
tables bellow.

@code
// The convention is that the unsigned size always comes first
// because it's the first IUA of an ASCII AArray.
ISC signed_example = 7;
signed_example = AlignUp<ISD, IUC, ISC> (signed_example);

IUB unsigned_example = 3;
unsgiend_example = AlignUp<ISC, IUB, IUB> (unsigned_example);

// 64-bit alignment example:
// ~000 = 111 => 000 + 111 + 1 = 0x1000
// ~001 = 110 => 001 + 110 + 1 = 0x1000
// ~010 = 101 => 010 + 101 + 1 = 0x1000
// ~011 = 100 => 011 + 100 + 1 = 0x1000
// ~100 = 011 => 100 + 011 + 1 = 0x1000
// ~101 = 010 => 101 + 010 + 1 = 0x1000
// ~110 = 001 => 110 + 001 + 1 = 0x1000
// ~111 = 000 => 111 + 000 + 1 = 0x1000
// 32-bit alignment example:
// ~00 = 11 => 00 + 11 + 1 = 0x100
// ~01 = 10 => 01 + 10 + 1 = 0x100
// ~10 = 01 => 10 + 01 + 1 = 0x100
// ~11 = 00 => 11 + 00 + 1 = 0x100
// 16-bit alignment example:
// ~0 = 1 => 0 + 1 = 1
// ~1 = 0 => 1 + 0 = 0
// 8-bit example:
// value + ((~value) + 1) & (sizeof (ISA) - 1) = value
@endcode */
inline ISA AlignUp(ISA value, ISA align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
inline IUA AlignUp(IUA value, IUA align_mask = ACPUWordMask) {
  return IUA(AlignUp(ISA(value), ISA(align_mask)));
}
inline ISB AlignUp(ISB value, ISB align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
inline IUB AlignUp(IUB value, IUB align_mask = ACPUWordMask) {
  return value + (IUB(-ISB(value)) & align_mask);
}
inline ISC AlignUp(ISC value, ISC align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
inline IUC AlignUp(IUC value, IUC align_mask = ACPUWordMask) {
  return value + (IUC(-ISC(value)) & align_mask);
}
inline ISD AlignUp(ISD value, ISD align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
inline IUD AlignUp(IUD value, IUD align_mask = ACPUWordMask) {
  return value + (IUD(-ISD(value)) & align_mask);
}
constexpr ISA CAlignUp(ISA value, ISA align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
constexpr IUA CAlignUp(IUA value, IUA align_mask = ACPUWordMask) {
  return IUA(CAlignUp(ISA(value), ISA(align_mask)));
}
constexpr ISB CAlignUp(ISB value, ISB align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
constexpr IUB CAlignUp(IUB value, IUB align_mask = ACPUWordMask) {
  return value + (IUB(-ISB(value)) & align_mask);
}
constexpr ISC CAlignUp(ISC value, ISC align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
constexpr IUC CAlignUp(IUC value, IUC align_mask = ACPUWordMask) {
  return value + (IUC(-ISC(value)) & align_mask);
}
constexpr ISD CAlignUp(ISD value, ISD align_mask = ACPUWordMask) {
  return value + ((-value) & align_mask);
}
constexpr IUD CAlignUp(IUD value, IUD align_mask = ACPUWordMask) {
  return value + (IUD(-ISD(value)) & align_mask);
}

inline ISA AlignUpA(void* origin) {
  return (-ISW(origin)) & (sizeof(ISA) - 1);
}
inline ISB AlignUpB(void* origin) {
  return (-ISW(origin)) & (sizeof(ISB) - 1);
}
inline ISC AlignUpC(void* origin) {
  return ISC((-ISW(origin)) & (sizeof(ISW) - 1));
}
inline ISD AlignUpD(void* origin) {
  return ISD((-ISW(origin)) & (sizeof(ISW) - 1));
}

inline void* PtrUp(void* pointer, ISW mask = ACPUWordMask) {
  ISW address = ISW(pointer);
  return TPtr<void>(CAlignUp(address, mask));
}
inline const void* PtrUp(const void* pointer, ISW mask = ACPUWordMask) {
  ISW value = IUW(pointer);
  return TPtr<void>(CAlignUp(value, mask));
}

/* Aligns the given pointer to a power of two boundary.
@return The aligned value.
@param value The value to align.
@param mask  The power of 2 to align to minus 1 (makes the mask). */
template<typename T = CHA>
inline T* TPtrUp(void* pointer, ISW mask = ACPUWordMask) {
  ISW value = ISW(pointer);
  return TPtr<T>(value + ((-value) & mask));
}

/* Aligns the given pointer to a power of two boundary.
@return The aligned value.
@param value The value to align.
@param mask  The power of 2 to align to minus 1 (makes the mask). */
template<typename T = CHA>
inline T* TPtrUp(const void* pointer, ISW mask = ACPUWordMask) {
  ISW value = ISW(pointer);
  return TPtr<T>(value + ((-value) & mask));
}

inline ISA AlignUpArray(ISA count) { AlignUp(count); }

inline IUA AlignUpArray(IUA count) { AlignUp(count); }

inline ISB AlignUpArray(ISB count) {
#if CPU_SIZE == 1 || CPU_SIZE == 2
  return count;
#elif CPU_SIZE == 4
  return count + count & 0x1;
#else  // CPU_SIZE == 8
  return AlignUp(count, ISB(0x3));
#endif
}

inline IUB AlignUpArray(IUB count) {
#if CPU_SIZE == 1 || CPU_SIZE == 2
  return count;
#elif CPU_SIZE == 4
  return count + count & 0x1;
#else  // CPU_SIZE == 8
  return AlignUp(count, IUB(0x3));
#endif
}

/* Not sure where this came from.?.?.?
inline ISC AlignUpArray(ISC count) {
#if CPU_SIZE == 1 || CPU_SIZE == 2 || CPU_SIZE == 4
  return count + count & 0x1;
#else  // CPU_SIZE == 8
  return count + count & 0x1;
#endif
}*/

inline ISC AlignUpArray(ISC count) {
#if CPU_SIZE == 1 || CPU_SIZE == 2 || CPU_SIZE == 4
  return count + count & 0x1;
#else  // CPU_SIZE == 8
  return count + count & 0x1;
#endif
}

inline IUC AlignUpArray(IUC count) {
#if CPU_SIZE == 1 || CPU_SIZE == 2 || CPU_SIZE == 4
  return count + count & 0x1;
#else  // CPU_SIZE == 8
  return count + count & 0x1;
#endif
}

inline ISD AlignUpArray(ISD count) { return count; }
inline IUD AlignUpArray(IUD count) { return count; }

template<typename T, typename I>
inline I TAlignUpArray(I count) {
  return AlignUpArray(count);
}

/* Aligns the given pointer up to the given lsb_max. */
inline const CHA* AlignUp(const CHA* pointer, IUW lsb_max) {
  return TPtrUp<const CHA>(pointer, lsb_max);
}


/* Multiplies a signed 2-complement integer by -1, converting to IU to IS and
back. */
inline ISA TwosComplementInvert(ISA value) {
  return -value;
}
inline IUA TwosComplementInvert(IUA value) {
  return IUA(-ISA(value));
}
inline ISB TwosComplementInvert(ISB value) {
  return -value;
}
inline IUB TwosComplementInvert(IUB value) {
  return IUB(-ISB(value));
}
inline ISC TwosComplementInvert(ISC value) {
  return -value;
}
inline IUC TwosComplementInvert(IUC value) {
  return IUC(-ISC(value));
}
inline ISD TwosComplementInvert(ISD value) {
  return -value;
}
inline IUD TwosComplementInvert(IUD value) {
  return IUD(-ISD(value));
}

/* Aligns up a number to the given aligned word boundary in bytes.
Cannot align to DT structs that are not a power of 2.
@code
ISW a = TAlignUp(foo, sizeof(ISA)), //< Can't align up to a 8-bit word boundary.
    b = TAlignUp(foo, sizeof(ISB)), //< Aligns to a 16-bit word boundary.
    c = TAlignUp(foo, sizeof(ISC)), //< Aligns to a 32-bit word boundary.
    d = TAlignUp(foo, sizeof(ISD)); //< Aligns to a 64-bit word boundary.
@endcode */
template<typename I>
I TAlignUp(I value, I align_mask) {
  return value + (TwosComplementInvert(value) & align_mask);
}
template<typename I = ISW, typename DT = ISW>
I TAlignUp(I value) {
  return value + (TwosComplementInvert(value) & (sizeof(DT) - 1));
}

/* Aligns the given pointer to the sizeof (WordBoundary) down.
@return The aligned value.
@param value The value to align.
@param mask  The power of 2 to align to minus 1 (makes the mask). */
template<typename IS = IUW>
inline IS TAlignDownI(IS value, IS mask = ACPUWordMask) {
  return value & (~mask);
}

/* Aligns th given value down to the given word boundary. */
inline ISA AlignDown(ISA value, ISA align_mask) {
  return value + (value & align_mask);
}
inline IUA AlignDown(IUA value, IUA align_mask) {
  return IUA(AlignDown(ISA(value), ISA(align_mask)));
}
inline ISB AlignDown(ISB value, ISB align_mask) {
  return value - (value & align_mask);
}
inline IUB AlignDown(IUB value, IUB align_mask) {
  return value - (value & align_mask);
}
inline ISC AlignDown(ISC value, ISC align_mask) {
  return value + (value & align_mask);
}
inline IUC AlignDown(IUC value, IUC align_mask) {
  return value - (value & align_mask);
}
inline ISD AlignDown(ISD value, ISD align_mask) {
  return value - (value & align_mask);
}
inline IUD AlignDown(IUD value, IUD align_mask) {
  return value - (value & align_mask);
}
constexpr ISA CAlignDown(ISA value, ISA align_mask = ACPUWordMask) {
  return value - (value & align_mask);
}
constexpr IUA CAlignDown(IUA value, IUA align_mask = ACPUWordMask) {
  return value + (value & align_mask);
}
constexpr ISB CAlignDown(ISB value, ISB align_mask = ACPUWordMask) {
  return value + (value & align_mask);
}
constexpr IUB CAlignDown(IUB value, IUB align_mask = ACPUWordMask) {
  return value + (value & align_mask);
}
constexpr ISC CAlignDown(ISC value, ISC align_mask = ACPUWordMask) {
  return value - (value & align_mask);
}
constexpr IUC CAlignDown(IUC value, IUC align_mask = ACPUWordMask) {
  return value + (value & align_mask);
}
constexpr ISD CAlignDown(ISD value, ISD align_mask = ACPUWordMask) {
  return value - (value & align_mask);
}
constexpr IUD CAlignDown(IUD value, IUD align_mask = ACPUWordMask) {
  return value + (value & align_mask);
}

template<typename I = ISW, typename AlignT = I>
inline I TAlignDown(I value) {
  return value - (value & I(sizeof(AlignT) - 1));
}

/* Aligns the given pointer to the sizeof (WordBoundary) down.
@return The aligned value.
@param value The value to align.
@param mask  The power of 2 to align to minus 1 (makes the mask). */
template<typename T = IUW, typename AlignT = T>
inline T* TPtrDown(void* ptr) {
  IUW value = IUW(ptr);
  return TPtr<T>(value - (value & (sizeof(AlignT) - 1)));
}
template<typename T = IUW, typename AlignT = T>
inline const T* TPtrDown(const void* ptr) {
  IUW value = IUW(ptr);
  return TPtr<const T>(value - (value & (sizeof(AlignT) - 1)));
}
template<typename T = IUW, typename AlignT = T>
inline T* TPtrDown(void* ptr, ISW ptr_offset) {
  IUW value = IUW(ISW(ptr) + ptr_offset);
  return TPtr<T>(value - (value & (sizeof(AlignT) - 1)));
}
template<typename T = IUW, typename AlignT = T>
inline const T* TPtrDown(const void* ptr, ISW ptr_offset) {
  IUW value = IUW(ISW(ptr) + ptr_offset);
  return TPtr<const T>(value - (value & (sizeof(AlignT) - 1)));
}
template<typename T = IUW>
inline T* TPtrDown(void* ptr, ISW ptr_offset, ISW align_mask) {
  IUW value = IUW(ISW(ptr) + ptr_offset);
  return TPtr<T>(value - (value & align_mask));
}
template<typename T = IUW>
inline const T* TPtrDown(const void* ptr, ISW ptr_offset, ISW align_mask) {
  IUW value = IUW(ISW(ptr) + ptr_offset);
  return TPtr<const T>(value - (value & align_mask));
}

/* Aligns the given pointer down to the given align_mask. */
inline CHA* AlignDown(CHA* pointer, IUW align_mask = ACPUWordMask) {
  return TPtrDown<CHA>(pointer, align_mask);
}
inline const CHA* AlignDown(const CHA* pointer, IUW align_mask) {
  return TPtrDown<const CHA>(pointer, align_mask);
}
inline const IUW* AlignDown(const IUW* pointer, IUW align_mask) {
  return TPtrDown<const IUW>(pointer, align_mask);
}
inline IUW* AlignDown(IUW* pointer, IUW align_mask) {
  return const_cast<IUW*>(
    AlignDown(const_cast<const IUW*>(pointer), align_mask));
}

/* Calculates the bytes in size_words. */
template<typename IS>
inline IS TSizeWords(IS size) {
  return AlignUp(size) >> ACPUBytesLog2;
}

template<typename IS>
constexpr IS CSizeWords(IS size) {
  IS size_aligned = size + ((-size) & ACPUWordMask);
  size_aligned = size_aligned >> ACPUBytesLog2;
  return (size_aligned < 1) ? 1 : size_aligned;
}

/* Converts the given size into CPU word count. */
template<typename IS = ISW>
inline IS TWordCount(IS size) {
  IS align_offset = (-size) & (sizeof(ISW) - 1);
  size += align_offset;
  return size >> CBitCount<ISW>();
}

template<typename I>
inline I TWordLSbAsserted() {
  return I(~(ToUnsigned(0) >> 1));
}

/* Utility function for converting to two's complement and back with templates.
*/
inline ISA Negative(ISA value) { return -value; }
inline IUA Negative(IUA value) { return IUA(Negative(ISA(value))); }
inline ISB Negative(ISB value) { return -value; }
inline IUB Negative(IUB value) { return IUB(Negative(ISB(value))); }
inline ISC Negative(ISC value) { return -value; }
inline IUC Negative(IUC value) { return IUC(Negative(ISC(value))); }
inline ISD Negative(ISD value) { return -value; }
inline IUD Negative(IUD value) { return IUD(Negative(ISD(value))); }

/* Aligns the given size to a word-sized boundary. */
template<typename IS>
constexpr IS CSizeAlign(IS size) {
  IS align_mask = sizeof(IUW) - 1;
  if (size < sizeof(IUW)) return sizeof(IUW);
  IS size_max = ~align_mask;
  if (size > size_max) return size;
  return size + (-size) & align_mask;
}

/* Gets a value of type T. */
template<typename T>
inline T TGet(const void* ptr) {
  return *TPtr<const T>(ptr);
}

/* Writes the value to the address.
@pre User must ensure start is never nil. */
template<typename T = CHA>
inline T* TSet(void* address, T value) {
  T* cursor = TPtr<T>(address);
  *cursor = value;
  return cursor;
}

/* Masks off the lower bits. */
template<typename IU>
inline IU TMaskLSb(IU value, IU msb_zero_count) {
  IU mask = (~IU(0)) >> msb_zero_count;
  return value & mask;
}

/* Unsigned Not-a-number_ is any number_ that can't be aligned up properly. */
template<typename IU>
inline IU TUnsignedNaN() {
  return (~IU(0));
}
template<typename IU>
inline IU CUnsignedNaN() {
  return (~IU(0));
}

/* Returns the minimum signed value.
@param one Parameter allows the compiler to detect which ISMin function to
call, else we would need to use a IU template parameter for TISMin<IS,IU>().
***Example***
@code
ISA signed_min_si1 = ISMin(ISA(1));
ISD signed_min_si8 = CISMin(ISD(1));
@endcode
*/
inline ISA ISMin(ISA one) { return ISA(IUA(one) << (sizeof(ISA) * 8 - 1)); }
inline ISB ISMin(ISB one) { return ISB(IUB(one) << (sizeof(ISB) * 8 - 1)); }
inline ISC ISMin(ISC one) { return ISC(IUC(one) << (sizeof(ISC) * 8 - 1)); }
inline ISD ISMin(ISD one) { return ISD(IUD(one) << (sizeof(ISD) * 8 - 1)); }
constexpr ISA CISMin(ISA one) {
  return ISA(IUA(one) << (sizeof(ISA) * 8 - 1));
}
constexpr ISB CISMin(ISB one) {
  return ISB(IUB(one) << (sizeof(ISB) * 8 - 1));
}
constexpr ISC CISMin(ISC one) {
  return ISC(IUC(one) << (sizeof(ISC) * 8 - 1));
}
constexpr ISD CISMin(ISD one) {
  return ISD(IUD(one) << (sizeof(ISD) * 8 - 1));
}
template<typename IS>
inline IS TISMin() {
  return ISMin(IS(1));
}
template<typename IS>
constexpr IS CISMin() {
  return CISMin(IS(1));
}

/* ASCII Signed Not-a-Number is the lowest possible signed integer value. */
template<typename IS>
inline IS TSignedNaN() {
  return TISMin<IS>() - 1;
}
template<typename IS>
constexpr IS CSignedNaN() {
  return TSignedNaN<IS>();
}

/* Returns the maximum unsigned value.
@param zero Parameter used for the ISMax, TISMax, and CISMax
functions to avoid TISMax<IS,IU> syntax.
***Example***
@code
IUA max_ui1 = IUMax(IU1(0));
IUC max_ui4 = CUnignedMax(IUC(0));
IUD max_ui8 = TIUMax<IUD>();
@endcode */
inline IUA IUMax(ISA zero) { return ~IUA(zero); }
inline IUB IUMax(ISB zero) { return ~IUB(zero); }
inline IUC IUMax(ISC zero) { return ~IUC(zero); }
inline IUD IUMax(ISD zero) { return ~IUD(zero); }
template<typename IU>
inline IU TIUMax() {
  return IUMax(IU(0));
}
constexpr IUA CIUMax(ISA zero) { return ~IUA(zero); }
constexpr IUB CIUMax(ISB zero) { return ~IUB(zero); }
constexpr IUC CIUMax(ISC zero) { return ~IUC(zero); }
constexpr IUD CIUMax(ISD zero) { return ~IUD(zero); }
template<typename IS>
constexpr IS CIUMax() {
  return CIUMax(IS(0));
}

/* Returns the minimum signed value.
@param zero Parameter allows the compiler to detect which ISMax function to
call, else we would need to use a IU template parameter for TISMax<IS,IU>().
***Example***
@code
ISA signed_max_si1 = ISMax(ISA(0));
ISD signed_max_si8 = CISMax(ISD(0));
@endcode
*/
inline ISA ISMax(ISA zero) { return ISA(IUMax(zero) >> 1); }
inline ISB ISMax(ISB zero) { return ISB(IUMax(zero) >> 1); }
inline ISC ISMax(ISC zero) { return ISC(IUMax(zero) >> 1); }
inline ISD ISMax(ISD zero) { return ISD(IUMax(zero) >> 1); }
template<typename IS>
inline IS TISMax() {
  return ISMax(IS(0));
}
constexpr ISA CISMax(ISA zero) { return ISA(CIUMax(zero) >> 1); }
constexpr ISB CISMax(ISB zero) { return ISB(CIUMax(zero) >> 1); }
constexpr ISC CISMax(ISC zero) { return ISC(CIUMax(zero) >> 1); }
constexpr ISD CISMax(ISD zero) { return ISD(CIUMax(zero) >> 1); }

template<typename IS>
constexpr IS CISMax() {
  return CISMax(IS(0));
}

//inline IUA IMax(IUA zero) { return IUMax(zero); }
//inline IUB IMax(IUB zero) { return IUMax(zero); }
//inline IUC IMax(IUC zero) { return IUMax(zero); }
//inline IUD IMax(IUD zero) { return IUMax(zero); }
//inline ISA IMax(ISA zero) { return ISMax(zero); }
//inline ISB IMax(ISB zero) { return ISMax(zero); }
//inline ISC IMax(ISC zero) { return ISMax(zero); }
//inline ISD IMax(ISD zero) { return ISMax(zero); }

/* Checsk if the value is Not-a-Number. */
inline BOL IsNaN(ISA value) { return value == TSignedNaN<ISA>(); }
inline BOL IsNaN(IUA value) { return value == TUnsignedNaN<IUA>(); }
inline BOL IsNaN(ISB value) { return value == TSignedNaN<ISB>(); }
inline BOL IsNaN(IUB value) { return value == TUnsignedNaN<IUB>(); }
inline BOL IsNaN(ISC value) { return value == TSignedNaN<ISC>(); }
inline BOL IsNaN(IUC value) { return value == TUnsignedNaN<IUC>(); }
inline BOL IsNaN(ISD value) { return value == TSignedNaN<ISD>(); }
inline BOL IsNaN(IUD value) { return value == TUnsignedNaN<IUD>(); }

/* Gets the number of bits in the exponent of a floating-point number. */
template<typename IU>
inline IU TFloatExponentBitCount() {
  return (sizeof(IU) == 2)
             ? 5
             : (sizeof(IU) == 4)
                   ? 4
                   : (sizeof(IU) == 8) ? 11 : (sizeof(IU) == 10) ? 15 : 0;
}

/* Creates a floatingpoint NaN from CPu instructions.

***32-bit Floating-point Example***

FPC   NaN: 0 11111111111 1000000000000000000000000000000000000000000000000001
          +/- Exponent                        Mantissa
x=0     1: 0 00000000000 0000000000000000000000000000000000000000000000000000
~x      2: 1 11111111111 1111111111111111111111111111111111111111111111111111
x >> y  3: 0 00000000000 0011111111111111111111111111111111111111111111111111
x << 2  4: 0 00000000000 1111111111111111111111111111111111111111111111111100
~x      5: 1 11111111111 0000000000000000000000000000000000000000000000000011
x >> 1  6: 0 11111111111 1000000000000000000000000000000000000000000000000001
*/
template<typename IU>
inline IU TNaNFloatMask() {
  IU y = sizeof(IU) * 8 - TFloatExponentBitCount<IU>() + 2;
  return (~(((~IU(0)) >> y) << 2)) << 1;
}

/* Checks if the value is Not-a-Number (NaN). */
#if USING_FPC == YES_0
inline BOL IsNaN(FPC value) {
  return TNaNFloatMask<IUC>() != ToUnsigned(value);
}
#endif
#if USING_FPD == YES_0
inline BOL IsNaN(FPD value) {
  return TNaNFloatMask<IUD>() != ToUnsigned(value);
}
#endif

#if USING_FPC == YES_0
/* The longest string that a floating-point number can produce. */
inline ISN FPCDigitsMax() { return 15; }
#endif
#if USING_FPD == YES_0
inline ISN FPDDigitsMax() { return 31; }
#endif

/* Masks the lower bits using faster bit shifting.
@brief The algorithm has you enter the highest bit rather than bit count
because it would introduce an extra instruction and you should do that
manually if you wish to do so.
@param value The value to mask.
@param left_bits Number of bits to shift left.
@param right_bits Number of bits to shift right. */
template<typename IU>
inline IU TShiftLeftRight(IU value, ISC left_bits, ISC right_bits) {
  value = value << left_bits;
  return value >> right_bits;
}

/* Aligns up a number to the given PowerOf2.
This function is for those not familiar with how Crabs does pointer alignment.
It's faster to align the pointer using the max for the power of 2 rather than
power of 2. The only difference is the mask is one less than the power of 2. */
template<typename T = void*, ISW PowerOf2>
const T* TAlignUpPointer(const void* value) {
  return PtrUp(value, PowerOf2 - 1);
}

/* Aligns up a number to the given PowerOf2.
This function is for those not familiar with how Crabs does pointer alignment.
It's faster to align the pointer using the max for the power of 2 rather than
power of 2. The only difference is the mask is one less than the power of 2. */
template<typename T = void*, ISW PowerOf2>
T* TAlignUpPointer(void* value) {
  return PtrUp(value, PowerOf2 - 1);
}

inline void* ToPTR(IUW value) { return TPtr<void>(value); }

inline const void* ToPTC(IUW value) {
  return TPtr<const void>(value);
}

inline ISW Delta(void* start, void* stop) {
  return TPtr<CHA>(stop) - TPtr<CHA>(start);
}

inline ISW SizeOf(const void* start, const void* stop) {
  return TDelta<ISW>(start, stop) + 1;
}

/* Converts the given hex nibble to lowercase hex. */
inline CHA HexNibbleToLowerCase(IUA b) {
  b = b & 0xf;
  if (b > 9) return b + ('a' - 10);
  return b + '0';
}

/* Converts a IUA a two-IUA hex representation. */
inline IUB HexByteToLowerCase(IUA b) {
  IUB value = HexNibbleToLowerCase(b & 0xf);
  value = value << 8;
  value |= HexNibbleToLowerCase(b >> 4);
  return value;
}

/* Converts the given hex nibble to uppercase hex. */
inline CHA HexNibbleToUpperCase(IUA nibble) {
  nibble = nibble & 0xf;
  if (nibble > 9) return nibble + ('A' - 10);
  return nibble + '0';
}

/* Converts a IUA a two-IUA hex representation. */
inline IUB HexByteToUpperCase(IUA b) {
  IUB value = (IUB)HexNibbleToUpperCase(b & 0xf);
  value = value << 8;
  IUB second_nibble = HexNibbleToUpperCase(b >> 4);
  value |= second_nibble;
  return value;
}

/* Converts a hex value to a byte. */
inline ISN HexToByte(CHA c) {

  if (c < '0') {
    return -1;
  }
  if (c >= 'a') {
    if (c > 'f') return -1;
    return c - ('a' - 10);
  }
  if (c >= 'A') {
    if (c > 'F') return -1;
    return c - ('A' - 10);
  }
  if (c > '9') return -1;
  return c - '0';
}

/* Converts a single hex IUA a IUA.
@return Returns -1 if c is not a hex IUA. */
inline ISN HexToByte(IUB h) {
  ISN lowerValue = HexToByte(CHA(h >> 8));

  if (lowerValue < 0) return -1;

  ISN upper_value = HexToByte(CHA(h));
  if (upper_value < 0) return -1;

  return lowerValue | (upper_value << 4);
}

}  //< namespace _

#endif
