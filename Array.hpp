// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_ARRAY_HPP
#define CRABS_ARRAY_HPP
#include "Array.h"
#if SEAM >= CRABS_ARRAY
#define ARY_A typename T = CHA, typename ISZ = ISN
#define ARY_P T, ISZ
#define ARY TArray<T, ISZ>
#define AARY AArray<T, ISZ>
#include "Autoject.hpp"
#include "Stringf.hpp"
#if SEAM == CRABS_ARRAY
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

#define ARY TArray<T, ISZ>
#define AARY AArray<T, ISZ>

/* An ASCII Array.
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/VectorTypes/Array.md

@code
+-----------------+
|  C-Style Array  |
|  255-byte array | size=255, requires 8-bits.
|-----------------|  ^
|  TArray Header  |  |
+-----------------+ 0xN
@endcode
*/
template<typename T = CHA, typename ISZ = ISW>
struct TArray {
  ISZ total;  //< Number of elements in the array.
};

template<ARY_A>
inline ISZ TArrayBytes(ISZ total) {
  return sizeof(ARY) + total * sizeof(T);
}
template<ARY_A>
inline ISZ TArrayBytes(const ARY* origin) {
  return TArrayBytes<ARY_P>(origin->total);
}

/* The maximum autoject size.
The max ASCII Object size is dictated by the max value allowed that can be
aligned up to a multiple of 8 (i.e. 64-bit word boundary).
template<typename ISZ = ISW>
inline ISZ TArraySizeMax() {
  ISZ total = 0;
  return (~total) - 15;
} */
/* The upper bounds defines exactly how many elements can fit into a space
in memory.
template<typename T = ISW, typename ISZ = ISW, typename Class>
inline ISZ TArraySizeMax() {
  ISZ max_value = ISZ((~ISZ(0)) >> 1);
  return max_value - ISZ(sizeof(IUW) - 1 - sizeof(Class) / sizeof(T));
} */

/* Checks if the given autoject count is in the min max bounds of an ASCII
Object.
template<ARY_A>
inline BOL TArrayCountIsValid(ISZ index, ISZ count_min) {
  return (index >= count_min) && (index < TArraySizeMax<T, ISZ>());
} */

/* Gets the begin byte. */
template<ARY_A>
inline T* TArrayBegin(void* origin) {
  return TPtr<T>(ISW(origin) + ISW(sizeof(ISZ)));
}
template<ARY_A>
inline const T* TArrayBegin(const void* origin) {
  return CPtr<T>(TArrayBegin<T, ISZ>(CPtr<void>(origin)));
}

/* Gets the start byte of the ASCII Object data section. */
template<ARY_A>
inline T* TArrayStart(void* origin) {
  return TArrayBegin<T, ISZ>(origin);
}
template<ARY_A>
inline const T* TArrayStart(const void* origin) {
  return TArrayBegin<T, ISZ>(origin);
}

/* Gets the begin byte. */
template<ARY_A>
inline T* TArrayStop(void* origin, ISZ bytes) {
  return TPtr<T>(origin, bytes);
}
template<ARY_A>
inline const T* TArrayStop(const void* origin, ISZ bytes) {
  return CPtr<T>(TArrayBegin(CPtr<void>(origin), bytes));
}
template<ARY_A>
inline T* TArrayStop(void* origin) {
  return TArrayStop<T, ISZ>(origin, TSize<ISZ>(origin));
}
template<ARY_A>
inline const T* TArrayStop(const void* origin) {
  return TArrayStop<T, ISZ>(origin, TSize<ISZ>(origin));
}

/* Fills the data section with the fill_char. */
template<ARY_A>
inline T* TArrayFill(void* origin, CHA fill_char = 0) {
  ISZ bytes_data = TPtr<ARY>(origin)->total * sizeof(T);
  return ArrayFill(TArrayBegin<ARY_P>(origin), bytes_data, fill_char);
}

/* Prints the item to the printer. */
template<typename Printer, ARY_A>
Printer& TArrayPrint(Printer& o, const ARY* item) {
  ISZ size = item->total;
  o << Linef("\n+---\nTArray<ISZ") << CSizeCodef<ISZ>()
    << "> size:" << size;
  if (size == 0) return o;
  T* items = TArrayStart<T, ISZ>(item),
   * items_end = items + size;
  o << *items++;
  while (items < items_end)
    o << ", " << *items++;
  return o << Linef("\n+---");
}

/* Prints the item to the printer. */
template<ARY_A, typename Printer>
Printer& TArrayPrintLines(Printer& o, const ARY* item) {
  ISZ size = item->total;
  o << Linef("\n+---\nTArray<ISZ") << CSizeCodef<ISZ>()
    << "> size:" << size << '\n';
  if (size == 0) return o;
  T* items = TArrayStart<T, ISZ>(item);
  ISZ i = 0;
  for (; i < size; ++i) {
    o << ',' << items[i];
  }
  return o << Linef("\n+---");
}

// Copies the source to the destination.
template<ARY_A, ISA AlignMask>
ARY* TArrayCopy(void* destination, const void* source) {
  auto dst = TPtr<ISZ>(destination);
  auto src = TPtr<const ISZ>(source);
  auto dst_count = *dst;
  auto src_count = *src;
  auto result = ArrayCopy(TPtrUp<>(dst + 1, AlignMask),
                        dst_count * sizeof(T),
                        TPtrUp<>(src + 1, AlignMask),
                        src_count * sizeof(T));
  if (result <= 0) return NILP;
  *dst = src_count;
  return TPtr<ARY>(dst);
}

/* Calculates the size in bytes of an array with given element_count. */
template<ARY_A>
inline ISZ TSizeOf(ISZ element_count) {
  return sizeof(T) * element_count;
}

/* Calculates the size in bytes of an array with given element_count including
the Class header. */
template<ARY_A, typename Class>
inline ISZ TSizeOf(ISZ size) {
  return sizeof(T) * size + sizeof(Class);
}

/* The minimum size of an array with zero elements. */
template<typename ISZ = ISW>
constexpr ISZ CSizeMin() {
  return 0;
}

/* Writes the source TArray to the destination. */
template<typename T = IUA, typename ISZ = ISW, typename Class>
ARY* TArrayWrite(ARY* destination, ARY* source,
                         ISZ size) {
  D_ASSERT(destination);
  D_ASSERT(source);

  ISW bytes = (ISW)TSizeBytes<T, ISZ, Class>(size);
  if (!ArrayCopy(destination, bytes, source, bytes)) return NILP;
  return destination;
}

template<typename T = IUA, typename ISZ = ISW, typename Class>
Class* TArrayWrite(IUW* destination, IUW* source, ISZ size) {
  ARY* result =
      TArrayWrite<T, ISZ, Class>(TPtr<ARY>(destination),
                                TPtr<ARY>(source), size);
  return TPtr<IUW>(result);
}

/* Clones the other ASCII Autoject including possibly unused autoject space.
@return Nil upon failure or a pointer to the cloned autoject upon success.
@param socket A raw ASCII Socket to clone.
template<typename T = IUA, typename ISZ = ISW, typename Class>
IUW* TArrayCloneSlow(Autoject& origin) {
  RAMFactory factory = origin.ram;
  IUW* origin = origin.origin;
  // if (IsError(factory) || IsError(origin)) return NILP;

  ARY* o = TPtr<ARY>(origin);
  ISZ size = o->size;
  IUW* clone = TObjectFactoryNew<T, ISZ, ARY>(size);
  return TArrayWrite<T, ISZ, Class>(clone, origin, size);
} */

/* Gets the stop of the TArray. */
template<typename T = CHA, typename ISZ = ISW>
inline T* TArrayStop(ARY* ary) {
  return TArrayStart<T, ISZ>(ary) + ary->total;
}
template<typename T = CHA, typename ISZ = ISW>
inline const T* TArrayStop(const ARY* ary) {
  return CPtr<T>(TArrayStop<T, ISZ>(CPtr<ARY>(ary)));
}

/* Gets the last IUA in the ASCII Object. */
template<typename T = CHA, typename ISZ = ISW>
inline T* TArrayEnd(ARY* array) {
  return TArrayStop<T, ISZ>(array);
}

/* Gets the last IUA in the ASCII Object. */
template<typename T = CHA, typename ISZ = ISW>
inline const T* TArrayEnd(const ARY* array) {
  return TArrayStop<T, ISZ>(array);
}

/* Inserts the given item into the array, but can't push on top.
@pre items_start must be less than items_stop */
template<typename T>
inline void TArrayInsert_NC(T* items_start, T* items_stop, T item) {
  while (items_start < items_stop) {
    auto read = *(items_stop - 1);
    *items_stop-- = read;
  }
  *items_start = item;
}

/* Inserts the given item at the start array.
@pre You must perform bounds checking before calling this function. */
template<ARY_A>
inline void TArrayInsert_NC(T* elements, ISZ count, T item) {
  return TArrayInsert_NC<T>(elements, elements + count, item);
}

/* Inserts the given item at the index index the elements of the given count.
@pre You must perform bounds checking before calling this function. */
template<ARY_A>
inline void TArrayInsert_NC(T* items, ISZ count, ISZ index, T item) {
  TArrayInsert_NC<T>(items + index, items + count, item);
}

/* Inserts the given item at the start array growing down negative.
@pre items_stop must be less than items_start. */
template<typename T>
inline void TArrayInsertDown_NC(T* items_start, T* items_stop, T item) {
  while (items_start > items_stop) {
    auto read = *(items_stop + 1);
    *items_stop++ = read;
  }
  *items_start = item;
}

/* Inserts the given item at the start array.
@pre You must perform bounds checking before calling this function. */
template<ARY_A>
inline void TArrayInsertDown_NC(T* elements, ISZ index, T item) {
  return TArrayInsertDown_NC<T>(elements, elements + index, item);
}

/* Inserts the given item at the index index the elements of the given count.
@pre You must perform bounds checking before calling this function. */
template<ARY_A>
inline void TArrayInsertDown_NC(T* items, ISZ count, ISZ index, T item) {
  TArrayInsertDown_NC<T>(items + index, items + count, item);
}

template<ARY_A>
inline T* TArrayInsert(T* dest, ISZ dest_total, const T* src, ISZ src_total, 
  ISZ src_offset = 0) {
  ISZ src_stop_count = src_total + src_offset;
  if (dest_total < 0 || src_total < 0 || src_offset < 0 || 
    src_stop_count >= dest_total)
    return NILP;
  T* dest_start = dest + src_offset,
   * dest_stop = dest + src_stop_count,
   * dest_end  = dest + dest_total;
  const T* src_start = src + src_offset;
  while (dest < dest_start) *dest++ = 0;
  while (dest < dest_stop) *dest++ = *src++;
  while (dest < dest_end ) *dest++ = 0;
}

/* Shifts the elements down starting at the index without deleting the last 
element.
@pre You must perform bounds checking before calling this function. */
template<typename T>
inline void TArrayShift_NC(T* elements, T* end) {
  while (elements < end) {
    *elements = *(elements + 1);
    ++elements;
  }
}
template<ARY_A>
void TArrayShift_NC
(T* elements, ISZ total) {
  if (total < 0) return;
  return TArrayShift_NC<T>(elements, elements + total);
}

/* Shifts the elements down starting at the index element and deletes the index
element at the given index.
@pre You must perform bounds checking before calling this function. */
template<ARY_A>
void TArrayShift(T* elements, T* end, ISZ count = 1) {
  if (count < 1) return;
  auto dez_nutz = elements + count;
  auto read_cursor = dez_nutz;
  while (elements < dez_nutz) {
    auto element = *read_cursor;
    *elements++ = element;
  }
}
template<ARY_A>
void TArrayShift(T* elements, ISZ total, ISZ count = 1) {
  if (total < 0) return;
  return TArrayShift<T>(elements, elements + total, count);
}

/* Shifts the elements above the index down 1. */
template<ARY_A>
void TArrayRemove_NC(T* begin, T* end, ISZ index) {
  if (begin == --end) {
    *begin = 0;
    return;
  }
  begin += index;
  while (begin < end) {
    T e = *(begin + 1);
    *begin++ = e;
  }
}
template<ARY_A>
inline void TArrayRemove_NC(T* begin, ISZ total, ISZ count) {
  return TArrayRemove_NC(begin, begin + total, count);
}

template<ARY_A>
inline void TArrayRemove(T* elements, T* end, ISZ count) {
  if (count < 1) return;
  TArrayRemove_NC<T, ISZ>(elements, end, count);
}

template<ARY_A>
inline void TArrayRemove(T* elements, ISZ total, ISZ count) {
  if (total < 0) return;
  TArrayRemove<T>(elements, elements + total, count);
}

/* An invalid index. */
template<typename ISZ = ISW>
constexpr ISZ CAInvalidIndex() {
  return -1;
}

/* Searches for the item in the elements.
@return An invalid index upon failure or a valid index upon success. */
template<ARY_A>
ISZ TArrayFind(const T* elements, ISZ element_count, const T& Item) {
  for (ISZ i = 0; i < element_count; ++i)
    if (*elements++ == Item) return i;
  return CAInvalidIndex<ISZ>();
}

/* Compares the two memory sockets.
@param start  The start of Array A.
@param stop   The stop of Array A.
@param origin The origin of Array B.
@param end    The end of Array B.
@return True if they are the same and false if they are not. */
inline ISW RAMCompare(const void* a_begin, const void* end_a,
                        const void* b_begin, const void* end_b) {
  return ArrayCompare(a_begin, SizeOf(a_begin, end_a), b_begin,
                      SizeOf(b_begin, end_b));
}

/* Compares the two memory sockets.
@param start  The start of Array A.
@param stop   The stop of Array A.
@param origin The origin of Array B.
@param size   The size of Array B.
@return True if they are the same and false if they are not. */
inline ISW RAMCompare(const void* a_begin, void* a_end, const void* b_begin,
                      ISW b_bytes) {
  return RAMCompare(a_begin, a_end, a_begin,
                    TPtr<CHA>(b_begin) + b_bytes);
}

/* Casts RAMCompare to type T. */
template<typename T>
inline T* TArrayCompare(void* a_begin, void* a_end, void* b_begin,
                        ISW b_bytes) {
  return TPtr<T*>(RAMCompare(a_begin, a_end, b_begin, b_bytes));
}

/* Copies the total elements from the source ASCII Array to the destination. */
template<typename T, typename Class>
inline ISW TArrayCopy(void* dest, ISW dest_total,
  const void* src, ISW src_count) {
  return ArrayCopy(TPtr<void, Class>(dest), dest_total * sizeof(T),
    TPtr<void, Class>(src), src_count * sizeof(T));
}
template<typename T, typename Class>
inline ISW TArrayCopy(void* dest, void* dest_end,
  const void* src, ISW src_count) {
  return ArrayCopy(TPtr<void, Class>(dest), TDelta<ISW, Class>(dest, dest_end),
    TPtr<void, Class>(src), src_count * sizeof(T));
}
template<typename T, typename Class>
inline ISW TArrayCopy(void* dest, ISW dest_total,
  const void* src, const void* src_end) {
  return ArrayCopy(TPtr<void, Class>(dest), dest_total * sizeof(T),
    TPtr<void, Class>(src), TDelta<ISW, Class>(src, src_end));
}
template<typename T, typename Class>
inline ISW TArrayCopy(void* dest, void* dest_end,
  const void* src, const void* src_end) {
  return ArrayCopy(TPtr<void, Class>(dest), TDelta<ISW, Class>(dest, dest_end),
    TPtr<void, Class>(src), TDelta<ISW, Class>(src, src_end));
}

/* Copies the data from the source ASCII Object to the destination. */
template<typename Class>
inline ISW TObjectCopy(void* dest, ISW dest_bytes,
  const void* src, ISW src_bytes) {
  return ArrayCopy(TPtr<void, Class>(dest), dest_bytes - sizeof(Class),
    TPtr<void, Class>(src), src_bytes - sizeof(Class));
}
template<typename Class>
inline ISW TObjectCopy(void* dest, void* dest_end,
  const void* src, ISW src_bytes) {
  return ArrayCopy(TPtr<void, Class>(dest), TDelta<ISW, Class>(dest, dest_end),
    TPtr<void, Class>(src), src_bytes - sizeof(Class));
}
template<typename Class>
inline ISW TObjectCopy(void* dest, ISW dest_bytes,
  const void* src, const void* src_end) {
  return ArrayCopy(TPtr<void, Class>(dest), dest_bytes - sizeof(Class),
    TPtr<void, Class>(src), TDelta<ISW, Class>(src, src_end));
}
template<typename Class>
inline ISW TObjectCopy(void* dest, void* dest_end, const void* src,
  const void* src_end) {
  return ArrayCopy(TPtr<void, Class>(dest), TDelta<ISW, Class>(dest, dest_end),
    TPtr<void, Class>(src), TDelta<ISW, Class>(src, src_end));
}

/* Casts RAMCompare to type T. */
template<typename T>
inline const T* TArrayCompare(const void* a_begin, void* a_end, 
  const void* b_begin, ISW b_bytes) {
  return TPtr<const T*>(RAMCompare(a_begin, a_end, b_begin, b_bytes));
}

template<ARY_A, ISZ TotalInit_ = AStackTotalDefault,
         typename BOF = TBOF<TotalInit_, T, ISZ, ARY>, typename Class = Nil>
class AArray {
  Autoboofer<ARY_P, BOF, ARY> aobj_;  //< An Auto-Object.

  constexpr RAMFactory InitRamFactory() {
    return sizeof(BOF) == 0 ? TObjectFactory<ISZ>::StackHeap
                            : TObjectFactory<ISZ>::StackStack;
  }

  inline RAMFactory InitRamFactory(RAMFactory factory) {
    return !factory ? InitRamFactory() : factory;
  }

 public:
  /* Initializes a array of n elements to whatever can fit in the BOF. */
  AArray() {
    D_RAM_WIPE(Begin(), CBytes() - sizeof(ARY));
  }

  /* Attempts to down-size the BOF or creates a dynamically allocated array.
  @param size The number of elements in the Array. */
  AArray(ISZ total) : aobj_(TArrayBytes<ARY_P>(total)) {
    D_RAM_WIPE(Begin(), CBytes() - sizeof(ARY));
  }

  AArray(ISZ total, RAMFactory ram) : aobj_(TArrayBytes<ARY_P>(total), ram) {
    D_RAM_WIPE(Begin(), CBytes() - sizeof(ARY));
  }

  /* Copies the items into the obj_.Boofer () with some additional
  empty_elements. */
  AArray(ISZ total, const T* items, ISZ items_count, ISZ items_start = 0)
      : aobj_(TArrayBytes<ARY_P>(items_count)) {
    TArrayInsert<ARY_P>(aobj_.AJT(), items, items_count, items_start);
  }

  /* Copies the items into the obj_.Boofer () with some additional
  empty_elements. */
  AArray(const T* items, ISZ items_count)
      : aobj_(TArrayBytes<ARY_P>(items_count)) {
    TArrayInsert<ARY_P>(aobj_.AJT(), items, items_count);
  }

  /* Destructs nothing. */
  ~AArray() {}

  inline ISZ Total() { return This()->total; }
  inline ISZ CBytes() { return TArrayBytes<T, ISZ>(Total()); }
  inline ISZ SizeWords() { return CBytes() >> ACPUBytesLog2; }

  /* Gets the 16-bit ASCII Data Type for this Stack. */
  constexpr DTB CType() { return VTSCKBits | CATypePOD<T>(); }

  /* Gets this auto-object. */
  inline Autoject& AJT() { return aobj_.AJT(); }

  /* Returns the boofer_. */
  inline IUW* Origin() { return AJT().origin; }

  /* Sets the origin to the given pointer.
  @return Nil if the input is nil. */
  inline IUW* OriginSet(void* socket) {
    return OriginSet(aobj_, socket);
  }

  /* Return the Origin base pointer as type T. */
  template<typename T_ = ARY>
  inline T_* As() { return TPtr<T_>(Origin()); }

  /* Creates a clone on the heap.
  @return NILP if the inputs are invalue */
  inline static ARY* Clone(Autoject& dest, const ARY* src, ISZ total_new = -1, 
                           ISZ count_start = 0, ISZ count_stop = -1) {
    D_ASSERT(src);
    ISZ total = src->total,
        count = src->count;
    D_ASSERT(total >= 0);
    D_ASSERT(count >= 0);
    D_ASSERT(count <= total);
    if (count_stop < 0) count_stop = count;
    if (total_new < 0) total_new = AutojectGrowTotal(total);
    if (total < 0 || count < 0 || count > total || count_start < 0 || 
        count_start >= count || count_stop > count)
      return NILP;
#ifdef D_THIS
    D_COUT("\nAuto-growing Stack...\nBefore:");
    TArrayPrint<COut, T, ISZ>(StdOut(), src);
#endif
    ISZ count_new = count_stop - count_start,
        bytes_copy = count_new * sizeof(T),
        bytes      = sizeof(ARY) + total_new * sizeof(T);
    ARY* dest_ary = TPtr<ARY>(dest.ram(NILP, bytes));
    if (IsError(dest_ary))
      return dest_ary;
    *dest_ary = { total_new, count_new };
    D_COUT("\nCopying data..." <<
      "\ncount_start:" << count_start << " count_stop:" << count_stop <<
      "\narray_new.count: " << dest_ary->count <<
      "\narray_new.total: " << dest_ary->total <<
      "\nbytes: " << bytes << "\nbytes_copy: " << bytes_copy <<
      "\nTArrayStart<ARY_P>(array): " << Hexf(TArrayStart<ARY_P>(src)));
    ArrayCopy(TPtr<void>(dest_ary, sizeof(Class)), bytes_copy,
              TPtr<void>(src, sizeof(Class)) + count_start, bytes_copy);
#ifdef D_THIS
    D_COUT("\nResult:");
    TArrayPrint<COut, T, ISZ>(StdOut(), dest_ary);
#endif
    dest.origin = TPtr<IUW>(dest_ary);
    dest.ram = RAMFactory(dest.ram(NILP, 0));
    dest_ary->total = total_new;
    dest_ary->count = count_new;
    return dest_ary;
  }

  /* Clones the other object including the total, growing if necessary.
  @return true upon success and false upon failure. */
  inline ARY* Clone(ARY* src) {
    if (IsError(src)) return NILP;
    ARY* ths = This();
    ISZ this_total = ths->total,
        this_count = ths->count,
        src_total = src->total,
        src_count = src->count;
    if (this_total <= 0 || src_total <= 0 ||
        this_count <  0 || src_count <  0 ||
        this_count > this_total || src_count > src_total) {
      return TPtr<ARY>(AErrorInvalidInput);
    }
    ISZ count_new = src_count + this_count;
    if (count_new > this_total) {
      Resize(count_new, count_new);
    }
    ARY* ths2 = This();
    TArrayCopy<ISZ>(TArrayStart<ARY_P>(ths2), this_total,
                    TArrayStart<ARY_P>(src), src_count);
    return ths2;
  }

  /* Resizes the Array to the new total, growing if necessary. */
  inline ARY* Resize(ISZ total_new = -1) {
    ARY* ths = This();
    ISZ total = ths->total;
    D_ASSERT(total >= 0);
    if (total_new < 0) total_new = AutojectGrowTotal(total);
    else if (total_new <= total) {
      ths->total = total;
      return ths;
    }

    Autoject& ajt = AJT();
    Autoject growth = { NILP, ajt.ram };
    Clone(growth, ths, total_new);
    IUW* origin_new = growth.origin;
    if (this->PtrIsValid(origin_new)) {
      Delete(ajt);
      ajt.origin = growth.origin;
      ajt.ram = growth.ram;
      return TPtr<ARY>(origin_new);
    }
    D_ASSERT(false);
    return NILP;
  }

  /* Checks if the given index is a valid element.
  @return true if the index is in bounds. */
  inline BOL InBounds(ISZ index) {
    return index >= 0 && index < Total();
  }

  /* Points to the begin element in the origin. */
  inline T* Begin() { return TPtr<T>(This(), sizeof(Class)); }

  /* Points to the end element after the origin. */
  inline T* End() { return Begin() + Total(); }

  /* Gets the element at the given index.
  @return -1 if a is nil and -2 if the index is out of bounds.
  @param index The index of the element to get. */
  inline T Get(ISZ index) { return Begin()[index]; }

  /* Returns true if the given origin contains the given address.
  @return false upon failure. */
  inline BOL Contains(void* address) {
    return address < TPtr<void>(This()) || address >= TPtr<void>(End());
  }

  /* Resizes array to the new_total, potentially shrinking the count or growing 
  if necessary. */
  inline ARY* Resize(ISZ total_new, ISZ count_start = 0, ISZ count_stop = 0) {
    ARY* t = This();
    ISZ total = t->total,
        count = t->count;
    if (total_new < 0) total_new = AutojectGrowTotal(total);
    if (total_new <= count) {
      t->count = total_new;
      return t;
    }
    Autoject growth = { NILP, AJT().ram };
    ARY* result = Clone(growth, t, total_new, count_start, count_stop);
    if (result) {
      Autoject& ths = AJT();
      ths.origin = growth.origin;
      ths.ram = growth.ram;
      result->count = count_stop;
      Delete(ths);
    }
    return result;
  }

  /* Gets this ASCII Object. */
  inline ARY* This() {
    return TPtr<ARY>(aobj_.Origin());
  }

  /* Prints this object to a Printer. */
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return TArrayPrint<Printer, T, ISZ>(o, This());
  }

  /* Gets a reference to the given element index. */
  inline T& operator[](ISZ index) { return Begin()[index]; }

  /* Gets a const reference to the given element index. */
  inline const T& operator[](ISZ index) const { return Begin()[index]; }
};

}  //< namespace _
#endif
#endif