// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_BOOK_CODE
#define CRABS_BOOK_CODE
#include "List.hpp"
#if SEAM >= CRABS_BOOK
#if SEAM == CRABS_BOOK
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {
/* @defgroup Book
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Book.md

Please see the ASCII List documentation for information about
*/

#undef  BOK_P
#define BOK_P CHS, CHT, ISZ, ISY, DT
#undef  BOK_A
#define BOK_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISN,\
  typename ISY = ISM, typename DT = DTB

/* @defgroup Book
@brief A contiguous memory Associative List composed of a Loom and list.
The Loom datatype stores the bytes, which includes the size of the 
Keys Loom and the Values List. The Keys Loom is an array of strings that 
stores the Book Keys. The Values List is a list of type-value tuples that 
correspond to the key indexes.

The first element of every List ist a Loom (Array of Strings) that stores
the keys.

@code
+====================================+
|_______   Boofer                    |
|_______ ^ List Value N              |
|_______ | List Value 1              |
|        | Keys Loom (Value 0)       |
|------------------------------------|
|_______   Boofer                    |
|_______ ^ List Value N Type         |
|_______ | List Value 1 Type         |
|        | Loom Type (Type 0)        |
|------------------------------------|
|_______   Boofer                    |
|_______   Offset to Value N         |
|_______ ^ Offset to Value 1         |
|        | Offset to Keys (Offset 0) |
+====================================+  ^ Up in
|          TList Struct              |  |
+====================================+  + 0xN
@endcode */
template<BOK_A>
struct TBook {
  TList<LST_P> values;  //< A list of values with a Loom of keys in index 0.
};

#define BOK TBook<CHS, CHT, ISZ, ISY, DT>

enum {
  // The number of bits to shift a Book's bytes right by to calculate the 
  // size_keys (Example: size_keys = bytes >> 2).
  BookDefaultKeysFractionShift = -1,
  // The number of bits to shift a Book's bytes right by to calculate the 
  // total (Example: total = bytes >> 5).
  BookDefaultTotalFractionShift = -6,
  BookDefaultLengthKey = 15,
  BookDefaultSizeBytes = 512,
  BookDefaultCount = BookDefaultSizeBytes >> -(BookDefaultKeysFractionShift),
};

/* The minimum count a good with the given template parameters can have. */
template<BOK_A>
constexpr ISY CBookMinCount() {
  return 8 / sizeof(ISZ);
}

/* The minimum size of a Book based on the CCountMin. */
template<BOK_A>
constexpr ISZ CBookMinSize(ISY total) {
  return sizeof(BOK) + total * (sizeof(ISZ) + 2); //< @todo ???
}

/* Gets the default count of a good with the given template parameters. */
template<BOK_A>
constexpr ISZ CBookDefaultCount() {
  return 32;
}

/* Gets the default count of a good with the given template parameters. */
template<BOK_A>
ISY TBookCount(const BOK* book) {
  return book->values.map.count - 1;
}

/* Gets the default count of a good with the given template parameters. */
template<BOK_A>
ISY TBookTotal(const BOK* book) {
  return book->values.map.total - 1;
}

/* Gets the default count of a good with the given template parameters. */
template<BOK_A>
ISY TBookDefaultCount(ISY bytes) {
  return bytes >> 5; // >> 5 to / by 32
}

/* Gets the default size of a Book with the CBookDefaultCount. */
template<BOK_A>
constexpr ISZ CBookDefaultSize(ISY total = CBookDefaultCount<BOK_P>()) {
  if (total < 1) return -1;
  if (total < CBookMinCount<BOK_P>()) 
    total = CBookMinCount<BOK_P>();
  return total * BookDefaultLengthKey * sizeof(CHS);
}

/* Points to the base of the value offsets array. */
template<BOK_A>
inline ISZ* TBookValuesMap(BOK* book) {
  return TListValuesMap<LST_P>(&book->values);
}
template<BOK_A>
inline const ISZ* TBookValuesMap(const BOK* book) {
  return CPtr<ISZ>(TBookValuesMap<BOK_P>(CPtr<BOK>(book)));
}

/* Gets the keys Loom (index 0 of the ASCII List). */
template<BOK_A>
inline TLoom<LOM_P>* TBookKeys(BOK* book) {
  return TPtr<TLoom<LOM_P>>(book, *TBookValuesMap<BOK_P>(book));
}
template<BOK_A>
inline const TLoom<LOM_P>* TBookKeys(const BOK* book) {
  return CPtr<TLoom<LOM_P>>(TBookKeys<BOK_P>(CPtr<BOK>(book)));
}

/* Gets the values List. */
template<BOK_A>
inline ISZ* TBookKeysMap(BOK* book) {
  return TLoomKeysMap<LOM_P>(TBookKeys<BOK_P>(book));
}
template<BOK_A>
inline const ISZ* TBookKeysMap(const BOK* book) {
  return CPtr<ISZ>(TBookKeysMap<BOK_P>(CPtr<BOK>(book)));
}

/* Gets the start of the book as a templated-defined character. */
template<BOK_A>
inline CHS* TBookStart(BOK* book, ISY total) {
  ISZ* top = &TStackBegin<ISZ, ISZ>(TBookKeys<BOK_P>(book))[total];
  return TPtr<CHS>(top);
}
template<BOK_A>
inline const CHS* TBookStart(const BOK* book, ISY total) {
  return CPtr<ISZ>(TBookStart<BOK_P>(CPtr<BOK>(book), total));
}

/* Gets the start byte of the book. */
template<BOK_A>
inline CHS* TBookStart(BOK* book) {
  return TBookStart<BOK_P>(book, TBookKeys<BOK_P>(book).total);
}

/* Points to the byte after the last byte of the book's contiguous memory block. */
template<BOK_A>
inline CHA* TBookEnd(BOK* book) {
  return TListEnd<ISZ>(&book->values);
}
template<BOK_A>
inline const ISZ* TBookEnd(const BOK* book) {
  return CPtr<ISZ>(TBookEnd<BOK_P>(CPtr<BOK>(book)));
}

/* Gets the byte after the end of the book's contiguous memory block. */
template<BOK_A>
inline CHA* TBookKeysEnd(BOK* book) {
  return TLoomEnd<LOM_P>(TBookKeys<BOK_P>(book));
}

/* Gets the size of the Book in bytes. */
template<BOK_A>
inline ISZ TBookTotal(BOK* book) {
  return TBookKeys<BOK_P>(book)->map.total;
}
template<BOK_A>
inline const ISZ* TBookTotal(const BOK* book) {
  return CPtr<ISZ>(TBookTotal<BOK_P>(CPtr<BOK>(book)));
}

/* Checks if the book size values result are valid.
You must have enough room in the bytes with given total. */
template<BOK_A>
inline BOL TBookSizesAreValid(ISZ bytes, ISZ size_keys, ISY total) {
  return size_keys <= TListSizeFreeMax<LST_P>(bytes, total);
}

// Freespace left in bytes for the given book's keys Loom and values List.
// @return Returns -1 upon failure.
template<BOK_A>
inline ISZ TBookFreeSpace(BOK* book) {
  D_CHECK_TPTR_RETURN(ISZ, book);
  auto keys = TBookKeys<BOK_P>(book);
  D_CHECK_TPTR_RETURN(ISZ, keys);
  return TListSpace<LST_P>(&book->values) + 
         TLoomSpace<LOM_P>(keys);
}

/* Prints the book to the Printer. */
template<typename Printer, BOK_A>
Printer& TBookPrint(Printer& o, const BOK* book) {
  D_ASSERT_PTR(book);
  if (IsError(book))
    return o << "book ptr invalid";
  ISZ total       = book->values.map.total;
  ISZ count       = book->values.map.count;
  const ISZ* vmap = TBookValuesMap<BOK_P>(book);
  const DT* types = TPtr<DT>(vmap + total) + 1;
  ISZ keys_offset = *vmap++;
  auto keys       = TPtr<TLoom<LOM_P>>(book, keys_offset);
  auto keys_size  = keys->bytes;
  auto keys_start = keys->start;
  //D_COUT("\ntotal:" << total << " count:" << count << 
  //       " keys_size:" << keys_size << " keys_top : " << keys_start);
  //D_COUT_LOOM(keys);
  D_AVOW(total, keys->map.total);
  if (count != keys->map.count) {
    D_COUT_LOOM(keys);
    D_COUT_LIST(&book->values);
  }
  D_AVOW(count, keys->map.count);
  o << Linef("\n\n+---\n| Book<CH") << CSizeCodef<CHS>() 
    << ",IS" << CSizeCodef<ISZ>() << ",IS"
    << CSizeCodef<ISY>() << "> bytes:" << book->values.bytes
    << " total:" << (total - 1) << " count:" << (count - 1)
    << "\n| list_top:" << book->values.top << " keys_offset:" << keys_offset
    << " keys_space:" << TLoomSpace<LOM_P>(keys)
    << " values_space:" << TListSpace<LST_P>(&book->values)
    << "\n| keys.bytes:" << keys_size << " keys.top:" << keys_start
    << " TypeOf(keys):" << ATypef(types[0]) << Linef("\n+---");
  for (ISY i = 1; i < count; ++i) {
    o << "\n| " << i << ".) \"" << TLoomGet<LOM_P>(keys, i) << "\" type:";
    auto type    = *types++;
    auto voffset = *vmap++;
    o << ATypef(type) << " voffset:" << voffset << " value:"; // << " voffset:" << voffset;
    TPrintValue<Printer>(o, ATypeMakePtr(type), book, voffset);
  }
  o << Linef("\n+---");
  //D_COUT(Charsf(book, book->values.bytes));
  return o;
}

/* ASCII Data Type for the given templated BOK.
We are mapping an offset to a CH type, and thus the VT bits are 0.
@todo Look into if VT bits should be 0 and not 
| b15:b14 | b13:b9 | b8:b7 | b6:b5 | b4:b0 |
|:-------:|:------:|:-----:|:-----:|:-----:|
|   MOD   |   MT   |  SW   |  VT   |  POD  |
1. POD: Plain Old Data bits.
2. VT : Vector Type bits.
3. SW : Size Width bits.
4. MT : Map Type bits.
5. MOD: Modifier bits. */
template<BOK_A>
constexpr DTB CBookKeysType() {
  enum {
    PODBits = CATypeCH<CHR>(),
    VTBits  = 1 << ATypeVTBit0,
    SWBits  = CBitCount<ISZ>() << ATypeSWBit0,
    MTBits  = CATypeIS<ISY>() << ATypeMTBit0
  };
  return MTBits | SWBits | VTBits | PODBits;
}

/* Initializes the book to the given total using the CBookDefaultSize.
@param bytes The size of the book in bytes.
@param total The maximum number of entries in the book -1 for the keys.
@param size_keys If this number is positive then the value is size of the keys 
Loom in bytes. If the value is negative then it is the number of bits to shift 
the bytes to get the size_keys. */
template<BOK_A>
BOK* TBookInit(BOK* book, ISY total = BookDefaultTotalFractionShift,
               ISZ size_keys = BookDefaultKeysFractionShift) {
  D_ASSERT(book);
  TList<LST_P>* values = &book->values;
  ISZ bytes = values->bytes;
  D_COUT("\n\nTBookInit bytes: " << bytes << " total:" <<
         total << " size_keys:" << size_keys);
  D_RAM_WIPE(&values->top, bytes - sizeof(ISZ));
  if (size_keys < 0) {
    size_keys *= -1;
    if (size_keys >= TBitCount<ISZ>()) {
      D_COUT("\nInvalid total bit shift value.");
      return NILP;
    }
    size_keys = bytes >> size_keys;
    D_COUT("\nnew size_keys:" << size_keys);
  }
  if (total < 0) {
    total *= -1;
    if (total >= TBitCount<ISZ>()) {
      D_COUT("\nInvalid total bit shift value.");
      return NILP;
    }
    total = ISY(bytes >> ISZ(total));
    D_COUT("\nnew total:" << total);
  }
  if (!TBookSizesAreValid<BOK_P>(bytes, size_keys, total)) {
    D_COUT("\nInvalid book sizes!");
    return NILP;
  }
  TListInit<LST_P>(values, total);
  DTB KeysType = CBookKeysType<BOK_P>();
  ISY keys_index = TListAlloc<LST_P>(values, KeysType, size_keys);
  D_AVOW(ISY(0), keys_index);
  D_COUT("\nkeys_index  :" << keys_index);
  TLoom<LOM_P>* keys = TListValue<LST_P, TLoom<LOM_P>>(values, keys_index);
  D_COUT("\nkeys offset : " << TDelta<>(book, keys) << 
         "\nKeysType:0b" << Binaryf(KeysType) << ' ');
  // Expected Keys offset with ABook<ISB,IUB,ISA,CHA> with Bytes:512
  //   sizeof(TBook<ISB,IUB,ISA,CHA>) + 8*sizeof(IUB+DTB)
  // = 8 + 8*sizeof(IUB+DTB) = 8 + 32 = 40
  D_COUT(TPrintAType<COut>(StdOut(), KeysType));
  D_COUT("\nvalue_offsets_delta:" << TDelta<>(book, TBookValuesMap<BOK_P>(book)));
  D_COUT("\nkeys_delta:" << TDelta<>(book, keys));
  if (IsError(keys)) {
    D_COUT("\nBook Keys too large to fit in list! size_keys:" << size_keys);
    return NILP;
  }
  auto result = TLoomInit<LOM_P>(keys, total);
  TLoomInsert<LOM_P>(result, "");
  D_COUT("\nkeys->top:" << keys->start);
  D_COUT("\nTDelta<>(book, TBookKeys<BOK_P>(book)):" << 
         TDelta<>(book, TBookKeys<BOK_P>(book)));
  if (IsError(result)) return NILP;
  D_COUT("\n\nTListInit Post bytes: " << bytes << 
         " total:" << total << 
         "\nBook End  :" << TDelta<>(book, TBookEnd<BOK_P>(book)) <<
         "\nKeys End  :" << TDelta<>(book, TLoomEnd<LOM_P>(keys)) <<
         "\nKeys Start:" << TDelta<>(book, keys) << "\n\nResult:\n");
  D_COUT_BOOK(book);
  return book;
}

/* Gets the element at the given index. */
template<BOK_A>
inline void* TBookValue(BOK* book, ISY index) {
  return TListValue<void, LST_P>(TBookKeys<BOK_P>(book), index);
}
template<BOK_A>
inline const void* TBookValue(const BOK* book, ISY index) {
  return CPtr<DT>(TBookValue<BOK_P>(CPtr<BOK>(book), index));
}

/* Gets the TypeValue tuple the given index.
@param tuple Reference to the return type passed in by reference. */
template<BOK_A>
inline ATypePtr TBookATypeValue(const BOK* book, ISY index) {
  return TListTypeValuePtr<ISZ, DT>(&book->values, index);
}

template<BOK_A>
inline DT* TBookTypes(BOK* book, ISY total) {
  return TListTypes<LST_P>(&book->values, total);
}
template<BOK_A>
inline const DT* TBookTypes(const BOK* book, ISY total) {
  return CPtr<DT>(TBookTypes<BOK_P>(CPtr<BOK>(book), total));
}

/* Gets the key at the given index without doing any error checking. */
template<BOK_A>
inline CHS* TBookKey_NC(BOK* book, ISY index) {
  return TLoomGet_NC<LOM_P>(TBookKeys<BOK_P>(book), index);
}

/* Gets the key at the given index without doing any error checking. */
template<BOK_A>
inline const CHS* TBookKey_NC(const BOK* book, ISY index) {
  return CPtr<CHS>(TBookKey_NC<LOM_P>(CPtr<BOK>(book), index));
}

/* Gets the keys offsets array. */
template<BOK_A>
inline CHS* TBookKeys_NC(BOK* book, ISY index) {
  return TPtr<CHS>(book, *TPtr<ISZ>(book, sizeof(BOK)));
}
template<BOK_A>
inline const CHS* TBookKeys_NC(const BOK* book) {
  auto bok = CPtr<BOK>(TBookKeys<BOK_P>(book));
  return CPtr<CHS>(TBookKeys_NC<LOM_P>(bok));
}

/* Gets the type at the given index without doing any error checking. */
template<BOK_A>
inline DT TBookType_NC(const BOK* book, ISY total, ISY index) {
  return TListType_NC<LST_P>(&book->values, total, index);
}

/* Gets the keys offsets array. */
template<BOK_A>
inline DT* TBookTypes_NC(BOK* book, ISY total) {
  return TPtr<DT>(book, TBookKeys_NC<BOK_P>(book) + total);
}
template<BOK_A>
inline const CHS* TBookTypes_NC(const BOK* book) {
  auto bok = CPtr<BOK>(TBookValuesMap<BOK_P>(book));
  return CPtr<CHS>(TBookTypes_NC<LOM_P>(bok));
}

/* Gets the type at the given index without doing any error checking. */
template<BOK_A>
inline void* TBookValue_NC(BOK* book, ISY index) {
  return TListValue_NC<LST_P>(&book->values, index);
}
template<BOK_A>
inline const void* TBookValue_NC(const BOK* book, ISY index) {
  return CPtr<void>(TBookValue_NC<BOK_P>(CPtr<BOK>(book), index));
}

/* Gets the type at the given index without doing any error checking. */
template<BOK_A>
inline ISZ TBookValueOffset_NC(const BOK* book, ISY index) {
  //return TListValueOffset_NC<LST_P>(&book->values, index);
  return TPtr<ISZ>(book, sizeof(BOK))[index];
}

/* Gets the TypeValue tuple the given index.
@param tuple Reference to the return type passed in by reference. */
template<BOK_A>
inline TATypeKV<ISZ, DT, CHS> TBookTypeKV(const BOK* book, ISY index) {
  auto count = book->values.map.count;
  if (index < 0 || index >= count) 
    return { NILP, _NIL, 0 };
  return { TBookKey_NC<BOK_P>(book, index), 
           TBookType_NC<BOK_P>(book, book->values.map.total, index),
           TBookValueOffset_NC<BOK_P>(book, index) };
}

/* Adds a key-value tuple to the end of the Book.
@return The index upon success or -1 upon failure. */
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key, 
                       DT type, IUW value, ISY index = PSH, 
                       IUW msb = 0) {
  D_COUT("\nAdding \"" << key << "\" type:" << ATypef(type) << ":0d" << type <<
         ":0x" << Hexf(type) << ":\'" << CHA(type) << "\' value:0d" <<
         value << ":0x" << Hexf(value) << " msb:" << msb << " top:" << book->values.top);
  if (index < PSH) return -AErrorInvalidIndex;
  auto result = TListInsert<LST_P>(&book->values, type, value, index, msb);
  //D_COUT_LIST(&book->values);
  if (result < 0)
    return result;
  auto key_index = TLoomInsert<LOM_P>(keys, key, index);
  if (key_index < 0) 
    TListRemove<LST_P>(&book->values, index);
  D_COUT_BOOK(book);
  return key_index;
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, DT type, IUW value, 
  ISY index = PSH, IUW msb = 0) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, type, value, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, ISA item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISA, &item,
    index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  IUA item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISA, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, IUA item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISA, &item,
    index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  ISB item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISB, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, ISB item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISB, &item,
    index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  IUB item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISB, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, IUB item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISB, &item,
    index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  ISC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISC, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, ISC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISC, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  IUC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISC, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, IUC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISC, &item,
    index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  ISD item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _ISD, &item, index);
}

template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, ISD item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _ISD, &item,
    index);
}

template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key, IUD item,
  ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _IUD, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, IUD item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _IUD, &item, 
                            index);
}
#if USING_FPC == YES_0
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  FPC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _FPC, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, FPC item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _FPC, &item, 
                            index);
}
#endif
#if USING_FPD == YES_0
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHS* key,
  FPD item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _FPD, &item, index);
}
template<BOK_A>
inline ISY TBookInsert(BOK* book, const CHS* key, FPD item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, _FPD, &item,
    index);
}
#endif

#if USING_STA == YES_0
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHA* key,
  const CHA* item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _SWA, item, index);
}
#endif
#if USING_STB == YES_0
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHB* key,
  const CHB* item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _SWB, item, index);
}
#endif
#if USING_STC == YES_0
template<BOK_A>
inline ISY TBookInsert(BOK* book, TLoom<LOM_P>* keys, const CHC* key,
  const CHC* item, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, _SWC, item, index);
}
#endif

template<BOK_A>
inline ISY TBookInsertFrom(BOK* book, TLoom<LOM_P>* keys, 
    const CHS* key, DT type, ISZ value_offset, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, keys, key, type, 
    TPtr<>(&book->values, value_offset), index);
}
template<BOK_A>
inline ISY TBookInsertFrom(BOK* book, const CHS* key, DT type,
    const void* value, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, type, 
                            value, index);
}
template<BOK_A>
inline ISY TBookInsertFrom(BOK* book, const CHS* key, DT type,
    ISZ value_offset, ISY index = PSH) {
  return TBookInsert<BOK_P>(book, TBookKeys<BOK_P>(book), key, type, 
                            value_offset, index);
}

template<BOK_A>
inline ISY TBookInsert(BOK* book, TATypeKV<ISZ, DT, CHS> item, 
                       ISY index = PSH) {
  return TBookInsert<BOK_P>(book, item.key, item.tv.type, 
                            TPtr<>(book, item.tv.value));
}

/* Adds all of the items from the source to the book. */
template<BOK_A>
BOK* TBookAppend(BOK* book, const BOK* source) {
  D_CHECK_PTR_RETURN(book);
  D_CHECK_CPTR_RETURN(BOK, source);
  auto src_values = &source->values;
  auto src_size = src_values->bytes;
  auto src_top  = src_values->top;
  ISY src_total = ISY(src_values->map.total),
      src_count = ISY(src_values->map.count);
  D_COUT("\nAdding " << src_count << " of " << src_total << 
    " items...\nsource:0x" << Hexf(source) << '\n');
  D_COUT_BOOK(source);
  D_ASSERT(src_count >= 0 && src_count <= src_total && src_top > 0);
  TLoom<LOM_P>* keys   = TBookKeys<BOK_P>(book);
  const LOM* src_keys  = TBookKeys<BOK_P>(source);
  const ISZ* src_kmap  = TBookKeysMap<BOK_P>(source) + 1;
  const ISZ* src_vmap  = TBookValuesMap<BOK_P>(source) + 1;
  const DT * src_types = TBookTypes<BOK_P>(source, src_total) + 1;
  for (ISY i = 1; i < src_count; ++i) {
    D_COUT("\n\n" << i << ": ");
    const CHS* key = TPtr<CHS>(src_keys, *src_kmap++);
    ISZ voffset    = *src_vmap++;
    DT  vtype      = *src_types++;
    ISY result     = TBookInsert<BOK_P>(book, keys, key, ATypeMakePtr
    (vtype),
                                        IUW(ISW(src_values) + voffset));
    D_COUT("\nInsert result: " << result << ' ' << AErrorf(result));
    D_ASSERT(result >= 0);
  }
  return book;
}

/* Removes the given index from the Book.
@return Pointer to the still-existing data in the boofer. */
template<BOK_A>
void* TBookRemove(BOK* book, ISY index) {
  if (index < 1) return NILP;
  TList<LST_P>* list = &book->values;
  ISY count = ISY(list->map.count);
  ISZ* offsets = TListValuesMap<ISZ>(list);
  TStackRemove<ISZ, ISZ>(offsets, count, index);
  TStackRemove<DTB, ISZ>(TListTypes<ISZ, DTB>(list), count, index);
  TLoomRemove<LOM_P>(TBookKeys<BOK_P>(book), index);
  return offsets + index;
}

/* Removes the given key from the Book.
@return Pointer to the still-existing data in the boofer. */
template<BOK_A>
void* TBookRemove(BOK* book, const CHS* key) {
  ISY index = TLoomFind<BOK_P>(TBookKeys<BOK_P>(book), key);
  if (index < 0) return index;
  return TBookRemove<BOK_P>(book, index);
}

/* Adds a string to the end of the Book.
@return The index upon success or -1 upon failure. */
template<BOK_A>
ISZ TBookPop(BOK* book) {
  return TBookRemove<BOK_P>(book, TBookKeys<BOK_P>(book).map.count - 1);
}

template<BOK_A>
ISZ TBookCharCount(BOK* book) {
  return (ISZ)(TBookEnd<BOK_P>(book) - TBookStart<BOK_P>(book));
}
template<BOK_A>
BOL TBookWrite(BOK* destination, BOK* soure) {
  return true;
}

/* Adds a string to the end of the Book.
@return The index upon success or -1 upon failure. */
template<BOK_A>
CHS* TBookPop(BOK* book) {
  if (TBookKeys<BOK_P>(book).count == 0) return NILP;
  ISZ offset = TStackPop<ISZ, ISZ>(TBookKeys<BOK_P>(book)), top = book->top;
  book->top = offset;
  return TPtr<CHS>(ISW(book) + offset);
}

/* Searches for the given string in the book.
@return -1 if the book doesn't contain the string or the index if it does. */
template<BOK_A>
ISZ TBookFind(BOK* book, const CHS* string) {
  D_ASSERT(book);
  D_ASSERT(string);
  return TLoomFind<CHS, ISZ>(TBookKeys<BOK_P>(book), string);
}

/* An ASCII Book Autoject.
@code
ABook<BOK_A, 1024, TBuf<>>All
@endcode */
template<BOK_A, ISZ SizeInit = 512, 
         typename BOF = TBOF<SizeInit, CHS, ISZ, TString<ISN>>>
class ABook {
  AArray<IUA, ISZ, SizeInit, BOF> obj_;  //< An Auto-Array object.
 public:

  static constexpr DTB Type() {
    enum {
      //PODBits = _NIL,
      VTBits = 1 << ATypeVTBit0,
      SWBits = CBitCount<ISZ>() << ATypeSWBit0,
      MTBits = CATypeCH<ISY>() << ATypeMTBit0
    };
    return MTBits | SWBits | VTBits; // | PODBits;
  }
  enum {
    SizeBytesDefault = CBookDefaultSize<BOK_P>(),
    TotalDefault = SizeBytesDefault >> -BookDefaultTotalFractionShift,
    BitchWhat_DEZ_NUTZ_BABY = Type()
  };

  ABook(ISY total = BookDefaultTotalFractionShift,
        ISZ size_keys = BookDefaultKeysFractionShift)
    : obj_(SizeInit, TObjectFactory<ISZ>().template Init<BOF>()) {
    TBookInit<BOK_P>(This(), total, size_keys);
  }

  /* Constructs a Book on the Heap.
  @param factory RAMFactory to call when the String overflows. */
  ABook(RAMFactory ram, 
        ISY total = BookDefaultTotalFractionShift,
        ISZ size_keys = BookDefaultKeysFractionShift)
      : obj_(ram) {
    TBookInit<BOK_P>(This(), total);
  }

  /* Returns the size in elements. */
  inline ISZ Size() { return obj_.Size(); }

  /* Shorthand way to get the TBook->keys List. */
  inline TList<LST_P>* Values() { return &This()->values; }

  /* Gets the Keys struct. */
  inline TLoom<LOM_P>* Keys() { return TBookKeys<BOK_P>(This()); }

  /* Returns the size in bytes. */
  inline ISZ CBytes() { return Values()->bytes; }

  /* Returns the size in words. */
  inline ISZ SizeWords() { return obj_.SizeWords() >> ACPUBytesLog2; }

  /* Returns the number of keys. */
  inline ISZ Count() { return Values()->map.count; }

  /* Returns the maximum number of keys. */
  inline ISZ Total() { return Values()->map.total; }

  /* Inserts the key and item on into the Loom and List at the given index.
  @return The index of the string in the Book. */
  inline ISY Insert(const CHS* key, ISA value, ISY index = PSH) {
    return InsertTV(key, _ISA, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, IUA value, ISY index = PSH) {
    return InsertTV(key, _IUA, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, ISB value, ISY index = PSH) {
    return InsertTV(key, _ISB, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, IUB value, ISY index = PSH) {
    return InsertTV(key, _IUB, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, ISC value, ISY index = PSH) {
    return InsertTV(key, _ISC, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, IUC value, ISY index = PSH) {
    return InsertTV(key, _IUC, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, ISD value, ISY index = PSH) {
    return InsertTV(key, _ISD, IUW(value), index);
  }
  inline ISY Insert(const CHS* key, IUD value, ISY index = PSH) {
    return InsertTV(key, _IUD, IUW(value), index);
  }
#if USING_FPC == YES_0
  inline ISY Insert(const CHS* key, FPC item, ISY index = PSH) {
    return InsertTV(key, _FPC, &item, index);
  }
#endif
#if USING_FPD == YES_0
  inline ISY Insert(const CHS* key, FPD item, ISY index = PSH) {
    return InsertTV(key, _FPD, &item, index);
  }
#endif
#if USING_STA == YES_0
  inline ISY Insert(const CHS* key, const CHA* item, ISY index = PSH) {
    return InsertTV(key, _SWA, item, index);
  }
#endif
#if USING_STB == YES_0
  inline ISY Insert(const CHS* key, const CHB* item, ISY index = PSH) {
    return InsertTV(key, _SWB, item, index);
  }
#endif
#if USING_STC == YES_0
  inline ISY Insert(const CHS* key, const CHC* item, ISY index = PSH) {
    return InsertTV(key, _SWC, item, index);
  }
#endif
  /* Removes the string at the given index from the Book. */
  inline void* Remove(ISY index) { return TBookRemove<BOK_P>(This(), index); }

  /* Removes the string at the given index from the Book. */
  inline void* Remove(const CHS* key) {
    return TBookRemove<BOK_P>(This(), key);
  }

  /* Removes the string at the given index from the Book. */
  inline ISZ Pop() { return TBookRemove<BOK_P>(This()); }

  /* Gets the string at the given index. */
  inline CHS* Get(ISY index) { return TBookATypeValue<BOK_P>(This(), index); }

  /* Searches for the given string.
  @return -1 if the Book doesn't contain the string or the index if it does. */
  inline ISZ Find(const CHS* string) {
    return TBookFind<BOK_P>(This(), string);
  }

  /* Gets the Autoject. */
  inline Autoject& AJT() { return obj_.AJT(); }

  /* Gets the Auto-Array.
  inline AArray<IUA, ISZ, BOF>& AJT() { return obj_; } */

  /* Gets the ASCII Object. */
  inline BOK* This() { return this->obj_.template As<BOK>(); }

  /* Prints this object to the Printer. */
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return TBookPrint<Printer, BOK_P>(o, This());
  }

  /* Prints this object to the stdout. */
  inline void COut() { PrintTo<::_::COut>(::_::StdOut()); }

 private:
  
  /* Doubles the size and total of the book.
  @return Returns nil if the size is greater than the amount of memory that
  can fit in type ISW, the unaltered socket pointer if the Stack has grown to the
  size upper bounds, or a new dynamically allocated socket upon failure. */
  BOL Grow() {
    D_COUT("\n\nGrowing Book...");
    Autoject& obj = AJT();
    /* Grow Algorithm.
    1. Check if we can grow and if so, create a new block of memory.
    2. Copy the Loom.
    3. Copy the List. */
    auto origin = obj.origin;
    D_ASSERT(origin);
    auto source    = TPtr<BOK>(origin);
    auto size      = source->values.bytes;
    auto top       = source->values.top;
    ISY  total = ISY(source->values.map.total),
         count     = ISY(source->values.map.count);
    D_COUT("\nsize:" << size << " top:" << top << " total:" << total << 
           " count:" << count);
    auto size_new = AutojectGrowBytes(size);
    if (!AutojectCanGrow(size, size_new)) {
      D_COUT("\n\nError: keys_size cannot grow! keys_size:" << size <<
        " keys_size_new:" << size_new);
      return false;
    }
    auto total_new = AutojectGrowTotal(total);
    if (!AutojectCanGrow(total, total_new)) {
      D_COUT("\n\nError: total cannot grow! count:" << total << 
             " total_new:" << total_new);
      return false;
    }
    auto keys_size = TBookKeys<BOK_P>(source)->bytes;
    auto keys_size_new = AutojectGrowBytes(keys_size);
    if (!AutojectCanGrow(keys_size, keys_size_new)) {
      D_COUT("\n\nError: keys_size cannot grow! keys_size:" << keys_size << 
             " keys_size_new:" << keys_size_new);
      return false;
    }
    IUW* origin_new = obj.ram(NILP, size_new);
    D_COUT("\n\n*TPtr<ISZ>(origin_new):" << *TPtr<ISZ>(origin_new) << 
           " size_new:" << size_new);
    auto destination = TPtr<BOK>(origin_new);
    TBookInit<BOK_P>(destination, total_new, keys_size_new);
    TBookAppend<BOK_P>(destination, source);
    obj.origin = origin_new;
    D_COUT("\n\nFinished growing. :-)\n\n");
    D_COUT_BOOK(TPtr<BOK>(obj.origin));
    return true;
  }

  /* Adds a string to the end of the Book, auto-growing if necessary.
  @return The index upon success or -1 if the obj can't grow anymore.
  @todo Verify compile size of this function isolated and in the AArray class.
  @todo Why am I calling TBookInsert twice? */
  ISY InsertTV(const CHS* key, DT type, IUW value, 
      ISY index = PSH, IUW msb = 0) {
    Autoject& obj = AJT();
    D_CHECK_PTR_TRETURN(ISY, key);
    ISY result = TBookInsert<BOK_P>(TPtr<BOK>(obj.origin), key, type, value, 
                                    index, msb);
    while (result < 0) {
      if (!Grow()) {
        return -AErrorBooferOverflow;
      }
      BOK* book = TPtr<BOK>(obj.origin);
      result = TBookInsert<BOK_P>(book, key, type, value, index, msb);
      if (result < 0) {
        D_COUT("\n\n\nFailed to insert into book:" << result << ' ' <<
               ASCIIErrorSTA(result));
        D_COUT_LOOM(TBookKeys<BOK_P>(book));
        return result;
      }
      D_COUT("\ndez nutz baby!!!\n");
    }
    return result;
  }
};

}  //< namespace _
#endif
#endif
