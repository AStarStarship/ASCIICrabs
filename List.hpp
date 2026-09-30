// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_LIST_HPP
#define CRABS_LIST_HPP
#include <_Config.h>
#if SEAM >= CRABS_LIST
#include "Binary.hpp"
#include "Set.hpp"
#include "Stack.hpp"
#include "BSeq.hpp"
#if SEAM == CRABS_LIST
#include "_Debug.h"
#else
#include "_Release.h"
#endif
#define LST_A typename ISZ = ISC, typename ISY = ISB, typename DT = DTB
#define LST_P ISZ, ISY, DT
#define LST TList<ISZ, ISY, DT>
namespace _ {

/* A collection of type-value tuples.
@see ASCII Data Type Specification.
@link /Spec/Data/MapTypes/List.md
@code
      List Memory Layout
+============================+
|_______   Boofer            |<--- top
|_______ ^ Value N           |
|        | Value 0           |
|----------------------------|
|_______   Boofer            |
|_______ ^ Type of Value N   |
|        | Type of Value 0   |
|----------------------------|
|_______   Boofer            |
|_______ ^ Offset to Value N |
|        | Offset to Value 0 |
+============================+  ^  Up in addresses.
|        TList Struct        |  |
+============================+ 0xN Base address
@endcode
*/
template<LST_A>
struct TList {
  ISZ bytes,        //< Size of the List in bytes.
      top;          //< Offset to the top of the data.
  TStack<ISZ, ISZ, ISY> map;  //< Stack of offset mappings to the list items.
};

/* Gets a ASCII List Data Type. */
template<LST_A>
constexpr DT CListType() {
  return -_LS0 + CASizeCode<ISZ>();
}

/* Calculates the minimum size of a List with all nil objects. */
template<LST_A>
inline ISZ TListBytesMin(ISY total) {
  return sizeof(LST) + total * (sizeof(ISZ) + sizeof(DT));
}

template<LST_A>
inline BOL TListTopIsValid(ISZ bytes, ISZ top, ISY total) {
  return (top > TListBytesMin<LST_P>(total)) || (top < bytes);
}

template<LST_A>
inline BOL TListIsValid(ISZ bytes, ISZ top, ISY total, ISY count) {
  if (bytes < 0) return false;
  return (count >= 0) && (count <= total) && TListTopIsValid<LST_P>(bytes, top, total);
}

/* The maximum theoretical amount of freespace in a List. */
template<LST_A>
inline ISZ TListSizeFreeMax(ISZ bytes, ISY total) {
  return bytes - sizeof(LST) - 
         total * (sizeof(DT) + sizeof(ISZ));
}

/* Gets the base pointer to the Offsets. */
template<LST_A>
inline ISZ* TListValuesMap(LST* list) {
  return TPtr<ISZ>(list, sizeof(LST));
}
template<LST_A>
inline const ISZ* TListValuesMap(const LST* list) {
  const LST* lst = CPtr<LST>(list);
  return CPtr<ISZ>(TListValuesMap<LST_P>(CPtr<LST>(list)));
}

template<LST_A>
inline DT* TListTypes(LST* list, ISZ total) {
  return TPtr<DT>(TListValuesMap<LST_P>(list) + total);
}
template<LST_A>
inline const DT* TListTypes(const LST* list, ISZ total) {
  return CPtr<DT>(TListTypes<LST_P>(CPtr<LST>(list), total));
}

template<LST_A>
inline DT TListType_NC(const LST* list, ISZ total, ISY index) {
  return TListTypes<LST_P>(list, total)[index];
}

/* Returns the type bytes array. */
template<LST_A>
inline const DT* TListTypes(const LST* list) {
  return TPtr<DT>(TListValuesMap<LST_P>(list) + list->map.total);
}

/* Returns the type bytes array. */
template<LST_A>
inline DT* TListTypes(LST* list) {
  auto lst = CPtr<LST>(list);
  return CPtr<DT>(TListValuesMap<LST_P>(lst) + list->map.total);
}

/* Returns the type at the given index. */
template<LST_A>
inline DT TListType(const LST* list, ISY index) {
  if (index < 0 || index >= list->map->total) return NIL;
  return TListTypes<LST_P>(list)[index];
}

/* Gets the first element of the List. */
template<LST_A, typename T>
inline T* TListBase(const LST* list) {
  return TPtr<T>(list, sizeof(LST) +
                 list->map.total * (sizeof(ISZ) + sizeof(DT)));
}
template<LST_A, typename T>
inline const T* TListBase(const LST* list) {
  return TPtr<T>(list, sizeof(LST) +
                 list->map.total * (sizeof(ISZ) + sizeof(DT)));
}

/* Returns the type at the given index. */

/* Returns the value at the given index.
@return Returns nil if the index is out of the count range. */
template<LST_A, typename T = void>
inline const T* TListValue(const LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return NILP;
  return TPtr<T>(list, TListValuesMap<LST_P>(list)[index]);
}

/* Returns the value at the given index.
@return Returns nil if the index is out of the count range. */
template<LST_A, typename T = void>
inline T* TListValue(LST* list, ISY index) {
  return CPtr<T>(TListValue<LST_P>(CPtr<LST>(list), index));
}

/* Returns the value at the given index witout error checking. */
template<LST_A, typename T = void>
T* TListValue_NC(LST* list, ISY index) {
  return TPtr<T>(list, TStackGet_NC<ISZ, ISZ>(&list->map, index));
}

/* Returns the value at the given index.
@return Returns nil if the index is out of the count range. */
template<LST_A, typename T = void>
inline const T* TListValue_NC(const LST* list, ISY index) {
  return CPtr<T>(TListValue_NC<LST_P>(CPtr<LST>(list), index));
}

/* Returns the value at the given index witout error checking. */
template<LST_A>
const ISZ TListValueOffset_NC(const LST* list, ISY index) {
  return TStackBegin<TStack<ISZ, ISZ, ISY>>(&list->map)[index];
}

/* Returns the type and value offset at the given index. */
template<LST_A>
inline TATypeValue<ISZ> TListTypeValueOffset(LST* list, ISY index) {
  if (index < 0 || index >= list->map->total) return NIL;
  auto values = TListValuesMap<LST_P>(list);
  auto types = TPtr<DT>(values + list->map->total);
  return {types[index], values[index]};
}

/* Gets the base pointer to the values. */
template<LST_A>
inline CHA* TListValues(LST* list, ISZ size) {
  return TPtr<CHA>(TListTypes<LST_P>(list, size) + list->map.total);
}
template<LST_A>
inline CHA* TListValues(LST* list) {
  return TListValues<LST_P>(list, list->map->size);
}

template<LST_A>
inline void* TListTop(LST* list) {
  return TPtr<void*>(list, list->top);
}

/* Returns the TTypeValue at the given index.
@return Returns nil if the index is out of the count range. */
template<LST_A>
inline const TATypeValuePtr<DT> TListTypeValue(LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return { 0, 0 };
  auto vmap = TListValuesMap<LST_P>(list);
  auto types = TPtr<DT>(vmap + list->map.total);
  return { types[index], TPtr<void>(list, vmap[index])};
}

/* Returns the TTypeValue at the given index.
@return Returns nil if the index is out of the count range. */
template<LST_A>
inline const TATypeValue<ISZ, DT> 
    TListTypeValueOffset(const LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return { 0, 0 };
  auto vmap = TListValuesMap<LST_P>(list);
  auto types = TPtr<DT>(vmap + list->map.total);
  return { types[index], vmap };
}

// I don't have to have these.
// Prints the list to the output stream.
template<typename Printer, LST_A>
Printer& TListPrintTypeValue(Printer& o, const LST* list, ISY index) {
  return TPrintValue<Printer>(o, TListType<LST_P>(list, index),
                              TListValue<LST_P>(list, index));
}

// Prints the list to the output stream.
template<typename Printer, LST_A>
Printer& TListPrintValue(Printer& o, const LST* list, ISY index) {
  return TPrintValue<Printer, DT, ISZ>(o, TListTypeValue<LST_P>(list, index));
}

/* Prints the list to the output stream. */
template<typename Printer, LST_A>
Printer& TListPrint(Printer& o, const LST* list) {
  ISY count = ISY(list->map.count),
      total = ISY(list->map.total);
  o << Linef("\n\n+---\n| List<IS") << CSizeCodef<ISZ>()
    << "> bytes:" << list->bytes << " total:" << (total - 1)
    << " count:" << (count - 1) << Linef("\n+---");

  const ISZ* vmap  = TListValuesMap<LST_P>(list);
  const DT * types = TPtr<DT>(vmap + total);
  for (ISY index = 0; index < count; ++index) {
    o << "\n| " << index << " " << Centerf(index, STRLength(count) + 2);
    DT type = *types++;
    o << ATypef(type) << ' ';
    TPrintValue<Printer>(o, ATypeMakePtr(type), list, *vmap++);
  }
  return o << Linef("\n+---");
}

/* Returns the minimum count to align the data struct to a 64-bit boundary. */
template<LST_A>
constexpr ISZ TListTotalBoundsLower() {
  return 8 / sizeof(ISZ);
}

/* Initializes a AsciiList from preallocated memory. */
template<LST_A>
LST* TListInit(LST* list, ISY total) {
  ISZ bytes = list->bytes;
  D_OBJ_WIPE(list, ISZ);
  D_ASSERT(bytes > sizeof(LST));
  list->map.count = 0;
  list->map.total = total;
  ISZ top = sizeof(LST) + total * (sizeof(ISZ) + sizeof(DT));
  list->top = top;
  if (bytes <= top) return NILP;
  return list;
}

/* Returns the min size of an ASCII List with the given total.
The min size is defined as enough memory to store the given total with
the largest_expected_type.
*/
template<LST_A, ISW largest_expected_type = sizeof(ISW)>
constexpr ISY TListSizeMin(ISY total) {
  return sizeof(LST) +
         ISZ(total * (largest_expected_type + sizeof(ISZ) + 1));
}

/* Returns the end byte of the data. */
template<LST_A>
inline CHA* TListEnd(LST* list, ISZ size) {
  return TPtr<CHA>(list) + list->bytes;
  //return TPtr<CHA>(list) + sizeof(LST) + list->bytes;
  //@Todo Why does this not add the size of the struct like the Array?
}
template<LST_A>
inline CHA* TListEnd(LST* list) {
  return TListEnd<LST_P>(list, list->map.total);
}

/* Gets the first addrss in the Values section where you may be able to write
too. */
template<LST_A>
inline const CHA* TListValuesTop(const LST* list) {
  return TPtr<CHA>(list, list->top);
}
template<LST_A>
inline CHA* TListValuesTop(LST* list) {
  return CPtr<CHA>(TListValuesTop<LST_P>(CPtr<LST>(list)));
}

template<LST_A>
inline const CHA* TListValuesEnd(const LST* list) {
  return TPtr<CHA>(list, list->bytes);
}
template<LST_A>
inline CHA* TListValuesEnd(LST* list) {
  return CPtr<CHA>(TListValuesEnd<LST_P>(CPtr<LST>(list)));
}

/* Calculates the amount of space left in the list. */
template<LST_A>
inline ISZ TListSpace(const LST* list) {
  return list->bytes - list->top;
}
    /* Returns the max count an array can handle. */
template<LST_A>
ISZ TListSize() {
  enum {
    MaxISYes = sizeof(ISZ) == 1 ? 120
             : sizeof(ISZ) == 2 ? 8 * 1024
             : sizeof(ISZ) == 4 ? 512 * 1024 * 1024
             : 0,
  };
  return MaxISYes;
}

/* Deletes the list contents without wiping the contents. */
template<LST_A>
void TListClear(LST* list) {
  list->map.count = 1;
  list->top = sizeof(LST) + list->map.total * (sizeof(ISZ) + sizeof(DT));
}

/* Gets an element of the List by index. */
template<LST_A>
TATypeValue<ISZ, DT> TListTypeValue(const LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return { _NIL, NILP };
  auto type_ptr = *TPtr<DT>(list, sizeof(LST) +
    list->map.total * sizeof(ISZ) + index * sizeof(DT));
  return { *type_ptr, *TStackBegin<LST_P>(&list->map)[index] };
}

/* Gets an element of the List by index. */
template<LST_A>
ATypePtr TListTypeValuePtr(const LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return { _NIL, NILP };
  DTW type = DTW(*TPtr<DT>(list, sizeof(LST) +
    list->map.total * sizeof(ISZ) + index * sizeof(DT)));
  return { type, TPtr<void>(list, TStackBegin<LST_P>(&list->map)[index]) };
}

/* Gets a points to the element of the List by index. */
template<typename T = void, LST_A>
const T* TListValuePtr(const LST* list, ISY index) {
  if (index < 0 || index >= list->map.count) return NILP;
  return TPtr<T>(list, TStackBegin<ISA, ISZ>(&list->map)[index]);
}

/* Gets a points to the element of the List by index. */
template<typename T = void, LST_A>
T* TListValuePtr(LST* list, ISY index) {
  return CPtr<T>(TListValuePtr<T, LST_P>(CPtr<LST>(list), index));
}

/* I don't remember what this does, it looks like it might be erroneously
labeled from a botched copy and replace in all files. */
template<typename T, LST_A>
CHA* TListContains(const LST* list, ISZ sizeof_value,
                   ISZ align_mask = sizeof(T) - 1) {
  ISZ size = list->map.total, count = list->map.count;
  if (count >= size) return NILP;
  ISZ *vmap = TListValuesMap<LST_P>(list, size),  //
      *vmap_end = vmap + size;
  DT* types = TPtr<DT>(vmap_end);
  if (vmap == vmap_end) return NILP;
  CHA* previous_begin = 0;

  CHA* origin = TPtr<CHA>(list, *vmap++);
  DT type = *types++;
  origin =
      PtrUp(origin, ATypeAlignMask(type)) + TSizeOf<ISZ>(origin, type);
  CHA* end = origin;
  ISY index = 0;
  while (++vmap < vmap_end) {
    end = PtrUp(end, align_mask);
    CHA* origin = TPtr<CHA>(list, *vmap++);
    if (TDelta<>(end, origin)) return index;
    ++index;
    DT type = *types++;
    origin = PtrUp(origin, ATypeAlignMask(type));
    CHA* end = origin + TSizeOf<ISZ>(origin, type);
    previous_begin = origin;
  }
}

/* Searches the list->offsets for the address.
@return An invalid index if the list doesn't dontain the address or the index
of the object if the list contains the address. */
template<LST_A>
ISY TListContains(const LST* list, void* address) {
  if (TListValues<LST_P>(list)) return false;
  return true;
}

/* Finds a tuple that contains the given address. */
template<LST_A>
ISY TListFind(const LST* list, void* address) {
  CHA* adr = TPtr<CHA>(address);
  ISZ size = list->map->size;
  ISZ* vmap = TListValuesMap<LST_P>(list, size),
     * voffset_end = vmap + list->count;
  ISZ offset = TDelta<ISZ>(list, address), index = 0;
  while (vmap < voffset_end) {
    if (*vmap++ == offset) return index;
    ++index;
  }
  return -AErrorInvalidIndex;
}

/* Copies an ASCII List from the origin to the destination
template<LST_A>
struct TList {
  ISZ bytes,       //< Size of the List in bytes.
      top;              //< Offset to the top of the data.
  SCK offsets;  //< Stack of offsets to the list items.
};
      List Memory Layout
+============================+
|_______   Boofer            |
|_______ ^ Value N           |
|        | Value 0           |
|----------------------------|
|_______   Boofer            |
|_______ ^ Type of Value N   |
|        | Type of Value 1   |
|----------------------------|
|_______   Boofer            |
|_______ ^ Offset to Value N |
|        | Offset to Value 1 |
+============================+  ^  Up in addresses.
|        TList Struct        |  |
+============================+ 0xN Base address

@todo research if it's faster to just copy the Boofer.
*/
template<typename T, LST_A>
LST* TListCopy(const LST* origin, LST* destination) {
  ISZ bytes = origin->bytes;
  if (destination->bytes < bytes) return NILP;
  ISZ origin_count = origin->map.count,
      delta = TDelta<ISZ>(origin, destination);
  // 1. Copy Offsets.
  ArrayCopy(destination, bytes, origin, TStackTop<T, ISZ>(origin->map));
  // 2. Copy Types.
  ISZ r_start = TDelta<ISZ>(origin, TListTypes<LST_P>(origin)),
      size = origin_count * sizeof(DT);
  ArrayCopy(TPtr<>(destination, r_start), size, TPtr<>(origin, r_start), size);
  // 3. Copy Values.
  r_start = TDelta<ISZ>(origin, TListValues<LST_P>(origin)),
  size = TDelta<ISZ>(origin, destination->top);
  ArrayCopy(TPtr<>(destination, r_start), size, TPtr<>(origin, r_start), size);
  return destination;
}

// Returns the end address of the 
template<typename T, LST_A>
T* TListValueEnd(LST* list, ISY index) {
}

// Allocates memory for an ASCII Object of the given size.
// @return The index of the allocated type-value.
template<LST_A>
inline ISY TListAlloc(LST* list, DT type, ISZ bytes,
  ISY index = PSH) {
  ISA  align_mask = ATypeAlignMask(DTB(type));
  auto top = TAlignUp<ISZ>(list->top, align_mask);
  ISY  count = ISY(list->map.count),
    total = ISY(list->map.total);
  if (top + bytes > list->bytes || count >= total) return -1;
  *TPtr<ISZ>(list, top) = bytes;
  auto vmap = TListValuesMap<LST_P>(list);
  vmap[count] = top;
  auto types = TPtr<DT>(vmap + total);
  types[count++] = type;
  list->top = top + bytes;
  list->map.count = count--;
  return count;
}

/* Copies the source to the destination.
This function is different in that it
@return NILP if the destination does not have enough space. */
template<typename IS = ISR>
void* TATypeClone(void* destination, const void* source) {
  auto dst = static_cast<IS*>(destination);
  auto src = static_cast<const IS*>(source);
  auto dst_count = *dst;
  auto src_count = *src;
  if (dst_count < src_count) return NILP;
  auto result = ArrayCopy(dst, dst_count, src, src_count);
  if (result <= 0) return NILP;
  return static_cast<void*>(dst);
}
/* Writes the given value to the socket. */
template<typename IS = ISR>
void* TBSeqWriteCustom(void* begin, void* end, DTB type) {
  return begin;
}

/* Inserts the item into the list at the given index. */
template<LST_A>
inline ISY TListInsert_NC(LST* list, ISZ bytes, ISZ top, ISY total, ISY count, 
    DT type, IUW value, ISY index = PSH, IUW value_msb = 0) {
  ISZ align_mask = ATypeAlignMask(type);
  top = AlignUp(top, align_mask);
  if (index == count) {
    //value = AlignUp(value, align_mask); // @todo Should this be aligned?
    auto result = TBSeqWrite_NC<ISZ>(TPtr<IUA>(list, top),
      TPtrDown<IUA>(list, bytes, align_mask), type, value, value_msb);
    D_CHECK_PTR_TRETURN(ISY, -AErrorBooferOverflow);
    TListValuesMap<LST_P>(list)[count] = top;
    TListTypes<LST_P>(list)[count++] = ATypeMDDeassert(type);
    list->map.count = count;
    list->top = TDelta<ISZ>(list, result);
    return index;
  }
  D_COUT("\nInserting into index:" << index << " of count:" << count);
  A_ASSERT(false); // @todo Fix me!
  return -1;
}
template<LST_A>
inline ISY TListInsert(LST* list, DT type, IUW value, ISY index = PSH,
                       IUW value_msb = 0) {
  ISZ bytes = list->bytes,
      top   = list->top;
  ISY total = ISY(list->map.total),
      count = ISY(list->map.count);
  A_ASSERT(top > 0);
  if (index == PSH) index = count;
  if (index >= total)
    return -AErrorStackOverflow;
  if (index < 0 || index > count)
    return -AErrorInvalidIndex;
  D_COUT("\n  Inserting " << ATypef(type) << ":0d" << type << ":0x" <<
         Hexf(type) << " count:" << count << " index:" << index <<
         " top(aligned):" << top);
  list->map.count = count + 1;
  return TListInsert_NC<LST_P>(list, bytes, top, total, count, type, value,
                               index, value_msb);
}
template<LST_A>
inline ISY TListInsert(LST* list, DT type, const void* value, ISY index = PSH) {
  return TListInsert<LST_P>(list, type, IUW(value), index);
}

/* Searches for the first empty spot in the list that can fit the item and
inserts the item to the list at the given index.
@return An invalid index upon failure or the index of the index upon success. */

/* Inserts the item into the list at the given index. */
template<LST_A>
inline ISY TListInsert(LST* list, IUA item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _IUA, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, ISA item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _ISA, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, CHA item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _CHA, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, IUB item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _IUB, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, ISB item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _ISB, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, CHB item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _CHB, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, IUC item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _IUC, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, ISC item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _ISC, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, CHC item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _CHC, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, FPC item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _FPC, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, IUD item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _IUD, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, ISD item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _ISD, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, FPD item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _FPD, IUW(item), index);
}
template<LST_A>
inline ISY TListInsert(LST* list, BOL item, ISY index = PSH) {
  return TListInsert<LST_P>(list, _BOL, IUW(item), index);
}

/* Adds all of the items from the source to the list. */
template<LST_A>
LST* TListAppend(LST* list, const LST* source) {
  D_CHECK_PTR_RETURN(list);
  D_CHECK_CPTR_RETURN(LST, source);
  ISZ src_bytes = source->bytes,
      src_top   = source->top;
  ISY src_total = ISY(source->map.total),
      src_count = ISY(source->map.count);
  D_COUT("\nAdding " << src_count << " of " << src_total <<
         " max items...\nsource:\n");
  D_COUT_LIST(source);
  D_ASSERT(TListIsValid<LST_P>(src_bytes, src_top, src_total, src_count));
  const ISZ* src_vmap = TListValuesMap<LST_P>(source);
  const DT * src_types    = TListTypes<LST_P>(source, src_total);
  D_COUT("\nlist:0x" << Hexf(list));
  for (ISY i = 0; i < src_count; ++i) {
    ISZ voffset = *src_vmap++;
    DT  vtype   = *src_types++;

    D_COUT("\n\ni:" << i << " type:" << ATypef(vtype) << " voffset:" <<
           voffset);
    ISY result = TListInsert<LST_P>(list, ATypeMakePtr(vtype),
                                    IUW(ISW(source) + voffset));
    D_COUT(" result:" << result << ' ' << 
           (result < 0 ? TAErrors<CHA, ISY>(result) : " "));
    D_ASSERT(result >= 0);
  }
  return list;
}

/* Removes the item at the given address from the list. */
template<LST_A>
inline void* TListRemove(LST* list, ISY index) {
  ISY total = ISY(list->map.total),
      count = ISY(list->map.count);
  if (index == PSH) index = count - 1;
  if (index < 0 || index >= count) return NILP;
  ISZ* vmap = TListValuesMap<LST_P>(list);
  ISZ voffset = *(vmap + index);
  TArrayRemove<ISZ, ISY>(vmap, count, index);
  TArrayRemove<DT , ISY>(TListTypes<LST_P>(list, total), count--, index);
  list->map.count = count;
  return TPtr<void>(list, voffset);
}

/* Removes the item at the given address from the list. */
template<LST_A>
inline void* TListRemove(LST* list, void* address) {
  return TListRemove<LST_P>(list, TListFind(list, address));
}

/* Removes a type-value to the stop of the list. */
template<LST_A>
inline void* TListPop(LST* list) {
  return TListRemove<LST_P>(list, ISY(list->map.count) - 1);
}

/* ASCII Auto List that uses dynamic memory. */
template<LST_A, ISZ BytesInit_ = 512,
         typename BOF = TBOF<BytesInit_, IUA, ISZ, Nil>> 
class AList {
  AArray<IUA, ISZ, BytesInit_, BOF> obj_;  //< An Auto-array.
 public:
  static constexpr DT Type() { return CListType<LST_P>(); }

  /* Constructs a list with a given total with estimated bytes. */
  AList(ISY total)
      : obj_(BytesInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    TListInit<LST_P>(This(), total);
  }

  /* Constructs a List with the given bytes and total. */
  //AList(ISY total, ISZ bytes)
  //    : obj_(TListNew<LST_P>(bytes, total,
  //           TObjectFactory<ISZ>().template Init<BOF>()),
  //           TObjectFactory<ISZ>().template Init<BOF>()) {}

  /* Maximum count of the item in the List. */
  inline ISY Total() { return ISY(This()->map.total); }

  /* Count of the item in the List. */
  inline ISY Count() { return ISY(This()->map.count); }

  /* Count of the item in the List. */
  inline ISZ CBytes() { return This()->bytes; }

  /* Count of the item in the List. */
  inline ISZ BytesWords() { return TSizeWords<ISZ>(CBytes()); }

  /* Count of the item in the List. */
  inline ISZ Top() { return This()->top; }

  /* Returns true if this crabs contains only the given address.
  @warning This function assumes that the member you're checking for came from
  Script. If it's you're own code calling this, you are required to ensure the
  value came from a ASCII List.
  @return  True if the data lies in the list's memory array. */
  inline CHA* Contains(void* value) {
    return TListContains<ISZ>(This(), value);
  }

  /* Clears the list by setting the count to 0. */
  void Clear() { TListClear<LST_P>(This()); }

  /* Removes the item at the given address from the list. */
  inline BOL Remove(void* address) { return TListRemove<LST_P>(This(), address); }

  /* Removes the item at the given address from the list. */
  inline BOL Remove(ISY index) { return TListRemove<LST_P>(This(), index); }

  /* Searches for the first empty spot in the list that can fit the item and
  inserts the item to the list at the given index.
  @return An invalid index upon failure or the index of the index upon success.
  */
  inline ISY Insert(IUA value, ISY index = PSH) {
    return InsertTV(AJT(), _IUA, value, index);
  }
  inline ISY Insert(ISA value, ISY index = PSH) {
    return InsertTV(AJT(), _ISA, IUW(value), index);
  }
  inline ISY Insert(CHA value, ISY index = PSH) {
    return InsertTV(AJT(), _CHA, IUW(value), index);
  }
  inline ISY Insert(IUB value, ISY index = PSH) {
    return InsertTV(AJT(), _IUB, IUW(value), index);
  }
  inline ISY Insert(ISB value, ISY index = PSH) {
    return InsertTV(AJT(), _ISB, IUW(value), index);
  }
  inline ISY Insert(CHB value, ISY index = PSH) {
    return InsertTV(AJT(), _CHB, IUW(value), index);
  }
  inline ISY Insert(BOL value, ISY index = PSH) {
    return InsertTV(AJT(), _BOL, IUW(value), index);
  }
  inline ISY Insert(IUC value, ISY index = PSH) {
    return InsertTV(AJT(), _IUC, IUW(value), index);
  }
  inline ISY Insert(ISC value, ISY index = PSH) {
    return InsertTV(AJT(), _ISC, IUW(value), index);
  }
  inline ISY Insert(CHC value, ISY index = PSH) {
    return InsertTV(AJT(), _CHC, IUW(value), index);
  }
  inline ISY Insert(FPC value, ISY index = PSH) {
    return InsertTV(AJT(), _FPC, IUW(value), index);
  }
  inline ISY Insert(IUD value, ISY index = PSH) {
    return InsertTV(AJT(), _IUD, IUW(value), index);
  }
  inline ISY Insert(ISD value, ISY index = PSH) {
    #if CPU_SIZE == CPU_8_BYTE
    return InsertTV(AJT(), _ISD, IUW(value), index);
    #elif CPU_SIZE == 4_BYTE
    return InsertTV(AJT(), _ISD, IUW(value), index, IUW(value >> 32));
    #else
    return InsertTV(AJT(), _ISD, &value, index);
    #endif
  }
  inline ISY Insert(FPD value, ISY index = PSH) {
    return InsertTV(_FPD, IUW(value), index);
  }

  inline ISY InsertTV(DT type, const void* value, ISY index = PSH) {
    return InsertTV(AJT(), type, IUW(value), index);
  }

  /* Removes the last item from the list.
  @returns The address of the object just popped off the stack. */
  inline void* Pop() { return TListPop<LST_P>(This()); }

  /* Gets the obj_'s Autoject. */
  inline Autoject& AJT() { return obj_.AJT(); }

  /* Gets the Auto-Array. */
  inline AArray<IUA, ISZ>& AJT_ARY() { return obj_; }

  /* Returns the contiguous ASCII List boofer_. */
  inline LST* This() { return TPtr<LST>(AJT().origin); }

  /* Prints This object to the printer. */
  template<typename Printer>
  Printer& PrintTo(Printer& o) {
    return TListPrint<Printer, LST_P>(o, This());
  }

 private:

   /* Doubles the size and total of the list.
   @return Returns nil if the size is greater than the amount of memory that
   can fit in type ISW, the unaltered socket pointer if the Stack has grown to the
   size upper bounds, or a new dynamically allocated socket upon failure. */
   static BOL Grow(Autoject& obj) {
     D_COUT("\n\nGrowing List...");
     /* Grow Algorithm.
     1. Check if we can grow and if so, create a new block of memory.
     2. Copy the Loom.
     3. Copy the List. */
     auto origin = obj.origin;
     D_ASSERT(origin);
     auto source = TPtr<LST>(origin);
     ISZ  bytes  = source->bytes,
          top    = source->top;
     ISY  total  = ISY(source->map.total),
          count  = ISY(source->map.count);
     D_COUT("\nsize:" << bytes << " top:" << top << " total:" << total <<
            " count:" << count);
     auto bytes_new = AutojectGrowBytes(bytes);
     if (!AutojectCanGrow(bytes, bytes_new)) {
       D_COUT("\n\nError: bytes cannot grow! bytes:" << bytes <<
         " bytes_new:" << bytes_new);
       return false;
     }
     auto total_new = AutojectGrowTotal(total);
     if (!AutojectCanGrow(total, total_new)) {
       D_COUT("\n\nError: total cannot grow! total:" << total <<
         " total_new:" << total_new);
       return false;
     }
     IUW* origin_new = obj.ram(NILP, bytes_new);
     D_COUT("\n\n*TPtr<ISZ>(origin_new):" << *TPtr<ISZ>(origin_new) <<
       " size_new:" << bytes_new);
     auto destination = TPtr<LST>(origin_new);
     TListInit<LST_P>(destination, total_new);
     TListAppend<LST_P>(destination, source);
     obj.origin = origin_new;
     D_COUT("\n\nFinished growing. :-)\n\n");
     D_COUT(Charsf(origin_new, *TPtr<ISZ>(origin_new)));
     D_COUT_LIST(TPtr<LST>(obj.origin));
     return true;
   }

   /* Adds a string to the end of the List, auto-growing if necessary.
   @return The index upon success or -1 if the obj can't grow anymore.
   @todo Verify compile size of this function isolated and in the AArray class. */
   static ISY InsertTV(Autoject& obj, DT type, IUW value, ISY index = PSH, 
                       IUW value_msb = 0) {
     ISY result = TListInsert<LST_P>(TPtr<LST>(obj.origin), type, IUW(value), 
                                     index);
     while (result < 0) {
       if (!Grow(obj)) {
         return -AErrorBooferOverflow;
       }
       result = TListInsert<LST_P>(TPtr<LST>(obj.origin), type, IUW(value), 
                                   index);
       if (result < 0) {
         D_COUT("\n\n\nFailed to insert into list:" << result << ' ' <<
           ASCIIErrorSTA(result));
         return result;
       }
     }
     return result;
   }
};

}  //< namespace _

template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, CHA item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, ISA item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, IUA item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, CHB item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, ISB item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, IUB item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, BOL item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, FPC item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, IUC item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, ISC item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, CHC item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, ISD item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, IUD item) {
  list.Add(item);
}
template<LST_A>
inline ::_::LST& operator<<(::_::LST& list, FPD item) {
  list.Add(item);
}

#endif
#endif
