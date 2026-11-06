// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsLibraryHPP
#define _CrabsLibraryHPP
#include <_Config.h>
#if _Seam >= _CrabsRoom
namespace _ {
#if _CrabsUsingText
template<typename TIndex, typename TKey, typename TData, ISC MaxStackHeight>
class Library : public Operand {
 public:
  /* Creates an empty automata. */
  Library()
  :num_members_(0),
   num_collisions_(0),
   header_size_(0),
   collisions_size_(0),
   data_size_(0)
  {
    foo();
  }

  virtual ~Library() {}

  /* Returns true if the current Book has a hash table. */
  BOL HasHashTable() { return collisions_size_ != 0; }

  /* Gets the number_ of keys in the current scope. */
  ISC GetBagType() { return 0; }

  /* Gets the number_ of keys in the current scope. */
  ISC GetNumKeys() { return num_members_; }

  /* Gets the size of the header in bytes. */
  ISC GetHeaderSize() { return header_size_; }

  /* Gets the data size in bytes. */
  ISC GetDataSize() { return data_size_; }

  /* Attempts to add the Page data into the AArray at the given BaseAddress.
  @return Returns nil upon success and an error CHA upon failure. */
  const Op* Add(IUA type, const CHA* key, void* data) {
    TIndex size_of_type = GetSizeOfType(type);
    if (size_of_type == 0) {
      return 0;
    }
    return 0;
  }

  /* Attempts to insert the Page data into the AArray at the given index.
  @return Returns nil upon success and an error CHA upon failure. */
  const Op* Insert(IUA type, const CHA* key, void* data, TIndex index = 0) {
    TIndex l_numOps = numNumbers;
    if (index > l_numOps) index = l_numOps;

    return 0;
  }

  /* Attempts to remove the Page data into the AArray at the given index.
  @return Returns nil upon success and an error CHA upon failure. */
  const Op* Remove(TIndex index) { return 0; }

  /* Attempts to clear the page at the given index.
  @return Returns nil upon success and an error CHA upon failure. */
  const Op* Clear(TIndex index) { return 0; }

  /* Attempts to find the given op name.
  @return Returns an invalid index upon failure. */
  TIndex Find(const CHA* key) { return 0; }

  /* Searches for the given query and returns a bag of query results.  */
  BOL Search(const CHA* query,
             Library<TIndex, TKey, TData, TData, MaxStackHeight>* results) {
    return false;
  }

  /* gets the size of the item at the given index. */
  IUA GetOpSize(TIndex index) { return 0; }

  /* Sets the size of the item to the given index to the new_size. */
  IUA SetOpSize(TIndex index, TData new_size) { return 0; }

  /* Returns the data address of the given op if it exists.
  @return Returns a pointer to one of the ChineseRoom error Strings upon
  failure. */
  void* GetDataAddress(TIndex index) {
#if _CrabsMemoryProfile >= 64
    IUD* IUD_ref = (IUD*)address;
#elif _CrabsMemoryProfile >= 32
    IUC* IUC_ref = (IUC*)address;
#elif _CrabsMemoryProfile >= 16
    IUB* IUB_ref = (IUB*)address;
#elif _CrabsMemoryProfile != 8
#error Index_t invalid size!
#endif

    IUA size = 0;

    IUA type = (*address) & 0x1f;

    IUN array_type = type >> 5;

    switch (array_type) {
      case 0:
#if _CrabsMemoryProfile >= 16
        /// Library format: { _IUA, _IUA, _IUB, _IUB }
        return size + sizeof(Library) + *(address + 1) * sizeof(IUA) +
               *(IUB_ref + 2) + *(IUD_ref + 4);
#else
        return 0;
#endif
      case 1:
#if _CrabsMemoryProfile >= 32
        /// Library format: { _IUA, _IUA, _IUB, _IUC }
        return size + sizeof(Bag32) + *(IUB_ref + 2) * sizeof(IUA) +
               *(IUC_ref + 4) + *(IUD_ref + 8);
#else
        return 0;
#endif
      case 2:
#if _CrabsMemoryProfile >= 64
        /// Library format: { _IUB, _IUB, _IUC, _IUD }
        return size + sizeof(Library) + *(IUB_ref + 2) * sizeof(IUA) +
               *(IUC_ref + 4) + *(IUD_ref + 8);
#else
        return 0;
#endif
      case 2: {
        break;
      }
    }
    return 0;
  }

  /* Shrinks the currently selected bag's socket to the min size. */
  void Shrink() {}

  /* Crabs Operations.
  @return       Returns nil upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure.
  @param index The index of the expression.
  @param crabs  The Crabs to read and write from. */
  virtual const Op* Star(CHC index, Crabs* crabs) {
    static const Op This = {"Library", OpFirst('A'), OpLast('A'),
                             "",        OpOperand,   0};

    switch (index) {
      case '?':
        return ExprEnquiry(crabs, This);
      case 'A': {
        static const Op This = {"Foo", Params<0>(), Params<0>(), "Foo.",
                                '(',   ')',         _NilP};
        return 0;
      }
      default:
        return 0;
    }
    return 0;
  }

 private:
  // NONCOPYABLE (Library)

  ISC reserved_;       //< Reserved for 64-bit memory alignment.
  Library** root_;     //< Pointer to the dynamically allocated bags.
  Library* bag_;       //< Currently selected bag.
  ISC index_,          //< Index of the currently selected bag.
      height_,         //< Number of bags on the stack.
      num_libraries_;  //< Number of libraries.
  IUA type_;           //< Current type of bag.
  TIndex num_keys_,    //< Current number_ of Star members.
      buffer_size_;    //< Current size of the header and names socket in bytes.
  TKey header_size_,   //< Current size of the header and names in bytes.
      collisions_size_;  //< Current size of the header and names socket in
                         // bytes.
  TData data_size_;      //< Current total size of the bag.
                         // Book<TIndex, TKey, TData, TData> bag;
};

/* Destructs the given bag. */
template<typename TIndex, typename TKey, typename TData, IUN MaxStaccSize>
void Delete(Library<TIndex, TKey, TData, TData, MaxStaccSize>* r) {
  if (r == _NilP) return;
  delete TRef<CHA>(r);
}
#endif  //< _CrabsMemoryProfile > 2

}  //< namespace _
#endif
#endif  //< _CrabsLibraryHPP
