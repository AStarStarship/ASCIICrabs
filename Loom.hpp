// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_LOOM_HPP
#define CRABS_LOOM_HPP
#include <_Config.h>
#if SEAM >= CRABS_LOOM
#include "Array.h"
#include "Stack.hpp"
#include "AType.hpp"
#include "BigInt.h"
#if SEAM == CRABS_LOOM
#include "_Debug.h"
#else
#include "_Release.h"
#endif
#define LOM_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, typename ISY = ISQ
#define LOM_P CHS, CHT, ISZ, ISY
#define LOM TLoom<LOM_P>
namespace _ {
/* @defgroup Loom
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Loom.md

# Memory Layout

+============================+
|_______ Boofer              | <-- start
|_______ String N            |
|_______ String 0            |
+----------------------------+
|_______ Boofer              |
|_______ String offset N     |
|        String offset 0     |
+============================+  ^ Up in
|       Loom Header          |  |
+============================+  + 0xN

@todo Look into adding variable for average UTF-8 and UTF-16 character length.
@todo Change keys_offset delta from table to keys_map.
*/

/* An array of Strings. */
template<LOM_A>
struct TLoom {
  ISZ bytes,                  //< Size of the Loom in bytes.
      start;                  //< Offset to the start of the string stack.
  TStack<ISZ, ISZ, ISY> map;  //< A Stack of offset mappings to strings.
};

template<LOM_A>
constexpr ISZ TLoomCountMin() {
  return static_cast<ISZ>(8 / sizeof(ISZ));
}

template<LOM_A>
constexpr ISZ TLoomBytesMin() {
  enum {
    CountMin = TLoomCountMin(),
    SizeMin = sizeof(TLoom<LOM_P>) + CountMin * (sizeof(ISZ) + 2),
  };
  return static_cast<ISZ>(SizeMin);
}

/* Calculates the minimum size of a Loom of empty strings with the given 
total. */
template<LOM_A>
ISZ TLoomBytesMin(ISZ total, ISZ average_string_length = 0) {
  return static_cast<ISZ>(sizeof(TLoom<LOM_P>) + 
    total * (sizeof(ISY) + (average_string_length + 1) * sizeof(CHS)));
}

/* The default length of a key. */
template<LOM_A>
constexpr ISZ TLoomKeyLengthDefault() {
  return 32;
}

/* Default number of strings in a Loom. */
template<LOM_A>
constexpr ISZ TLoomCountDefault() {
  return 32;
}

/* The default size of a Loom when no parameters are specified. */
template<LOM_A>
constexpr ISZ TLoomSizeDefault() {
  return (TLoomKeyLengthDefault<LOM_P>() *
    TLoomCountDefault<LOM_P>() * sizeof(CHS)) & (sizeof(ISW) - 1);
}

/* The default size of a Loom when no parameters are specified. */
template<LOM_A>
constexpr ISZ TLoomSize(ISY total, ISZ string_length_average = 
                         TLoomKeyLengthDefault<LOM_P>()) {
  return total * ((string_length_average + 1) * sizeof(CHS) +
          sizeof(ISY)) + sizeof(TLoom<LOM_P>);
}

/* Gets the default number of characters in a loom string. */
template<LOM_A>
constexpr ISZ TLoomDefaultLengthString() {
  return 32;
}

/* Calculates the default size of a Loom with the given average string
length. */
template<LOM_A>
inline ISZ TLoomKeysSize(ISY total, ISZ average_length_string =
  TLoomDefaultLengthString<LOM_P>()) {
  return total * sizeof(CHS) * (average_length_string + 1) +
          total * sizeof(ISY) + sizeof(TLoom<LOM_P>);
}

/* Returns the offset to the begin of the strings. */
template<LOM_A>
inline ISZ TLoomKeysBeginOffset(ISY total) {
  return sizeof(ISZ) * total + sizeof(TLoom<LOM_P>);
}

/* Returns the pointer to the first character in the loom string boofer. */
template<LOM_A>
inline CHS* TLoomKeysBegin(TLoom<LOM_P>* loom) {
  return TLoomKeysBegin<LOM_P>(loom, loom->map.total);
}
template<LOM_A>
inline const CHS* TLoomKeysBegin(const TLoom<LOM_P>* loom) {
  return TLoomKeysBegin<LOM_P>(loom, loom->map.total);
}

/* Returns the first char in the string boofer. */
template<LOM_A>
inline CHS* TLoomKeysBegin(TLoom<LOM_P>* loom, ISY total) {
  return TPtr<CHS>(loom, TLoomKeysBeginOffset<LOM_P>(total));
}
template<LOM_A>
inline const CHS* TLoomKeysBegin(const TLoom<LOM_P>* loom, ISY total) {
  return TPtr<const CHS>(loom, TLoomKeysBeginOffset<LOM_P>(total));
}

template<LOM_A>
CHS* TLoomStop(TLoom<LOM_P>* loom, ISZ bytes) {
  return TPtr<CHS>(TPtrDown<ISW, CHS>(loom, bytes));
}
template<LOM_A>
inline const CHS* TLoomStop(const TLoom<LOM_P>* loom, ISZ bytes) {
  return CPtr<CHS>(TLoomStop<LOM_P>(CPtr<TLoom<LOM_P>>(loom), bytes));
}

template<LOM_A>
CHS* TLoomStop(TLoom<LOM_P>* loom) {
  return TLoomStop<LOM_P>(loom, loom->bytes);
}
template<LOM_A>
inline const CHS* TLoomStop(const TLoom<LOM_P>* loom) {
  return TLoomStop<LOM_P>(loom, loom->bytes);
}

/* Returns the byte after the last byte in the Loom data structure. */
template<LOM_A, typename T = IUA>
inline const T* TLoomEnd(const TLoom<LOM_P>* loom) {
  return TPtr<T>(loom, loom->bytes);
}
template<LOM_A, typename T = IUA>
inline T* TLoomEnd(TLoom<LOM_P>* loom) {
  return CPtr<T>(
    TLoomEnd<LOM_P>(CPtr<TLoom<LOM_P>>(loom)));
}

/* Returns the byte after the last byte in the Loom data structure. */
template<LOM_A, typename T = IUA>
inline const T* TLoomTop(const TLoom<LOM_P>* loom) {
  return TPtr<T>(loom, loom->bytes);
}
template<LOM_A, typename T = IUA>
inline T * TLoomTop(const TLoom<LOM_P>*loom) {
  return CPtr<T>(
    TLoomTop<LOM_P>(CPtr<TLoom<LOM_P>>(loom), loom->bytes));
}

/* Free space left in the keys boofer */
template<LOM_A>
inline ISZ TLoomSpace(const TLoom<LOM_P>* loom) {
  return loom->bytes - loom->start;
}

/* Returns the minimum number of bytes required to hold the current contents
of the loom, sized for exactly loom->map.count map entries.
@pre loom must be a valid, initialized TLoom. */
template<LOM_A>
ISZ TLoomBytesMinFast(const TLoom<LOM_P>* loom) {
  if (IsError(loom))
    return 0;
  ISZ string_bytes = loom->start -
    TLoomKeysBeginOffset<LOM_P>(ISY(loom->map.total));
  // @todo Scan through keys to find unused space; or make another function.
  return ISZ(sizeof(TLoom<LOM_P>)) + ISZ(loom->map.count) * ISZ(sizeof(ISZ)) +
    string_bytes;
}

/* Points to the string offsets array. */
template<LOM_A>
ISZ* TLoomKeysMap(TLoom<LOM_P>* loom) {
  return TPtr<ISZ>(loom, sizeof(TLoom<LOM_P>));
}
template<LOM_A>
const ISZ* TLoomKeysMap(const TLoom<LOM_P>* loom) {
  return CPtr<ISZ>(TLoomKeysMap<LOM_P>(CPtr<TLoom<LOM_P>>(loom)));
}

/* Gets the element at the given index. */
template<LOM_A>
CHS* TLoomGet(TLoom<LOM_P>* loom, ISY index) {
  D_ASSERT(loom);
  if (index < 0 || index >= loom->map.count) 
    return NILP;
  return TPtr<CHS>(loom, TStackBegin<ISZ, ISZ>(&loom->map)[index]);
}
template<LOM_A>
const CHS* TLoomGet(const TLoom<LOM_P>* loom, ISY index) {
  return CPtr<CHS>(TLoomGet<LOM_P>(CPtr<TLoom<LOM_P>>(loom), index));
}

/* Gets the element at the given index. */
template<LOM_A>
const CHS* TLoomGet_NC(const TLoom<LOM_P>* loom, ISY index) {
  return TPtr<CHS>(loom, TLoomKeysMap<LOM_P>(loom)[index]);
}
template<LOM_A>
CHS* TLoomGet_NC(TLoom<LOM_P>* loom, ISY index) {
  return CPtr<CHS>(TLoomGet_NC<LOM_P>(CPtr<TLoom<LOM_P>>(loom), index));
}

/* Prints the Loom to the stream. */
template<typename Printer, LOM_A>
Printer& TLoomPrint(Printer& o, const TLoom<LOM_P>* loom) {
  ISY count = ISY(loom->map.count);
  o << Linef("\n+---\nLoom<CH") << CSizeCodef<CHS>() << ", IS" 
    << CSizeCodef<ISZ>() << ", IS" << CSizeCodef<ISY>() << "> size:" 
    << loom->bytes << " start:" << loom->start << " total:" << loom->map.total
    << " count:" << count;
  const ISZ* offsets = TStackBegin<ISZ, ISZ>(&loom->map);
  for (ISY i = 0; i < count; ++i)
    o << '\n' << i << ".) \"" << TLoomGet<LOM_P>(loom, i) << "\":"
      << offsets[i];
  return o << Linef("\n+---\n");
}

/* Initializes the loom with the given total.
@pre The size in bytes must be the first ISZ of the data. */
template<LOM_A>
inline TLoom<LOM_P>* TLoomInit(TLoom<LOM_P>* loom, ISY total) {
  D_ASSERT(loom);
  A_ASSERT(total >= TLoomCountMin<LOM_P>());
  D_RAM_WIPE(&loom->start, loom->bytes - sizeof(ISZ));

  // total -= total & 3; // @todo Ensure the values are word-aligned?
  auto start = TLoomKeysBegin<LOM_P>(loom, total); // @todo Why???
  ISZ bytes = loom->bytes;
  loom->start = TDelta<ISZ>(loom, start);
  loom->map.total = total;
  loom->map.count = 0;
  D_COUT("\n\nTLoomInit" << "" <<" bytes: "
         << loom->bytes << " count:" << loom->map.count
         << "/" << total << " start:" << loom->start
         << " space_atop:" << (loom->start - loom->bytes));
  return loom;
}

/* Adds a string to the end of the Loom.
@return The index upon success or -1 upon failure. */
template<LOM_A, typename CH = CHS>
ISY TLoomInsert(TLoom<LOM_P>* loom, const CH* element, ISY index = PSH) {
  D_ASSERT(loom);
  D_ASSERT(element);
  if (IsError(loom) || IsError(element))
    return -AErrorInvalidInput;
  ISY count = ISY(loom->map.count),
      total = ISY(loom->map.total);
  if (count >= total) {
    D_COUT("\n\nERROR: count:" << count << " total:" << total);
    return -AErrorInvalidInput;
  }
  D_ASSERT(count >= 0);
  CHS* cursor = NILP;
  if (index < 0) {
    if (index == ANY) {
      D_COUT("\n\nindex == _ANY\n\n");
      if (count <= 1) {
        index = count;
      } else {
        auto length = TSCodeCount<CH, ISZ>(element);
        for (ISN i = 0; i < count; ++i) {
          if (i == count) {
            index = count;
            break;
          }
          cursor = TSStop<CHS, CHT>(TLoomGet<LOM_P>(loom, i)) + 1;
          CHS* start_next_string = TLoomGet<LOM_P>(loom, i + 1);
          if (start_next_string - cursor > length) {
            index = i;
            break;
          }
          cursor = start_next_string;
        }
      }
      if (index != count) {
        index = TStackInsert<ISZ, ISZ, ISY>(&loom->map, TDelta<ISZ>(loom, cursor), 
                                            index);
        TSPrint<CHS, CHT>(cursor, TLoomEnd<LOM_P, CHS>(loom), element);
        return index;
      }
    }
  }
  cursor = TPtr<CHS>(loom, loom->start);
  cursor = TSPrint<CHS, CHT>(cursor, (CHS*)TLoomEnd<LOM_P>(loom), element);
  if (IsError(cursor))
    return -1;

  auto i = TStackInsert<ISZ, ISZ, ISY>(&loom->map, loom->start, index);
  index = ISY(i);
  loom->start = TDelta<ISZ>(loom, cursor + 1);
  return index;
}

template<LOM_A>
ISZ TLoomCharCount(TLoom<LOM_P>* loom) {
  return ISZ(TLoomEnd<LOM_P>(loom) - TLoomKeysBegin<LOM_P>(loom));
}
template<LOM_A>
BOL TLoomWrite(TLoom<LOM_P>* destination, TLoom<LOM_P>* source) {
  return true;
}

/* Clones the loom to the pre-allocated destination Socket.
@note dest is an ASCII Object at this point and is not initialized. */
template<LOM_A>
TLoom<LOM_P>* TLoomClone(TLoom<LOM_P>* dest, const TLoom<LOM_P>* src,
    ISZ src_bytes, ISY src_total, ISY total_new /* = src_total */) {
  D_COUT("\n\nTLoomClone:\n  dest_bytes:" << dest->bytes << " src_bytes:" <<
         src_bytes << " src_total:" << src_total << " total_new:" << total_new);
  if (IsError(src) || IsError(dest) || total_new <= 0)
    return NILP;
  ISZ src_start = src->start;
  ISY src_count = ISY(src->map.count);
  const ISZ* src_offsets = TLoomKeysMap<LOM_P>(src);
  ISZ src_start_new = 0;
  ISY copy_count = 0;
  if (total_new < src_count) {
    copy_count = total_new;
    src_start_new = TDelta<ISZ>(src, &src_offsets[copy_count]);
  } else {
    copy_count = src_count;
    src_start_new = src_start;
  }
  ISZ offsets_copy_bytes = copy_count * sizeof(ISZ);
  const CHS* src_start_char = TPtr<CHS>(src, src_start_new);
  dest->map.total = total_new;
  dest->map.count = src_count;
  // Copy offsets.
  ISZ* dest_offsets     = TLoomKeysMap<LOM_P>(dest),
     * dest_offsets_end = dest_offsets + copy_count;
  ISZ growth_delta = (total_new - src_total) * sizeof(ISZ);
  while (dest_offsets <= dest_offsets_end) 
    *dest_offsets++ = *src_offsets++ + growth_delta;
  D_COUT("\n  growth_delta:" << growth_delta);
  dest->start = src_start_new + growth_delta;
  
  // Copy strings.
  CHS* dest_begin = TLoomKeysBegin<LOM_P>(dest, total_new);
  ISZ src_begin_offset = TLoomKeysBeginOffset<LOM_P>(src_total);
  const CHS* src_begin = TPtr<CHS>(src, src_begin_offset);
  ISZ src_copy_end_offset = copy_count < src_count ? src_offsets[copy_count - 1] 
                                                   : src_start;
  ISZ string_copy_bytes = src_copy_end_offset - src_begin_offset;
  D_COUT("\n  dest_begin:" << TDelta<>(dest, dest_begin) << 
         " src_copy_end_offset:" << src_copy_end_offset <<
         " src_begin_offset:" << src_begin_offset <<
         " string_copy_bytes:" << string_copy_bytes <<
         " src_begin:" << TDelta<>(src, src_begin));
  D_ASSERT(string_copy_bytes > 0);
  ISW result = ArrayCopy(dest_begin, string_copy_bytes, src_begin, 
                         string_copy_bytes);
  D_ASSERT(result > 0);
  if (result <= 0)
    return NILP;
  return dest;
}
template<LOM_A>
inline TLoom<LOM_P>* TLoomClone(TLoom<LOM_P>* dest, const TLoom<LOM_P>* src,
  ISY total_new) {
  return TLoomClone<LOM_P>(dest, src, src->bytes, ISY(src->map->total), 
                           total_new);
}
template<LOM_A>
inline TLoom<LOM_P>* TLoomClone(TLoom<LOM_P>* dest, const TLoom<LOM_P>* src) {
  return TLoomClone<LOM_P>(dest, src, src->bytes, ISY(src->map->total));
}

/* Adds a string to the end of the Loom.
@return The index upon success or -1 upon failure. */
template<LOM_A>
CHS* TLoomPop(TLoom<LOM_P>* loom) {
  if (loom->map.count == 0) return NILP;
  ISZ offset = TStackPop<ISZ, ISZ>(&loom->map),
      start  = loom->start;
  loom->start = offset;
  return TPtr<CHS>(ISW(loom) + offset);
}

/* Adds a string to the end of the Loom.
@return The index upon success or -1 upon failure. */
template<LOM_A>
ISY TLoomRemove(TLoom<LOM_P>* loom, ISY index) {
  ISY count = ISY(loom->map.count);
  if (index == count) {
    if (!TLoomPop<LOM_P>(loom)) return -1;
    return count;
  }
  if (index < 0 || index > count) return -1;

  ISZ* offsets = TStackBegin<ISZ, ISZ>(&loom->map);
  ISZ offset   = offsets[index],        //
      offset_b = offsets[index + 1],  //
      delta    = offset_b - offset;      //

  TStackRemove<ISZ, ISZ>(&loom->map, ISZ(index));

  // RAMShiftDown(TPtr<CHA>(loom) + offset, delta);
  return index;
}

/* Searches for the given string in the loom.
@return -1 if the loom doesn't contain the string or the index if it does. */
template<LOM_A>
ISY TLoomFind(TLoom<LOM_P>* loom, const CHS* string) {
  D_ASSERT(loom);
  D_ASSERT(string);
  ISZ* offsets = TStackBegin<ISZ, ISZ>(&loom->map);
  for (ISY i = 0; i < loom->map.count; ++i) {
    ISZ offset = offsets[i];
    CHS* other = TPtr<CHS>(IUW(loom) + offset);
    if (TSEquals<CHS>(string, other)) return i;
  }
  return -1;
}

/* An 
Loom Autoject. */
template<LOM_A, ISZ Total_ = 512,
          typename BOF = TBOF<Total_, CHS, ISZ, TLoom<LOM_P>>>
class ALoom {
  AArray<IUA, ISZ, Total_, BOF> obj_;  //< An Auto-Array object.
 public:
  enum { CountDefault = Total_ / 16 };
  /* Constructs a Loom. */
  ALoom(ISY total = CountDefault) {
    TLoomInit<LOM_P>(This(), total);
  }

  /* Constructs a Loom subclass.
  @param factory RAMFactory to call when the String overflows. */
  ALoom(RAMFactory ram, ISY count = CountDefault)
      : obj_(ram) {
    TLoomInit<LOM_P>(This(), count);
  }

  /* Constructs a Loom subclass.
  @param factory RAMFactory to call when the String overflows. */
  ALoom(RAMFactory ram, ISZ bytes = TLoomSizeDefault<LOM_P>(),
        ISZ count = TLoomCountDefault<LOM_P>())
      : obj_(ram) {
    TLoomInit<LOM_P>(This(), count);
  }

  inline ISZ Size() { return obj_.Size(); }

  /* Removes the string at the given index from the Loom. */
  inline ISY Remove(ISY index) { return TLoomRemove<LOM_P>(This(), index); }

  /* Gets the string at the given index. */
  inline CHS* Get(ISY index) { return TLoomGet<LOM_P>(This(), index); }

  /* Searches for the given string.
  @return -1 if the Loom doesn't contain the string or the index if it does.
*/
  inline ISY Find(const CHS* string) {
    return TLoomFind<LOM_P>(This(), string);
  }

  /* Gets the ASCII Object. */
  inline TLoom<LOM_P>* This() { return this->obj_.template As<TLoom<LOM_P>>(); }

  /* Gets the Auto-Array. */
  inline Autoject& AJT() { return obj_.AJT(); }

  /* Gets the Auto-Array.
  inline AArray<IUA, ISZ, BOF>& AJT_ARY() { return obj_; } */

  template<typename Printer>
  inline Printer& PrintTo( Printer& o) {
    TLoomPrint<Printer, LOM_P>(o, This());
    return o;
  }

  inline void COut() { PrintTo<::_::COut>(::_::StdOut()); }

  /* Doubles the size and total of the loom.
  @return Returns nil if the size is greater than the amount of memory that
  can fit in type ISW, the unaltered socket pointer if the Stack has grown to
  the size upper bounds, or a new dynamically allocated socket upon failure. */
  BOL Grow() {
    Autoject& obj = AJT();
    auto src = TPtr<TLoom<LOM_P>>(obj.origin);
    if(!src)
      return false;
    ISZ bytes     = src->bytes,
        bytes_new  = AutojectGrowBytes(bytes);
    if (bytes == bytes_new) return false;
    ISY total     = ISY(src->map.total),
        total_new = AutojectGrowTotal(total);
    #ifdef D_THIS
    D_COUT("\n\nGrowing Loom... bytes_new:" << bytes_new << " total:" << 
           total << " total_new:" << total_new << "\n\nBefore:\n");
    COut();
    D_COUT(Charsf(src, bytes));
    #endif
    IUW* growth = obj.ram(NILP, bytes_new);
    D_OBJ_WIPE(growth, ISZ);
    auto dest = TPtr<TLoom<LOM_P>>(growth);
    auto dest_copy = TLoomClone<LOM_P>(dest, src, bytes, total, total_new);
    if (IsError(dest_copy))
      return false;
    Delete(obj, growth);
    #ifdef D_THIS
    dest = TPtr<LOM>(obj.origin);
    D_COUT("\n\nAfter:\n");
    TLoomPrint<::_::COut, LOM_P>(StdOut(), dest);
    //D_COUT(Charsf(dest, dest->bytes - 1)); //< @todo Why is this crashing?
    #endif
    return true;
  }

  /* Deep copies the given string into the Loom.
  @return The index of the string in the Loom. */
  template<typename CH = CHS>
  inline ISY Insert(const CH* item, ISY index = PSH) {
    if (IsError(item)) return -1;
    D_COUT("\nAdding:\"" << item << '\"' << " freespace:" << 
           TLoomSpace<LOM_P>(This()));
    IUW* origin = AJT().origin;
    LOM* loom = TPtr<LOM>(origin);
    ISY result = TLoomInsert<LOM_P, CH>(loom, item, index);
    while (result < 0) {

      if (!Grow()) {
        ISZ total = loom->map.total;
        D_COUT("\n\nFailed to grow with total:" << total << ".\n\n");
        return -1;
      }
      result = TLoomInsert<LOM_P, CH>(This(), item, index);
    }
    return result;
  }
};
}  //< namespace _
#endif
#endif
