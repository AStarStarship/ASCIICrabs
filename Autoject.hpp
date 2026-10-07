// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_AUTOJECT_CODE
#define CRABS_AUTOJECT_CODE
#include "Autoject.h"
#include "Array.h"
#include "Binary.hpp"
#include "AType.hpp"
#if SEAM >= CRABS_UNIPRINTER
#if SEAM == CRABS_UNIPRINTER
#include "_Debug.h"
#else
#include "_Release.h"
#endif
namespace _ {

/* @defgroup ASCII Autoject
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/vector_types/array.md */

/* Returns the start of the OBJ. */
  template<typename T = CHA, typename ISZ = ISW, typename Class = ISZ>
  inline T* TObjectStart(void* obj) {
    IUW start = IUW(obj) + sizeof(Class);
    return TPtr<T>(start);
  }

  template<typename T = CHA, typename ISZ = ISW, typename Class>
  inline const T* TObjectStart(const void* obj) {
    return CPtr<T>(TObjectStart<T, ISZ, Class>(CPtr<void>(obj)));
  }
  
/* A word-aligned array of Size_ elements of T on the progam stack. */
template<ISW Size_ = ACPUCacheLineSize, typename T = IUA, typename ISZ = ISW,
         typename Class = Nil>
class TBOF {
 public:
  /* Default destructor does nothing. */
  TBOF() : words_() { *TPtr<ISZ>(words_) = Size(); }

  /* Returns the socket as a IUW*. */
  inline IUW* Words() { return words_; }

  /* Gets the end word of the socket. */
  inline IUW* WordsEnd() { return &words_[SizeWords()]; }

  /* Gets the stop word of the socket.
  inline IUW* WordsStop() { return WordsEnd() - 1; } */

  /* Gets the origin element of the socket. */
  template<typename T_ = CHA>
  inline T_* Begin() {
    return TPtr<T_>(words_);
  }

  /* Returns the first element of the ASCII Object data section. */
  template<typename RT = T>
  inline RT* Start() {
    ISW address = IUW(words_);
    return TPtr<RT>(address + sizeof(Class));
  }

  /* Gets the end element of the socket. */
  inline T* End() { return &words_[SizeWords()]; }

  /* Returns the last element of the ASCII Object data section.
  inline T* Stop() { return Start<>() + Size() - 1; } */

  /* Gets the origin of the socket. */
  template<typename T_ = CHA, typename ISZ_ = ISW>
  inline T_& Element(ISZ_ index) {
    if (index < 0 || index >= Size()) return NILP;
    return Begin()[index];
  }

  /* Sets the size to the new value. */
  template<typename ISW_>
  inline IUW* SizeSet(ISW_ size) {
    A_ASSERT((size & ACPUWordMask) == 0);
    *TPtr<ISZ>(words_) = size;
    return words_;
  }

  /* The size in either elements or bytes. */
  static constexpr ISZ Size() { return ISZ((Size_ < 0) ? 0 : ISZ(Size_)); }

  /* The size in words rounded down. */
  static constexpr ISZ SizeWords() { return CSizeWords<ISZ>(CBytes()); }

  /* The size in bytes including the header. */
  static constexpr ISZ CBytes() {
    return Size() * sizeof(T) + sizeof(Class);
  }

 private:
  IUW words_[SizeWords()];  //< The word-aligned socket.
};

/* A Block of heap. */
template<typename ISZ, typename T = CHA>
class TArrayFactory {
public:

  /* Does nothing. */
  TArrayFactory() {}

  /* RAMFactory Stack function allocates heap memory, returns the heap function,
  or returns the type. */
  static IUW* Stack(IUW* boofer, ISW total) {
    if (boofer == NILP)
      return reinterpret_cast<IUW*>(&Heap);
    if (total < 0) return NILP;
    ISW bytes = total * sizeof(T);
    if (bytes < sizeof(ISW)) bytes = sizeof(ISW);
    ISW size_words = AlignUp(bytes) >> ACPUBytesLog2;
    boofer = new IUW[size_words];
    *TPtr<ISZ>(boofer) = total;
    return boofer;
  }

  /* Deletes a non-nil boofer or calls Stack(boofer, bytes). */
  static IUW* Heap(IUW* boofer, ISW bytes) {
    if (boofer) {
      delete[] boofer;
      return NILP;
    }
    return Stack(boofer, bytes);
  }

  /* Gets the initial RAMFactory for the program stack or heap. */
  template<typename BOF>
  static constexpr RAMFactory RamFactoryInit() {
    return (sizeof(BOF) == 0) ? Heap : Stack;
  }

  /* Gets the RAMFactory to use upon construction. */
  template<typename BOF>
  inline RAMFactory Init() {
    return RamFactoryInit<BOF>();
  }

  /* Gets the RAMFactory to use upon construction. */
  template<typename BOF>
  inline RAMFactory Init(RAMFactory factory) {
    return (!factory) ? RamFactoryInit<BOF>() : factory;
  }
};

/* A Block of heap memory that starts with the size in bytes including the header. */
template<typename ISZ, typename T = CHA>
class TObjectFactory {
public:

  /* RAMFactory Stack function allocates heap memory, returns the heap function, or returns the type. */
  static IUW* Stack(IUW* boofer, ISW bytes) {
    if (boofer == NILP && bytes == 0) 
      return reinterpret_cast<IUW*>(&Heap);
    if(bytes < sizeof(ISZ)) return NILP;
    ISW size_words = AlignUp(bytes) >> ACPUBytesLog2;
    boofer = new IUW[size_words];
    *TPtr<ISZ>(boofer) = ISZ(bytes);
    return boofer;
  }

  /* Deletes a non-nil boofer or calls Stack(boofer, bytes). */
  static IUW* Heap(IUW* boofer, ISW bytes) {
    if (boofer) {
      delete[] boofer;
      return NILP;
    }
    return Stack(boofer, bytes);
  }

  /* Gets the initial RAMFactory for the program stack or heap. */
  template<typename BOF>
  static constexpr RAMFactory RamFactoryInit() {
    return (sizeof(BOF) == 0) ? Heap : Stack;
  }

 public:
  /* Does nothing. */
  TObjectFactory() {}

  /* Gets the RAMFactory to use upon construction. */
  template<typename BOF>
  inline RAMFactory Init() {
    return RamFactoryInit<BOF>();
  }

  /* Gets the RAMFactory to use upon construction. */
  template<typename BOF>
  inline RAMFactory Init(RAMFactory factory) {
    return (!factory) ? RamFactoryInit<BOF>() : factory;
  }
};

/* Deletes the given obj using the obj.factory. */
inline void Delete(Autoject& obj, IUW* origin = NILP) {
  RAMFactory ram = obj.ram;
  if (ram) ram(obj.origin, RAMFactoryDelete);
  obj.origin = origin;
}
inline RAMFactory RAMFactoryHeap(RAMFactory ram) {
  return RAMFactory(ram(NILP, 0));
}
inline RAMFactory RAMFactoryHeap(Autoject& obj) {
  return RAMFactory(obj.ram(NILP, 0));
}

/* Sets the size to the new_size*/
template<typename ISZ = ISW>
inline IUW* TSizeSet(IUW* origin, ISZ new_size) {
  *TPtr<ISZ>(origin) = new_size;
  return origin;
}

template<typename ISZ = ISW>
inline ISZ TSizeSet(Autoject& autoject, ISZ new_size) {
  return TSizeSet<ISZ>(autoject.origin, new_size);
}

/* Gets the ASCII Autoject size. */
template<typename ISZ = ISW>
inline ISZ TSize(IUW* origin) {
  return *TPtr<ISZ>(origin);
}

/* Gets the ASCII Autoject size. */
template<typename ISZ = ISW>
inline ISZ TSize(Autoject& autoject) {
  return TSize<ISZ>(autoject.origin);
}

/* Gets the ASCII Autoject size. */
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeBytes(ISZ size) {
  return sizeof(Class) + sizeof(T) * size;
}
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeBytes(IUW* origin) {
  return TSizeBytes<T, ISZ, Class>(TSize<ISZ>(origin));
}
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeBytes(Autoject& autoject) {
  return TSizeBytes<T, ISZ, Class>(autoject.origin);
}

/* Gets the ASCII Autoject size in CPU words. */
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeWords(ISZ size) {
  ISZ size_aligned_up = AlignUp(TSizeBytes<T, ISZ, Class>(size));
  return size_aligned_up >> ACPUBytesLog2;
}
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeWords(IUW* origin) {
  return TSizeWords<T, ISZ, Class>(TSize<ISZ>(origin));
}
template<typename T, typename ISZ, typename Class>
inline ISZ TSizeWords(Autoject& autoject) {
  return TSizeWords<T, ISZ, Class>(autoject.origin);
}

/* Dynamically allocates a new Array of the given size.
@return Nil upon failure or a pointer to the cloned autoject upon success.
@param socket A raw ASCII Socket to clone. */
template<typename T, typename ISZ = ISW, typename Class>
IUW* TObjectFactoryNew(RAMFactory factory, ISZ size) {
  IUW* origin = factory(NILP, TSizeBytes<T, ISZ, Class>(size));
  TSizeSet<ISZ>(origin, size);
  return origin;
}


/* Grows the size by up to double with exception of last grow,
which grows to max size.
@todo This design is because I'm having template issues.*/
inline ISA AutojectGrowBytes(ISA size) { return size + size; }
inline ISB AutojectGrowBytes(ISB size) { return size + size; }
inline ISC AutojectGrowBytes(ISC size) { return size + size; }
inline ISD AutojectGrowBytes(ISD size) { return size + size; }

/* Grows the total by up to double with exception of last grow,
which grows to max total. */
inline ISA AutojectGrowTotal(ISA total) { return total + total; }
inline ISB AutojectGrowTotal(ISB total) { return total + total; }
inline ISC AutojectGrowTotal(ISC total) { return total + total; }
inline ISD AutojectGrowTotal(ISD total) { return total + total; }

/* Checks of the given size is able to double in size.
@return True if the autoject can double in size. */
inline BOL AutojectCanGrow(ISA size, ISA new_size) {
  return new_size > size;
}
inline BOL AutojectCanGrow(ISB size, ISB new_size) {
  return new_size > size;
}
inline BOL AutojectCanGrow(ISC size, ISC new_size) {
  return new_size > size;
}
inline BOL AutojectCanGrow(ISD size, ISD new_size) {
  return new_size > size;
}

template<typename IS>
inline BOL TAutojectCanGrow(Autoject& obj) {
  IUW* origin = obj.origin;
  if (IsError(origin)) return false;
  IS size = *TPtr<IS>(origin);
  return AutojectCanGrow(size, AutojectGrowBytes(size));
}


/* A templated auto-object that has an optional boofer on the program stack. */
template<typename T, typename ISZ, typename BOF, 
         typename Class>
class Autoboofer {
  Autoject ajt_;  //< Automatic Object.
  BOF      bof_;  //< Optional boofer on the program stack.

 public:

  /* Writes the size to the given word-aligned-down socket, making a new one if
  required. */
  inline IUW* Init(IUW* boofer, ISZ size, RAMFactory ram) {
    D_ASSERT(ram);
    D_ASSERT(size >= CSizeMin<ISZ>());
    ajt_.ram = ram;
    if (IsError(boofer)) {
      ISZ boofer_size = AlignUp(ISZ(size * sizeof(T) + sizeof(Class)));
      boofer = ram(NILP, boofer_size);
    }
    D_RAM_WIPE(boofer, size * sizeof(T) + sizeof(Class));
    TSizeSet<ISZ>(boofer, size);
    ajt_.origin = boofer;
    return boofer;
  }

  /* Constructs. */
  Autoboofer() {
    Init(bof_.Words(), ISZ(bof_.Size()),
         TObjectFactory<ISZ>().template Init<BOF>());
  }

  /* Creates a autoject with either statically or dynamically allocated
  memory based on the size can fit in the boofer_. If the boofer_ is
  statically allocated but the size can't fit in the boofer a new block of
  dynamic memory will be created. */
  Autoboofer(ISZ size, RAMFactory ram = NILP) {
    Init(bof_.Words(), size,
         TObjectFactory<ISZ>().template Init<BOF>(ram));
  }

  /* Stores the origin and ram to the obj_. */
  Autoboofer(IUW* origin, RAMFactory ram) {
    ajt_.origin = origin;
    ajt_.ram = ram;
  }

  /* Destructor calls the RAMFactory factory. */
  ~Autoboofer() { Delete(ajt_); }

  /* Gets the boofer_. */
  inline BOF& Boofer() { return bof_; }

  /* Returns the boofer_. */
  inline IUW* Origin() { return ajt_.origin; }

  /* Sets the origin to the given pointer.
  @return Nil if the input is nil. */
  inline IUW* OriginSet(void* socket) { return OriginSet(ajt_, socket); }

  /* Gets the ASCII Object size in elements. */
  inline ISZ Size() { return TSize<ISZ>(ajt_.origin); }

  /* Gets the total ASCII Object size in bytes. */
  template<typename Class_ = void>
  inline ISZ CBytes() {
    return TSizeBytes<T, ISZ, Class_>(AJT());
  }

  /* Gets the total ASCII Object size in words. */
  template<typename Class_ = void>
  inline ISZ SizeWords() {
    return TSizeWords<T, ISZ, Class_>(AJT());
  }

  /* Returns the begin of the OBJ. */
  inline T* Begin() {
    return TPtr<T>(ajt_.origin, sizeof(Class));
  }

  /* Returns the start of the OBJ. */
  inline T* Start() {
    return Begin();
  }

  /* Gets the stop of the OBJ. */
  inline CHA* Stop() {
    return TPtr<CHA>(ajt_.origin) + CBytes() - 1;
  }

  /* Gets the Autoject. */
  inline Autoject& AJT() { return ajt_; }

  /* Attempts to grow the this autoject.
  @return false if the grow op failed. */
  inline BOL CanGrow() { return TAutojectCanGrow<ISZ>(ajt_); }

  /* Doubles the size of the obj_ array. */
  inline void Grow() {
    Grow(ajt_);
  }

  /* Returns true if the boofer is a Socket and false if it's a Nil. */
  static constexpr BOL UsesStack() { return sizeof(bof_) != 0; }

  /* Prints this autoject to the given printer. */
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return o;
  }

  //void CPrint() { PrintTo<::_::COut>(::_::StdOut()); }

  private:

  

  /* Resizes the given array to double it's prior capacity.
  @return True upon success and false upon failure. */
  static BOL Grow(Autoject& obj) {
    ISZ size = TSizeBytes<ISZ>(obj.origin);
    if (!TAutojectCanGrow<ISZ>(size)) return false;
    return Resize(obj, size << 1);
  }

  /* Resizes the given array.
  @return True upon success and false upon failure. */
  BOL Resize(Autoject& obj, ISZ new_size) {
    D_COUT("\nResizing Array to new_size:" << new_size);
    IUW* boofer = obj.origin;
    if (IsError(boofer)) return false;
    ISZ size = TSizeBytes<ISZ>(boofer);
    if (size < new_size) {
      if (size <= 0) return false;
      TSizeSet<ISZ>(boofer, new_size);
      return true;
    }
    IUW* new_begin = obj.ram(NILP, new_size);
    TSizeSet<ISZ>(new_begin, new_size);
    ArrayCopy(new_begin, boofer, new_size);
    obj.origin = new_begin;
    return true;
  }
};

} //< namespace _
#endif
#endif
