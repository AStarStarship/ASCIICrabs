// Copyright AStarship <https://astarship.net>.
#pragma once 
#include <_Config.h>
#ifndef _CrabsStackHPP
#define _CrabsStackHPP
#if _Seam >= _CrabsStack
#include "Array.hpp"
#if _Seam == _CrabsStack
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

/* @defgroup Stack
ASCII Stack - contiguous LIFO memory
A fixed-size stack of POD items with ISZ index tracking.
Used for the Crabs expression stack, Wall storage, and
Slot offset maps. The T/A prefix convention: T=POD,
A=Autoject (RAMFactory-managed).
@{ */

template<SCK_A>
constexpr DTB CStackType() { return VTSCKBits | CATypePOD<T>(); }

/* Returns the begin element such that the buffer size equals end - begin. */
template<SCK_A>
inline T* TStackBegin(void* stack) {
  return TRef<T>(stack, sizeof(SCK));
}
template<SCK_A>
inline const T* TStackBegin(const void* sck) {
  return CRef<T>(TStackBegin<SCK_P>(CRef<void>(sck)));
}

/* Returns the first empty element of the stack. */
template<SCK_A>
inline const T* TStackTop(const SCK* sck, ISY count) {
  return TStackBegin<SCK_P>(sck) + count;
}
template<SCK_A>
inline T* TStackTop(SCK* sck, ISY count) {
  return CRef<T>(TStackTop<SCK_P>(CRef<SCK>(sck)), count);
}
template<SCK_A>
inline const T* TStackTop(const SCK* sck) {
  return TStackTop<SCK_P>(sck, sck->count);
}
template<SCK_A>
inline T* TStackTop(SCK* sck) {
  return TStackTop<SCK_P>(sck, sck->count);
}

/* Returns the element after the last element of the stack. */
template<SCK_A>
inline T* TStackEnd(void* sck, ISY total) {
  void* pointer = TRef<SCK>(sck);
  return TStackBegin<SCK_P>(sck) + total;
}
template<SCK_A>
inline const T* TStackEnd(const void* sck, ISY total) {
  return CRef<T>(TStackEnd<SCK_P>(CRef<void>(sck), total));
}
template<SCK_A>
inline T* TStackEnd(void* sck) {
  if(sck) return TStackBegin<SCK_P>(TRef<SCK>(sck)) + TRef<SCK>(sck)->total;
  return _NilP;
}
template<SCK_A>
inline const T* TStackEnd(const void* sck) {
  if (sck) return TStackBegin<SCK_P>(TRef<const SCK>(sck)) + TRef<const SCK>(sck)->total;
  return _NilP;
}

template<SCK_A>
inline ISZ TStackSizeOf(ISY count) {
  return count * sizeof(T) + sizeof(SCK);
}

/* Prints the given obj to the console. */
template<typename Printer, SCK_A>
Printer& TStackPrint(Printer& o, const SCK* sck) {
  ISY total = sck->total,
      count = sck->count;
  o << Linef("\n+---\n| TStack<T") << CSizeCodef<T>() 
    << ",IS" << CSizeCodef<ISZ>()
    << ">: bytes:" << TStackSizeOf<SCK_P>(total)
    << " total: " << total << " count:" << count << Linef("\n+---");

  const T* elements = TStackBegin<SCK_P>(sck);
  _DROut("\n     sck:0x" << Hexf(sck) << "\nelements:0x" << Hexf(elements) <<
         "   delta:" << TDelta<>(sck, elements) << "\nsizeof():" << sizeof(SCK));
  for (ISC i = 0; i < count; ++i) o << "\n| " << i << ".) " << *elements++;
  if (count == 0) o << "\n| Empty";
#ifdef _DThis
  o << Linef("\n+---") << Charsf(sck, TStackSizeOf<SCK_P>(sck->total)) << '\n';
#endif
  return o;
}

template<SCK_A>
CHA* TStackFill(void* origin, CHA fill_char=0, ISZ count_start=0) {
  SCK* sck = TRef<SCK>(origin);
  ISZ total = sck->total;
  if (count_start < 0 || count_start >= total) return _NilP;
  return ArrayFill(TStackBegin<SCK_P>(sck) + count_start,
                   sizeof(T) * (total - count_start), fill_char);
}

/* Copies an ASCII Stack contents from the origin to the destination. */
template<SCK_A>
inline SCK* TStackCopy(SCK* dest, ISZ dest_total, const SCK* src, ISZ src_count)
{
  if (dest_total < src_count) return _NilP;
  return TRef<SCK>(ArrayCopy(TStackBegin<SCK_P>(dest), dest_total * sizeof(T),
                   TStackBegin<SCK_P>(src), src_count * sizeof(T)));
}
template<SCK_A>
inline SCK* TStackCopy(SCK* dest, const SCK* src, ISZ src_count)
{
  if (IsError(dest)) return CRef<SCK>(dest);
  return TStackCopy<SCK_P>(dest, dest->total, src, src_count);
}
template<SCK_A>
inline SCK* TStackCopy(SCK* dest, ISZ dest_total, const SCK* src)
{
  if (IsError(src)) return CRef<SCK>(src);
  return TStackCopy<SCK_P>(dest, dest->total, src, src->count);
}

/* Copies an ASCII Stack from the origin to the destination. */
template<SCK_A>
inline SCK* TStackCopy(SCK* dest, const SCK* src) {
  ISZ src_total = src->total,
      src_count = src->count;
  _DAssert(src_count <= src_total);
  if (src_count > src_total) return _NilP;
  return TStackCopy<SCK_P>(dest, TStackTop<SCK_P>(dest), src, src_count);
}

// Utility to cast Autoject origin to SCK*.
template<SCK_A>
inline TStack<SCK_P>* TStackPtr(IUW* buffer) {
  return TRef<TStack<SCK_P>>(buffer);
}

// Utility to cast Autoject origin to SCK*.
template<SCK_A>
inline const SCK* TStackPtr(const IUW* buffer) {
  return TRef<const SCK>(buffer);
}

template<typename ISZ = ISW, typename DT = DTB, DT POD>
constexpr DT TStackType() {
  return (TBitCode<ISZ>() << ATypeVTBit0) | (_SCK << TBitCode<ISZ>()) | POD;
}

/* RAMFactoryStack function for the TStack.
template<typename ISZ = ISW, typename DT = DTB, DT Type = _ISN>
IUW* TStackFactoryStack(IUW* origin, ISW bytes) {
  if (bytes <= 0) return TRef<IUW>(TStackType<ISZ, DT, Type>());
  return RAMFactoryStack(origin, bytes);
} */

/* RAMFactoryHeap function for the TStack.
template<typename ISZ = ISW, typename DT = DTB, DT Type = _ISN>
IUW* TStackFactoryHeap(IUW* origin, ISW bytes) {
  if (bytes <= 0) return TRef<IUW>(TStackType<ISZ, DT, Type>());
  return RAMFactoryHeap(origin, bytes);
} */

// Calculates the size of a SCK in bytes based on the count.
template<SCK_A>
inline ISZ TStackBytesData(ISY total) {
  return total * sizeof(T);
}

// Calculates the size of a SCK in bytes based on the count.
template<SCK_A>
inline ISZ TStackBytes(ISY total) {
  return sizeof(SCK) + ISZ(total) * sizeof(T);
}
template<SCK_A>
inline ISZ TStackBytes(const SCK* sck) {
  return TStackBytes<SCK_P>(sck->total);
}

/* Gets the min size of a Stack with zero elements. */
template<SCK_A>
inline ISZ TStackSizeMin() {
  enum {
    StackCountMin = sizeof(T) > 8 ? 1 : 8 / sizeof(T),
    StackTotalMin = sizeof(SCK) + sizeof(T) * StackCountMin,
  };
  return StackTotalMin;
}

/* Gets the max number_ of elements in an obj with the specific index
width. */
template<SCK_A>
inline ISZ TStackSizeMax() {
  return ISZ((((~ISZ(0)) - ACPUWordMask) - ISZ(sizeof(SCK))) /
               ISZ(sizeof(T)));
}

/* The minimum obj size. */
template<SCK_A>
inline ISZ TStackSizeMin(ISY total) {
  ISZ count_upper_bounds = TStackSizeMax<SCK_P>();
  if (total > count_upper_bounds)
    total = count_upper_bounds;
  return ISZ(sizeof(SCK) + total * sizeof(T));
}

/* Initializes an obj from a preallocated socket who's size is a multiple of
8 bytes.
@param socket An obj of bytes large enough to fit the obj.
@return A dynamically allocated socket. */
template<SCK_A>
SCK* TStackInit(SCK* stack, ISY total) {
  if (IsError(stack) || total < 0) return _NilP;
  _DArrayFill(stack, ISW(TStackSizeOf<SCK_P>(total)));
  stack->total = total;
  stack->count = 0;
  return stack;
}

/* Resets the stack count. */
template<SCK_A>
void TStackReset(SCK* sck) {
  sck->count = 0;
  _DArrayFill(TStackBegin<SCK_P>(sck), TStackBytes<SCK_P>(sck) - sizeof(SCK));
}

/* Initializes an obj from a preallocated socket who's size is a multiple of
8 bytes.
@param socket An obj of bytes large enough to fit the obj.
@return A dynamically allocated socket. */
template<SCK_A>
SCK* TStackInit(SCK* stack, ISZ size, const T* items, ISY count) {
  if (IsError(stack) || !items || size <= 0 || count < 0) return _NilP;
  _DArrayFill(stack, ISW(TStackSizeOf<SCK_P>(size)));

  stack->total = size;
  stack->count = 0;
  return stack;
}

/* Checks if the given index exists in the stack. */
template<SCK_A>
BOL TStackInBounds(const SCK* stack, ISY index) {
  return index >= 0 && index < stack->total;
}

/* Gets the element at the given index.
@param obj    The obj.
@param index The index of the element to get.
@return -1 if a is nil and -2 if the index is out of bounds. */
template<SCK_A>
T TStackGet(const SCK* stack, ISY index) {
  _AAssert(stack);
  if (index < 0 || index >= stack->count) return 0;
  return TStackBegin<SCK_P>(stack)[index];
}

template<SCK_A>
T TStackGet_NC(const SCK* stack, ISY index) {
  return TStackBegin<SCK_P>(stack)[index];
}

/* Peeks at the top item on the stack without popping it off. */
template<SCK_A>
T TStackPeek(const SCK* sck, ISY count) {
  _AAssert(sck);
  if (count <= 1) return 0;
  return *(TStackTop<SCK_P>(sck, count) - 1);
}
template<SCK_A>
T TStackPeek(const SCK* sck) {
  _AAssert(sck);
  return TStackPeek<SCK_P>(sck, sck->count);
}

/* Adds the given item to the stop of the obj.
@return The index of the newly stacked item or -1 upon failure.
@param stack The Ascii Object base poiner.
@param item  The item to push onto the obj. */
template<SCK_A>
inline void TStackPush_NC(SCK* stack, T item, ISY count) {
  *TStackTop<SCK_P>(stack, count) = item;
  stack->count = count + 1;
}

/* Adds the given item at the index.
@return The index of the newly stacked item or -1 upon failure. */
template<SCK_A>
inline ISY TStackInsert(T* begin, ISY total, ISY count, T item, ISY index = PSH) {
  if (index < 0) index = count;
  if (index >= total || total < 0 || count < 0 || count >= total)
    return -AErrorInvalidIndex;
  TArrayInsert_NC<SCK_P>(begin, count, item);
  return index;
}
template<SCK_A>
inline ISY TStackInsert(SCK* stack, T item, ISY index = PSH) {
  //_DCheckPointerTReturn(ISY, stack);
  if (index < PSH) return -AErrorInvalidIndex;
  ISY count = ISY(stack->count),
      total = ISY(stack->total);
  if (count >= total) return -AErrorStackOverflow;
  if (index == PSH) index = count;
  _DROut("  Pushing:" << item << " total:" << total << " count:" << count);
  _DROut("\n| Before:" << Charsf(stack, TStackSizeOf<SCK_P>(total)));
  TArrayInsert_NC<T, ISZ>(TStackBegin<SCK_P>(stack), count++, index, item);
  stack->count = count;
  return index;
}

/* Adds the given item to the stop of the obj.
@return The index of the newly stacked item or -1 upon failure.
@param stack The Ascii Object base poiner.
@param item  The item to push onto the obj. */
template<SCK_A>
inline ISY TStackInsertDown(T* start, ISY total, ISY count, T item, 
  ISY index = PSH) {
  if (count >= total) return -AErrorStackOverflow;
  if (index > 0) return -AErrorInvalidIndex;
  TArrayInsertDown_NC<T, ISZ>(start, count, item);
  return index;
}

/* Pops the top item off of the obj.
@note We do not delete the item at the
@param a The obj.
@return The item popped off the obj. */
template<SCK_A>
T TStackPop(SCK* stack) {
  _AAssert(stack);
  ISY count = stack->count;
  if (count == 0) return 0;
  T* items = TStackBegin<SCK_P>(stack);
  stack->count = count - 1;
  T item = items[count - 1];
  return item;
}

/* Shifts the elements down starting at the index element and deletes the index
element at the given index.
@pre You must perform bounds checking before calling this function. */
template<SCK_A>
void TStackRemove(T* elements, ISW element_number, ISW element_count) {
  return TArrayRemove<T>(elements + element_number,
                              elements + element_count, ISC(1));
}

/* Removes an element from the given array.
@return The new count of the stack. */
template<SCK_A>
inline ISZ TStackRemove(T* elements, ISY count, ISY index) {
  _AAssert(elements);
  if (index < 0 || index >= count) return -1;
  elements += index;
  T* elements_end = elements + --count;
  while (elements <= elements_end) *elements++ = *elements;
  return count;
}

/* Removes the given index from the obj.
@param stack The Ascii Object base poiner.
@param index The index the item to remove.
@return True if the index is out of bounds. */
template<SCK_A>
ISZ TStackRemove(SCK* stack, ISY index) {
  _AAssert(stack);
  ISZ result =
      TStackRemove<SCK_P>(TStackBegin<SCK_P>(stack), stack->count, index);
  if (result < 0) return result;
  stack->count = result;
  return result;
}

/* Returns true if the given obj contains the given address.
@return false upon failure. */
template<SCK_A>
BOL TStackContains(SCK* stack, void* address) {
  _AAssert(stack);
  CHA *pointer = TRef<CHA>(stack),
      *adr = TRef<CHA>(address);
  if (adr < pointer) return false;
  if (adr >= pointer + stack->size_array) return false;
  return true;
}

/* The obj size in words. */
template<SCK_A>
inline ISZ TStackSizeWords(ISY count) {
  return TStackSizeMin<SCK_P>(count) / sizeof(IUW);
}

/* An ASCII Stack Autoject.
@see ASCII Data Type Specification.
@link ./Spec/Data/MapTypes/Table.md

@code
 Stack Memory Map
+----------------+
|  Packed Stack  |
|----------------|
|  Word Aligned  |
|     Buffer     |
|----------------|
| Stack Elements |
|----------------|  +
|  Stack struct  |  |
+----------------+ 0x0
@endcode
*/
template<SCK_A, ISZ TotalInit_ = AStackTotalDefault,
         typename BOF = TBOF<TotalInit_, T, ISZ, TStack<SCK_P>>>
class AStack {
  Autobuffer<T, ISZ, BOF, TStack<SCK_P>> aobj_; //< An Auto-Object or Auto-Array.

  constexpr RAMFactory InitRamFactory() {
    return sizeof(BOF) == 0 ? TObjectFactory<ISZ>::StackHeap
                            : TObjectFactory<ISZ>::StackStack;
  }

  inline RAMFactory InitRamFactory(RAMFactory factory) {
    return !factory ? InitRamFactory() : factory;
  }

 public:
  /* Initializes a stack of n elements to whatever can fit in the BOF. */
  AStack() {
    Reset();
  }

  /* Attempts to down-size the BOF or creates a dynamically allocated stack.
  @param size The number of elements in the Array. */
  AStack(ISY total) : aobj_(TStackBytes<SCK_P>(total)) {
    Reset();
  }

  AStack(ISY total, RAMFactory ram) : aobj_(TStackBytes<SCK_P>(total), ram) {
    Reset();
  }

  /* Copies the items into the obj_.Buffer () with some additional
  empty_elements. */
  AStack(ISY total, const T* items, ISY items_count)
      : aobj_(TStackBytes<SCK_P>(items_count)) {
    Reset();
    TStackInsert<SCK_P>(This(), items, items_count);
  }

  /* Copies the items into the obj_.Buffer () with some additional
  empty_elements. */
  AStack(const T* items, ISY items_count)
      : aobj_(TStackBytes<SCK_P>(items_count)) {
    Reset();
    TStackInsert<SCK_P>(This(), items, items_count);
  }

  /* Destructs nothing. */
  ~AStack() {}

  inline void Reset() {
    return TStackReset<SCK_P>(This());
  }

  inline ISY Total() { return This()->total; }
  inline ISZ Count() { return This()->count; }
  inline ISZ CBytes() { return TStackBytes<SCK_P>(Total()); }
  inline ISZ SizeWords() { return CBytes() >> ACPUBytesLog2; }

  /* Gets the 16-bit ASCII Data Type for this Stack. */
  constexpr DTB CType() { return VTSCKBits | CATypePOD<T>(); }

  /* Gets this auto-object. */
  inline Autoject& AJT() { return aobj_.AJT(); }

  /* Returns the buffer_. */
  inline IUW* Origin() { return AJT().origin; }

  /* Sets the origin to the given pointer.
  @return Nil if the input is nil. */
  inline IUW* OriginSet(void* socket) {
    return OriginSet(aobj_, socket);
  }

  /* Returns true if the given origin pointer is a valid (non-nil)
     allocation. Used after a Clone/growth to check the RAM factory
     succeeded. A nil origin means the allocation failed. */
  inline BOL PtrIsValid(IUW* origin) const { return origin != _NilP; }

  /* Return the Origin base pointer as type T. */
  template<typename T_ = SCK>
  inline T_* As() { return TRef<T_>(Origin()); }

  /* Adds the given item to the stop of the obj.
  @return The index of the newly stacked item or -1 upon failure.
  @param stack The Ascii Object base poiner.
  @param item  The item to push onto the obj. */
  inline ISZ Insert(T item, ISY index = PSH) {
    SCK* src = This();
    if (index > src->total) return -AErrorInvalidIndex;
    Autoject& ajt = AJT();
  Insert:
    ISZ result = TStackInsert<SCK_P>(src, item, index);
    if (result < 0) {
      _DROut("\n\nError " << result << ": " << TAErrors<>(result) <<
        " at index:" << index << "\nAutogrowing...");
      Autoject dest = { _NilP, AJT().ram };
      src = Clone(dest, src);
      IUW* origin = dest.origin;
      if (IsError(origin))
        return -TDelta<ISZ>(dest.origin);
      ajt.origin = origin;
      ajt.ram = dest.ram;
      goto Insert;
    }
    return result;
  }

  /* Adds the given items to the stop of the stack.
  @return The index of the newly stacked item or -1 upon failure.
  @param stack       The Ascii Object base poiner.
  @param items       The  items to push onto the obj.
  @param items_count The number of items to push. */
  ISZ Insert(const T* items, ISY items_count) {
    _DAssert(items);
    _DAssert(items_count >= 0);
    _DROut("\nPushing " << items_count << " items:");
    SCK obj = This();
    ISY count = obj->count;
    auto result = 0;
    for (ISN i = 0; i < count; ++i) {
      result = Insert(items[i]);
      if (result < 0) return result;
    }
    return result;
  }

  /* Adds the given item to the stop of the obj.
  @return The index of the newly stacked item.
  @param item The item to push onto the obj. */
  inline ISZ Push(T item) { return Insert(item); }

  /* Pops the top item off of the obj.
  @return The item popped off the obj. */
  inline T Pop() { return TStackPop<SCK_P>(This()); }

  /* Removes the given index from the obj.
  @return True if the index is out of bounds.
  @param index The index the item to remove. */
  inline BOL Remove(ISY index) { return TStackRemove<SCK_P>(This(), index); }

  /* Creates a clone on the heap.
  @return _NilP if the inputs are invalue */
  inline static SCK* Clone(Autoject& dest, const SCK* src, ISZ total_new = -1, 
                           ISZ count_start = 0, ISZ count_stop = -1) {
    _DAssert(src);
    ISY total = src->total,
        count = src->count;
    _DAssert(total >= 0);
    _DAssert(count >= 0);
    _DAssert(count <= total);
    if (count_stop < 0) count_stop = count;
    if (total_new < 0) total_new = AutojectGrowTotal(total);
    if (total < 0 || count < 0 || count > total || count_start < 0 || 
        count_start >= count || count_stop > count)
      return _NilP;
#ifdef _DThis
    _DROut("\nAuto-growing Stack...\nBefore:");
    TStackPrint<ROut, T, ISZ>(RefOut(), src);
#endif
    ISZ count_new = count_stop - count_start,
        bytes_copy = count_new * sizeof(T),
        bytes      = sizeof(SCK) + total_new * sizeof(T);
    SCK* dest_sck = TRef<SCK>(dest.ram(_NilP, bytes));
    if (IsError(dest_sck))
      return dest_sck;
    *dest_sck = { total_new, count_new };
    _DROut("\nCopying data..." <<
      "\ncount_start:" << count_start << " count_stop:" << count_stop <<
      "\nstack_new.count: " << dest_sck->count <<
      "\nstack_new.total: " << dest_sck->total <<
      "\nbytes: " << bytes << "\nbytes_copy: " << bytes_copy <<
      "\nTStackStart<SCK_P>(stack): " << Hexf(TStackBegin<SCK_P>(src)));
    ArrayCopy(TStackBegin<SCK_P>(dest_sck), bytes_copy,
              TStackBegin<SCK_P>(src) + count_start, bytes_copy);
#ifdef _DThis
    _DROut("\nResult:");
    TStackPrint<ROut, T, ISZ>(RefOut(), dest_sck);
#endif
    dest.origin = TRef<IUW>(dest_sck);
    dest.ram = RAMFactory(dest.ram(_NilP, 0));
    dest_sck->total = total_new;
    dest_sck->count = count_new;
    return dest_sck;
  }

  /* Clones the other object including the total, growing if necessary.
  @return true upon success and false upon failure. */
  inline SCK* Clone(SCK* src) {
    if (IsError(src)) return _NilP;
    SCK* ths = This();
    ISZ this_total = ths->total,
        this_count = ths->count,
        src_total = src->total,
        src_count = src->count;
    if (this_total <= 0 || src_total <= 0 ||
        this_count <  0 || src_count <  0 ||
        this_count > this_total || src_count > src_total) {
      return TRef<SCK>(AErrorInvalidInput);
    }
    ISZ count_new = src_count + this_count;
    if (count_new > this_total) {
      Resize(count_new, count_new);
    }
    SCK* ths2 = This();
    TArrayCopy<ISZ>(TStackBegin<SCK_P>(ths2), this_total,
                    TStackBegin<SCK_P>(src), src_count);
  }

  inline SCK* Resize(ISZ total_new = -1, ISZ count_new = -1) {
    SCK* ths = This();
    if (count_new < 0) count_new = ths->count;

    ISY total = ths->total;
    if (count_new > total) return _NilP;

    if (total_new < 0) total_new = AutojectGrowTotal(total);
    else if (total_new < count_new) count_new = total_new;

    if (total_new <= total) {
      ths = { total_new, count_new };
      return ths;
    }

    Autoject& ajt = AJT();
    Autoject growth = { _NilP, ajt.ram };
  AttemptToResize:
    Clone(growth, ths, total_new, 0, count_new);
    IUW* origin_new = growth.origin;
    if (this->PtrIsValid(origin_new)) {
      Delete(ajt);
      ajt.origin = growth.origin;
      ajt.ram = growth.ram;
      return TRef<SCK>(origin_new);
    }
    _DAssert(false);
    goto AttemptToResize;
    return _NilP;
  }

  /* Checks if the given index is a valid element.
  @return true if the index is in bounds. */
  inline BOL InBounds(ISY index) {
    return TStackInBounds<SCK_P>(This(), index);
  }

  /* Gets the max number_ of elements in an obj with the specific index
  width. */
  inline ISZ SizeMax() { return TStackSizeMax<SCK_P>(); }

  /* Gets the min size of the entire Stack, including header, in bytes. */
  inline ISZ SizeMin() { return TStackSizeMin<SCK_P>(); }

  /* Gets a pointer to the first element in the obj. */
  inline T* Begin() { return TStackBegin<SCK_P>(This()); }

  /* Gets a pointer to the last element in the obj. */
  inline T* End() { return TStackEnd<SCK_P>(This()); }

  /* Peeks the top item off of the obj without popping it.
  @return The item popped off the obj. */
  inline T Peek() { return TStackPeek<SCK_P>(This()); }

  /* Gets the element at the given index.
  @return -1 if a is nil and -2 if the index is out of bounds.
  @param index The index of the element to get. */
  inline T Get(ISY index) { return TStackGet<SCK_P>(This(), index); }

  /* Returns true if the given obj contains the given address.
  @return false upon failure. */
  inline BOL Contains(void* address) {
    return TStackContains<SCK_P>(This(), address);
  }

  /* Resizes stack to the new_total, potentially shrinking the count or growing 
  if necessary. */
  inline SCK* Resize(ISZ total_new, ISZ count_start = 0, ISZ count_stop = 0) {
    SCK* t = This();
    ISY total = t->total,
        count = t->count;
    if (total_new < 0) total_new = AutojectGrowTotal(total);
    if (total_new <= count) {
      t->count = total_new;
      return t;
    }
    Autoject growth = { _NilP, AJT().ram };
    SCK* result = Clone(growth, t, total_new, count_start, count_stop);
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
  inline SCK* This() {
    return TRef<SCK>(aobj_.Origin());
  }

  /* Prints this object to a Printer. */
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return TStackPrint<Printer, T, ISZ>(o, This());
  }

  /* Prints this object to the given UTF. */
  inline void CPrint() { PrintTo<::_::ROut>(::_::RefOut()); }

  /* Gets a reference to the given element index. */
  inline T& operator[](ISY index) { return Begin()[index]; }

  /* Gets a const reference to the given element index. */
  inline const T& operator[](ISY index) const { return Begin()[index]; }
};

}  //< namespace _

/* @} */  // End of Stack group.

//template<SCK_A, typename BOF = ::_::Nil>
//::_::AStack<SCK_P, BOF>& operator<<(::_::AStack<SCK_P, BOF>& obj, T item) {
//  obj.Push(item);
//  return obj;
//}
//
//template<SCK_A, typename BOF = ::_::Nil>
//::_::AStack<SCK_P, BOF>& operator>>(T& item, ::_::AStack<SCK_P, BOF>& obj) {
//  item = obj.Pop();
//  return obj;
//}

#endif
#endif  //< _CrabsStackHPP
