// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBSeqHPP
#define _CrabsBSeqHPP
#include "BSeq.h"
#if _Seam >= _CrabsList
#if _Seam == _CrabsList
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

/* Utility class for printing B-Sequences. */
template<typename ISY = ISN, typename DT = DTB>
struct TBSeq {
  const DT* params;  //< BSeq params.

  TBSeq(const DT* params = _NilP) : params(params) {}

  constexpr const DT* CParams() { return params; }
};

enum {
  BSQMax = 420   //< Max number of elements in a BSQ.
};

#if _CPUSize == _CPU16Bit
inline ISB UnpackISV(ISB value) {
  if (value < 0) {
    IUB result = 0x8000;
    return result | ~(value - 1);
  }
  return value;
}

inline ISB PackISV(ISB value) {
  if (value < 0) {
    IUB result = 1 << 15;
    return result | ((~value + 1) << 1);
  }
  return value;
}
#elif _CPUSize == _CPU32Bit
#elif _CPUSize == _CPU8Byte
#endif

/* Creates a immutable Type Sequence (TSQ).
C++11 variadic template to ensure only one copy in ROM and to eliminate some 
redundant typing. */
template<const DTB... N>
constexpr const DTB* TTSQ() {
  static constexpr DTB Values[] = {DTB(sizeof...(N)), N...};
  return Values;
}

/* Utility class for making allowed and disallowed char sequences. */
template<const CHR... N>
constexpr const CHR* CCHRSQ() {
  static constexpr CHR Values[] = {CHR(sizeof...(N)), N...};
  return Values;
}

/* Writes the given value to the socket between begin and end without any checks.
@warning You must memory align begin and end and verify the buffer fits before
calling. */
template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTB type, IUW value, 
                    IUW value_msb = 0) {
  _DROut(" value:" << value << " value_msb:" << value_msb);
  // | b15 | b14:b13 | b12:b9 | b8:b7 | b6:b5 | b4:b0 |
  // |:---:|:-------:|:------:|:-----:|:-----:|:-----:|
  // | CNS |    MD   |   MT   |  SW   |  VT   |  POD  |
  type &= DTB(-1) >> 1;
  const DTB MD = type >> ATypeMDBit0;
  type ^= MD << ATypeMDBit0;

  ISW freespace = ISW(end) - ISW(begin);
  if (type <= _CHA) {
   Write1Byte:
    auto pointer = TRef<IUA>(begin);
    if (freespace < 1) return _NilP;
    *pointer++ = MD ? *TRef<IUA>(value)
                : IUA(value);
    return pointer;
  }
  if (type <= _CHB) {
   Write2Bytes:
    if (freespace < 2) return _NilP;
    auto pointer = TRef<IUB>(begin);
    *pointer++ = MD ? *TRef<IUB>(value)
                : IUB(value);
    return pointer;
  }
  if (type <= _CHC) {
   Write4Bytes:
    if (freespace < 4) return _NilP;
    auto pointer = TRef<IUC>(begin);
    *pointer++ = MD ? *TRef<IUC>(value)
                : IUC(value);
    return pointer;
  }
  if (type <= _SSD) {
   Write8Bytes:
    if (freespace < 8) return _NilP;
    auto pointer = TRef<IUD>(begin);
    *pointer++ = MD ? *TRef<IUD>(value)
                : IUD(value);
    return pointer;
  }
  if (type <= _SSE) {
   Write16Bytes:
    if (freespace < 16) return _NilP;
    auto dcursor = TRef<IUW>(begin);
    if (MD) {
      auto vcursor = TRef<IUW>(value);
      *dcursor++ = *vcursor++;
      #if _CPUSize == _CPU16Bit
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      #elif _CPUSize == _CPU32Bit
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      *dcursor++ = *vcursor++;
      #endif
      return dcursor;
    }
#if _CPUSize == _CPU8Byte
    *dcursor++ = value;
    *dcursor++ = value_msb;
#endif
    return dcursor;
  }
  if (type <= _ECE) goto Write16Bytes;
  if (type <= _ECD) goto Write8Bytes;
  if (type <= _ECC) goto Write4Bytes;
  if (type <= _ECB) goto Write2Bytes;
  if (type <= _ECA) goto Write1Byte;

  DTB mt = type >> ATypeMTBit0;
  type ^= mt << ATypeMTBit0;
  DTB sw = type >> ATypeSWBit0;
  type ^= sw << ATypeSWBit0;
  DTB vt = type >> ATypeVTBit0;
  type ^= vt << ATypeVTBit0;

  //@todo Fix me!
  if (vt == 0) { // Vector of Homotuples.
    auto bytes = (1 << vt) * ATypeSizeOfPOD(type);
    return TRef<ISA>(begin) + bytes;
  }
  else {
    /* Write varint length as VUC (32-bit unsigned varint).
     * Each byte: bit 7 = continuation (0=last byte), bits 0-6 = data. */
    IUC length = IUC(value);
    /* Calculate how many varint bytes this length needs. */
    IUC tmp = length;
    IUC varint_bytes = 1;
    while (tmp > 0x7F) {
      tmp >>= 7;
      ++varint_bytes;
    }
    if (freespace < varint_bytes) return _NilP;
    IUA* pointer = TRef<IUA>(begin);
    for (IUC i = 0; i < varint_bytes; ++i) {
      IUA byte = IUA(length & 0x7F);
      length >>= 7;
      if (length > 0) byte |= 0x80;
      *pointer++ = byte;
    }
    return pointer;
  }
  return _NilP;
}
template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTB type, const void* value) {
  return TBSeqWrite_NC<IS>(begin, end, type, IUW(value));
}

template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTC type, IUW value) {
  auto result = TBSeqWrite_NC<IS>(begin, end, DTB(type), value);
  if (IsError(result)) return result;
  return TBSeqWrite_NC<IS>(result, end, DTB(type >> 16), value);
}
template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTC type, const void* value) {
  return TBSeqWrite_NC<IS>(begin, end, type, IUW(value));
}

template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTD type, IUW value) {
  auto result = TBSeqWrite_NC<IS>(begin, end, DTB(type), value);
  if (IsError(result)) return result;
  result = TBSeqWrite_NC<IS>(result, end, DTB(type >> 16), value);
  if (IsError(result)) return result;
  return TBSeqWrite_NC<IS>(result, end, DTB(type >> 24), value);
}
template<typename IS = ISR>
void* TBSeqWrite_NC(void* begin, void* end, DTD type, const void* value) {
  return TBSeqWrite_NC<IS>(begin, end, type, IUW(value));
}

template<typename IS = ISR>
void* TBSeqWrite(void* begin, void* end, DTB type, const void* value) {
  return TBSeqWrite<IS>(begin, end, type, value);
}

template<typename IS = ISR>
void* TBSeqWrite(void* begin, void* end, DTC type, const void* value) {
  auto result = TBSeqWrite<IS>(begin, end, DTB(type), value);
  if (IsError(result)) return result;
  return TBSeqWrite<IS>(result, end, DTB(type >> 16), value);
}

template<typename IS = ISR>
void* TBSeqWrite(void* begin, void* end, DTD type, const void* value) {
  auto result = TBSeqWrite<IS>(begin, end, DTB(type), value);
  if (IsError(result)) return result;
  result = TBSeqWrite<IS>(result, end, DTB(type >> 16), value);
  if (IsError(result)) return result;
  return TBSeqWrite<IS>(result, end, DTB(type >> 24), value);
}

/* Prints a BSQ to the Printer. */
template<typename Printer>
Printer& TBSeqPrint(Printer& o, const DTB* params) {
  if (IsError(params)) {
    return o;
  }
  /* Read param_count as VUC (32-bit unsigned varint).
   * Each byte: bit 7 = continuation, bits 0-6 = data. */
  IUC param_count = 0;
  IUC shift = 0;
  DTB byte;
  do {
    byte = *params++;
    param_count |= IUC(byte & 0x7F) << shift;
    shift += 7;
  } while (byte & 0x80);

  ISN i     = 0;
  DTB type  = 0,
      value = 0;
  if (param_count == 0) {
    return o << "Param<NIL>";
  }
  o << "Param<";
  if (param_count > BSQMax) {
    o << "\nInvalid param_count: " << param_count << '>';
    return o;
  }
  o << param_count << ": ";
  for (i = 1; i < param_count; ++i) {
    value = *params++;
    //type = value & 0x1f;  //< Mask off type.
    //value = value >> 5;   //< Shift over array type.
    o << ATypef(value) << ", ";
  }
  // Do the last set without a comma.
  value = *params++;
  return o << ATypef(value) << '>';
}

}  //< namespace _

#endif  //< _Seam >= _CrabsOp
#endif  //< _CrabsBSeqHPP
