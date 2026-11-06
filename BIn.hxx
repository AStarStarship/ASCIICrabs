// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsBInHXX
#define _CrabsBInHXX
#include "BIn.hpp"
#if _Seam >= _CrabsOp
#include "BSeq.hpp"
#include "Hash.hpp"
#include "Varint.hpp"
#include "Slot.hpp"
#include "Op.h"
#if _Seam == _CrabsOp
#include "_Debug.h"
#else
#include "_Release.h"
#include "AType.hpp"
#endif
namespace _ {

IUA* BInEnd(TBIn<ISC>* bin) { return TRef<IUA>(bin) + bin->bytes; }

ISW SlotLength(IUA* origin, IUA* stop, IUW size) {
  return stop - origin;
}

ISW SlotSpace(IUA* origin, IUA* stop, IUW size) {
  return size - (stop - origin);
}

ISN BInSpace(TBIn<ISC>* bin) {
  IUA* txb_ref = TRef<IUA>(bin);
  return ISN(SlotSpace(txb_ref + bin->origin, txb_ref + bin->stop, bin->bytes));
}

ISN BinBufferLength(TBIn<ISC>* bin) {
  IUA* origin = BInOrigin(bin);
  return ISN(SlotLength(origin + bin->origin, origin + bin->stop, bin->bytes));
}

/* Used to return an erroneous result from a B-Output.
@param error The error type.
@return Returns a Static Error Op Result. */
inline const Op* BInError(TBIn<ISC>* bin, ERC error) {
  //_DROut("\nBIn " << TAErrors<>(error) << " error!");
  return OpError(error);
}

/* Used to return an erroneous result from a B-Output.
@param bin    The source TBIn.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Op Result. */
inline const Op* BInError(TBIn<ISC>* bin, ERC error, const ISN* header) {
  //_DROut("\nBIn %s error!" << TAErrors<>(error));
  return OpError(error);
}

/* Used to return an erroneous result from a B-Output.
@param bin    The source TBIn.
@param error   The error type.
@param header  The B-Sequence Header.
@param offset  The offset to the type in error in the B-Sequence.
@param address The address of the IUA in error.
@return         Returns a Static Error Op Result. */
inline const Op* BInError(TBIn<ISC>* bin, ERC error, const DTB* header,
                          DTB offset) {
  //_DROut("\nBIn " << TAErrors<>(error) << " error!");
  return OpError(error);
}

/* Used to return an erroneous result from a B-Output.
@param bin        The source TBIn.
@param error      The error type.
@param header     The B-Sequence Header.
@param bsq_error  The offset to the type in error in the B-Sequence.
@param error_byte The address of the IUA in error.
@return Returns a Static Error Op Result. */
inline const Op* BInError(TBIn<ISC>* bin, ERC error, const DTB* header, DTB bsq_error,
                          IUA* error_byte) {
  //_DROut("\nBIn " << TAErrors<>(error) << " error at " <<
  //       TDelta<>(BInOrigin(bin), error_byte));
  return OpError(error);
}

TBIn<ISC>* BInInit(TBIn<ISC>* bin, ISN buffer_bytes) {
  _DAssert(buffer_bytes >= SlotBytesMin);
  _DAssertPointer(bin);
  bin->bytes = buffer_bytes - sizeof(TBIn<ISC>);
  bin->origin = 0;
  bin->stop = 0;
  bin->read = 0;
  _DArrayFill(BInOrigin(bin), buffer_bytes);
  return bin;
}

ISN BInStreamByte(TBIn<ISC>* bin) {
  IUA* begin  = BInOrigin(bin),
     * end    = begin + bin->bytes - 1,
     * read   = begin + bin->read, 
     * origin = begin + bin->origin;
  ISN length = (ISN)((origin < read) ? read - origin + 1
                                     : (end - origin) + (read - origin) + 2);
  if (length < 1) {
    BInError(bin, AErrorBufferOverflow, TTSQ<_STR>(), 2, origin);
    return -1;
  }
  bin->stop = (++origin >= end) ? TDelta<ISN>(origin, end)
                                : TDelta<ISN>(origin, origin);
  return 0;
}

BOL BInIsReadable(TBIn<ISC>* bin) { return BinBufferLength(bin) > 0; }

const Op* BInRead(TBIn<ISC>* bin, const DTB* params, void** args, IUD pc_ctx) {
  //_DROut("\nReading ");
  //_DROutBSQ(params);
  //_DROut(" from B-Input:");
  //_DROutBIn(bin);

  if (IsError(bin)) {
    return BInError(bin, AErrorImplementation);
  }
  if (IsError(params)) {
    return BInError(bin, AErrorImplementation);
  }
  if (IsError(args)) {
    return BInError(bin, AErrorImplementation);
  }
  IUA  iua = 0;               //< Temp variable.
  IUB  iub = 0;               //< Temp variable.
  IUC  iuc = 0;               //< Temp variable.
  IUD  iud = 0;               //< Temp variable.
  CHA* iua_ref = 0;           //< Pointer to a _IUA.
  IUB* iub_ref = 0;           //< Pointer to a _IUB.
  IUC* iuc_ref = 0;           //< Pointer to a _IUC.
  IUD* iud_ref = 0;           //< Pointer to a _IUA.
  DTB  type = 0;              //< The current type being read.
  ISN  bytes = 0,             //< Size of the ring socket.
       length = 0,            //< Length of the data in the socket.
       count = 0,             //< Argument length.
       index = 0,             //< Index in the params.
       arg_index = 0,         //< Index in the args.
       num_params = *params;  //< Number of params.
  IUB  hash = 0;              //< Hash of the incoming data.

  if (num_params == 0) return 0;  //< Nothing to do.

  hash = _PrimeLargestIUB;
  bytes = bin->bytes;

  IUA* begin  = BInOrigin(bin),        //< The beginning of the socket.
     * end    = begin + bytes,         //< The end of the socket (sentinel).
     * origin = begin + bin->origin,   //< The read cursor (consumer head).
     * stop   = begin + bin->stop;     //< The write cursor (producer head).
  // const ISN* param = params + 1;   //< The current param.

  length = TSlotLength<ISN>(origin, stop, bytes);
  
  IUC temp = 0; //@todo This won't let me init this in the cases?

  // When we scan, we are reading from the beginning of the Slot socket.

  for (index = 1; index <= num_params; ++index) {
    type = params[index];
    //_DROut("\nparam:" << arg_index + 1 << " type:" << ATypef(type) <<
    //       " origin:" << TDelta<>(origin, origin) << " stop:" <<
    //       TDelta<>(origin, stop) << " length:" << length);
    if (type < 0) { // It's marked read-only, we don't care about the RO mod.
        type ^= 1 << ((sizeof(DTB) * 8) - 1);
    }
    DTB mod = (type >> sizeof(DTB)) - 3;
    if (mod == 0) {

    } else if (1) {
    }
    switch (type) {
      case _NIL:
        return BInError(bin, AErrorInvalidType, params, index, origin);
      case _ISA:  //< _R_e_a_d__1__B_y_t_e__T_y_p_e_s________________
      case _IUA:
      case _CHA:
#ifdef _CrabsUsing1ByteTypes
        if (length-- == 0)
          return BInError(bin, AErrorBufferUnderflow, params, index, origin);

        // Load next pointer and increment args.
        iua_ref = TRef<CHA>(args[arg_index]);
        if (IsError(iua_ref)) break;

        // Read type;

        // Byte 1
        iua = *origin;  //< Read
        //_DROut(" '" << iua << "', ");
        hash = HashIUB(iua, hash);  //< Same byte hash as BOutWrite.
        ++origin; if (origin >= end) origin = begin; //< Increment
        *iua_ref = iua;                        //< Write
        break;
#else
        return BInError(bin, AErrorInvalidType, params, index, origin);
#endif
      case _ISB:  //< _R_e_a_d__1_6_-_b_i_t__T_y_p_e_s_______________
      case _IUB:
      case _FPB:
      case _CHB:
#ifdef _CrabsUsing2ByteTypes
        if (length < 2)
          return BInError(bin, AErrorBufferUnderflow, params, index, origin);
        length -= 2;

        // Load next pointer and increment args.
        iua_ref = TRef<CHA>(args[arg_index]);
        if (IsError(iua_ref)) break;

        // Read type

        // Byte 1
        iua = *origin;                         //< Read
        hash = HashIUB(iua, hash);             //< Hash
        ++origin; if (origin >= end) origin = begin; //< Increment
        *iua_ref = iua;                        //< Write

        // Byte 2
        iua = *origin;                         //< Read
        hash = HashIUB(iua, hash);             //< Hash
        ++origin; if (origin >= end) origin = begin; //< Increment
        *(iua_ref + 1) = iua;                  //< Write
        break;
#else
        return BInError(bin, AErrorInvalidType, params, index, origin);
#endif
      case _ISC:  //< _R_e_a_d__3_2_-_b_i_t__T_y_p_e_s_______________
      case _IUC:
      case _FPC:
      case _CHC:
#ifdef _CrabsUsing4ByteTypes
        if (length < 4)
          return BInError(bin, AErrorBufferUnderflow, params, index, origin);
        length -= 4;

        // Load next pointer and increment args.
        iua_ref = TRef<CHA>(args[arg_index]);
        if (IsError(iua_ref)) break;

        // Read type

        for (ISN value = sizeof(IUC); value > 0; --value) {
          // Byte 1
          iua = *origin;                         //< Read
          hash = HashIUB(iua, hash);             //< Hash
          ++origin; if (origin >= end) origin = begin; //< Increment
          *iua_ref++ = iua;                      //< Write
        }
        break;
#else
        return BInError(bin, AErrorInvalidType, params, index, origin);
#endif
      case _ISD:  //< _R_e_a_d__6_4_-_b_i_t__T_y_p_e_s_______________
      case _IUD:
      case _FPD:
      case _SSD:
#ifdef _CrabsUsing8ByteTypes
        if (length < 8)
          return BInError(bin, AErrorBufferUnderflow, params, index, origin);
        length -= 8;

        // Load next pointer and increment args.
        iua_ref = TRef<CHA>(args[arg_index]);
        if (IsError(iua_ref)) break;

        // Read type
        for (ISN value = sizeof(IUD); value > 0; --value) {
          // Byte 1
          iua = *origin;                         //< Read
          hash = HashIUB(iua, hash);             //< Hash
          ++origin; if (origin >= end) origin = begin; //< Increment
          *iua_ref++ = iua;                      //< Write
        }
        break;
#else
        return BInError(bin, AErrorInvalidType, params, index, origin);
#endif
      case STR_: {
        if (index == num_params || params[index + 1] <= 0)
          return BInError(bin, AErrorInvalidType);
        count = params[++index];
        iua_ref = TRef<CHA>(args[arg_index]);
        if (IsError(iua_ref))
          return BInError(bin, AErrorImplementation);
        BOL terminated = false;
        for (ISN i = 0; i < count; ++i) {
          // Keep the two-byte hash trailer out of the string payload.
          if (length <= ISN(sizeof(IUB)))
            return BInError(bin, AErrorBufferUnderflow);
          --length;
          iua = *origin;
          hash = HashIUB(iua, hash);
          ++origin; if (origin >= end) origin = begin;
          *iua_ref++ = CHA(iua);
          if (!iua) {
            terminated = true;
            break;
          }
        }
        if (!terminated) return BInError(bin, AErrorBufferOverflow);
        break;
      }
      //case _ISV:  //< _R_e_a_d__S_i_g_n_e_d__V_a_r_i_n_t______________
      //case _IUV:  //< _R_e_a_d__U_n_s_i_g_n_e_d__V_a_r_i_n_t___________
#if _CPUSize <= 16
                 // Load next pointer and increment args.
        iub_ref = TRef<IUB>(args[arg_index]);
        if (iub_ref == _NilP)
          return BInError(bin, AErrorImplementation, params, index, origin);
        // SScan IUA 1.
        iua = *origin;
        ++origin; if (origin >= end) origin = begin;
        hash = HashIUB(iua, hash);
        iub = iua;
        //ISN temp = 7;   //< Number of bits to shift iua to the left.
        temp = 7;   //< Number of bits to shift iua to the left.
        count = 5;  //< The max number_ of Varint4 bytes.
        while (iua >> 7 == 0) {
          if (length-- == 0)
            return BInError(bin, AErrorBufferUnderflow, params, index, origin);
          iua = *origin;
          ++origin; if (origin >= end) origin = begin;
          hash = HashIUB(iua, hash);
          iuc |= IUC(iua & 0x7F) << temp;
          //< @todo I'm starting to second guess if we need to mask iua
          //< because we're packing them up and will overwrite.
          temp += 7;
          if (--count == 0)
            return BInError(bin, AErrorVarintOverflow, params, index, origin);
        }
        if (count == 5)  //< If there is only one IUA we need to
          iuc &= 0x7F;   //< mask off the terminating varint bit.
        if (type == _VSC) iuc = VSCUnpack(iuc);
        *iuc_ref = iuc;
        break;
#else
                 // Load next pointer and increment args.
        iuc_ref = TRef<IUC>(args[arg_index]);
        if (iuc_ref == _NilP)
          return BInError(bin, AErrorImplementation, params, index, origin);

        // SScan IUA 1.
        iua = *origin;
        ++origin; if (origin >= end) origin = begin;
        hash = HashIUB(iua, hash);
        iuc = iua;
        iub = 7;    //< Number of bits to shift iua to the left.
        count = 5;  //< The max number_ of Varint4 bytes.
        while (iua >> 7 == 0) {
          if (length-- == 0)
            return BInError(bin, AErrorBufferUnderflow, params, index, origin);
          iua = *origin;
          ++origin; if (origin >= end) origin = begin;
          hash = HashIUB(iua, hash);
          iuc |= ((IUC)(iua & 0x7F)) << iub;
          //< @todo I'm starting to second guess if we need to mask iua
          //< because we're packing them up and will overwrite.
          iub += 7;
          if (--count == 0)
            return BInError(bin, AErrorVarintOverflow, params, index, origin);
        }
        if (count == 5)  //< If there is only one IUA we need to
          iuc &= 0x7F;   //< mask off the terminating varint bit.
        if (type == SVI) iuc = TTypeUnpackVarint<IUC>(iuc);
        *iuc_ref = iuc;
        break;
#endif
      case _VSD:  //< _R_e_a_d__V_a_r_i_n_t__8____________________
      case _VUD:
        // Load next pointer and increment args.
        iud_ref = TRef<IUD>(args[arg_index]);
        if (IsError(iud_ref)) {
          return BInError(bin, AErrorImplementation, params, index, origin);
        }
        // SScan IUA 1.
        iua = *origin;
        ++origin; if (origin >= end) origin = begin;
        hash = HashIUB(iua, hash);
        iud = iua;
        iub = 7;    //< Number of bits to shift iua to the left.
        count = 9;  //< The max number_ of Varint8 bytes.
        while (iua >> 7 == 0) {
          if (length-- == 0)
            return BInError(bin, AErrorBufferUnderflow, params, index, origin);
          iua = *origin;
          ++origin; if (origin >= end) origin = begin;
          hash = HashIUB(iua, hash);
          if (count == 1) {
            // Varint 8 differs from Varint 2 and 4 in that on the
            // last IUA, IUA 9 of 9, there is no terminating
            // varint bit.
            iud |= IUD(iua) << iub;
            break;
          }
          iud |= IUD(iua & 0x7F) << iub;
          //< @todo I'm starting to second guess if we need to mask
          //< iua because we're packing them up and will overwrite.
          iub += 7;
          if (--count == 0)
            return BInError(bin, AErrorVarintOverflow, params, index, origin);
        }
        if (count == 9)  //< If there is only one IUA we need to
          iud &= 0x7F;   //< mask off the terminating varint bit.
        if (type == _VSD) iud = VSDUnpack(iud);
        *iud_ref = iud;
        break;
      case _BSQ:  //< _B_-_S_e_q_u_e_n_c_e__S_t_r_i_n_g______________
#if USING_BSQ
        iua_ref = TRef<CHA>(args[arg_index]);
        if (iua_ref == _NilP)
          return BInError(bin, AErrorImplementation, params, index, origin);
        iua = *origin;
#endif
      default: {
        //_DROut("\nIt's an array!\n");
#if USING_CRABS_ARRAY
        switch (type & 0x60) {
          case 0: {
            if ((type < _LST) && (type < _MAP))
              return BInError(bin, AErrorInvalidType, params, index, origin);
            if (length < 1)  // 1 IUA for the width word.
              return BInError(bin, AErrorBufferUnderflow, params, index,
                              origin);

            iua_ref = TRef<CHA>(args[arg_index]);
            if (iua_ref == _NilP)
              return BInError(bin, AErrorImplementation, params, index, origin);

            iua = *origin;
            ++origin; if (origin >= end) origin = begin;
            hash = HashIUB(iua, hash);
            if (iua > length - 1)
              return BInError(bin, AErrorBufferOverflow, params, index, origin);
            length = length - count - 1;
            count = IUW(iua);
            break;
          }
          case 1: {
            if (length < 2)  // 2 IUA for the width word.
              return BInError(bin, AErrorBufferUnderflow, params, index,
                              origin);
            length -= 2;
            iub_ref = TRef<IUB>(args[arg_index]);
            if (iub_ref == _NilP)
              return BInError(bin, AErrorImplementation, params, index, origin);

            for (ISN i = 0; i <= sizeof(IUB); i += 8) {
              iua = *origin;
              ++origin; if (origin >= end) origin = begin;
              hash = HashIUB(iua, hash);
              iub |= ((IUB)iua) << i;
            }
            if (iub > length)
              return BInError(bin, AErrorBufferOverflow, params, index, origin);
            length -= count;
            count = IUW(iub);
            iua_ref = TRef<CHA>(iub_ref);
            break;
          }
          case 2: {
            if (length < 4)  // 4 IUA for the width word.
              return BInError(bin, AErrorBufferUnderflow, params, index,
                              origin);
            length -= 4;
            iuc_ref = TRef<IUC>(args[arg_index]);
            if (iuc_ref == _NilP)
              return BInError(bin, AErrorImplementation, params, index, origin);

            for (ISN i = 0; i <= sizeof(IUC); i += 8) {
              iua = *origin;
              ++origin; if (origin >= end) origin = begin;
              hash = HashIUB(iua, hash);
              iuc |= IUC(iua) << i;
            }
            if (iuc >= length)
              return BInError(bin, AErrorBufferOverflow, params, index, origin);
            length -= count;
            count = IUW(iuc);
            iua_ref = TRef<CHA>(iuc_ref);
            break;
          }
          case 3: {  // 8 IUA for the width word.
            if (length < 9)
              return BInError(bin, AErrorBufferUnderflow, params, index,
                              origin);
            length -= 8;
            iud_ref = TRef<IUD>(args[arg_index]);
            if (iud_ref == _NilP)
              return BInError(bin, AErrorImplementation, params, index, origin);

            for (ISN i = 0; i <= sizeof(IUD); i += 8) {
              iua = *origin;
              ++origin; if (origin >= end) origin = begin;
              hash = HashIUB(iua, hash);
              iud |= IUD(iua) << i;
            }
            if (iud > length)
              return BInError(bin, AErrorBufferOverflow, params, index, origin);
            length -= count;
            count = IUW(iud);
            iua_ref = TRef<CHA>(iud_ref);
            break;
          }
          default:
            return BInError(bin, AErrorImplementation, params, index, origin);
        }

        if (length < count)
          return BInError(bin, AErrorBufferOverflow, params, index, origin);
        if (count == 0) break;  //< Not sure if this is an error.
        if (origin + count >= stop) {
          for (; size - count > 0; --count) {
            iua = *origin;
            ++origin; if (origin >= end) origin = begin;
            hash = HashIUB(iua, hash);
            *iua_ref = iua;
            ++iua_ref;
          }
          stop = origin - 1;
          for (; count > 0; --count) {
            iua = *origin;
            ++origin; if (origin >= end) origin = begin;
            hash = HashIUB(iua, hash);
            *iua_ref = iua;
            ++iua_ref;
          }
          break;
        }
        for (; count > 0; --count) {
          iua = *origin;
          ++origin; if (origin >= end) origin = begin;
          hash = HashIUB(iua, hash);
          *iua_ref = iua;
          ++iua_ref;
        }
        break;
#endif  //< _DThis
      }
    }
    ++arg_index;
    //_DROut(" |");
  }
  //_DROut("\nHash expected:0x" << Hexf(hash));
  if (length < 2)
    return BInError(bin, AErrorBufferUnderflow, params, index, origin);
  iub = *origin;
  ++origin; if (origin >= end) origin = begin;
  iua = *origin;
  ++origin; if (origin >= end) origin = begin;
  iub |= (((IUB)iua) << 8);
  //_DROut("found:0x" << Hexf(iub));
  if (hash != iub)
    return BInError(bin, AErrorInvalidHash, params, index, origin);

  //_DROut("\nDone reading\n");
  _DArrayFill(origin, stop);

  // Convert pointer back to offset: advance the read head (consumer) to where
  // we stopped reading. TDelta(begin, origin) = origin - begin (byte offset).
  bin->origin = TDelta<ISN>(begin, origin);

  return 0;
}

const Op* BInRead(TBOut<ISC>* bout, const DTB* params, void** args, IUD pc_ctx) {
  return BInRead(TRef<TBIn<ISC>>(bout), params, args, pc_ctx);
}

}  //< namespace _
#endif

#endif  //< _CrabsBInHXX
