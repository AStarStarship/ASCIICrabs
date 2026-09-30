// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_STRING_HPP
#define CRABS_STRING_HPP
#include <_Config.h>
#include "Stack.hpp"
#if SEAM >= CRABS_STRING
#include "Uniprinter.hpp"
#include "AType.h"
#if SEAM == CRABS_STRING
#include "_Debug.h"
#else
#include "_Release.h"
#define D_COUT_STRING(s)
#endif

#define STR_A typename T=CHR, typename CHT = CHD, typename ISZ=ISW, typename ISY=ISZ
#define STR_P T, CHT, ISZ, ISY
#define STR TString<T, CHT, ISZ, ISY>

//#define SND_A typename T = CHR, typename ISZ = ISC = ISC, typename ISY = ISB
//#define SND_P T, ISZ, ISY
//#define SND TRope<T, ISZ, ISY>
namespace _ {

/* A ASCII str created from an ASCII Stack that doesn't keep track of the 
number of Unicode characters. */
template<STR_A>
struct TString {
  ISZ total,  //< Total elements in the boofer.
      count;  //< Count of characters in the boofer.
};

/* A UTF-8 or UTF-16 ASCII str created from an ASCII Stack that keeps track 
of the number of Unicode characters. */
template<STR_A>
struct TRope {
  ISZ datum,  //< Memory alignment datum can be used for multiple purposes.
      charc,  //< Unicode Character Count.
      total,  //< Size of the str in bytes including the header.
      count;  //< Count of elements in the boofer.
};

/* The ASCII Data Type for the given T, CHA, CHB, or CHC. */
template<STR_A>
constexpr DTB CATypeCH() {
  return DTB((sizeof(T) == 1) ? _CHA :
             (sizeof(T) == 2) ? _CHB : 
             (sizeof(T) == 4) ? _CHC : 0);
}

/* Gets the begin character in the str. */
template<STR_A>
inline T* TStringBegin(STR* element) {
  return TStackBegin<SCK_P>(TPtr<TStack<SCK_P>>(element));
}
template<STR_A>
inline const T* TStringBegin(const STR* element) {
  return TStackBegin<SCK_P>(TPtr<TStack<SCK_P>>(element));
}

/* Gets the first character in the str.
template<STR_A>
inline T* TStringStart(STR* str, ISZ count) {
  return TStringBegin<STR_P>(str) + count;
}
template<STR_A>
inline const T* TStringStart(const STR* str, ISZ count) {
  return CPtr<STR>(TStringStart<STR_P>(CPtr<STR>(str), str->count));
}
template<STR_A>
inline T* TStringStart(STR* str) {
  return TStringStart<STR_P>(str, str->count);
}
template<STR_A>
inline const T* TStringStart(const STR* str) {
  return TStringStart<STR_P>(str, str->count);
} */

/* Gets the stop char of the str.
template<STR_A>
inline T* TStringStop(STR* origin, ISW total) {
  return TStringBegin<SCK_P>(origin) + total;
}
template<STR_A>
inline const T* TStringStop(const STR* origin, ISW total) {
  return CPtr<T>(TStringStop<STR_P>(CPtr<STR>(origin), total));
}
template<STR_A>
inline T* TStringStop(STR* str) {
  return TStringStop<SCK_P>(str, str->total);
}
template<STR_A>
inline const T* TStringStop(const STR* str) {
  return TStringStop<SCK_P>(str, str->total);
} */

/* Gets the stop char of the str. */
template<STR_A>
inline T* TStringEnd(STR* element, ISY total) {
  return TStackBegin<SCK_P>(element, total);
}
template<STR_A>
inline const T* TStringEnd(const STR* element, ISY total) {
  return TStackEnd<SCK_P>(element, total);
}

/* Searches for the stop of the str. */
template<STR_A>
inline T* TStringEnd(STR* element) {
  return TStackEnd<SCK_P>(element, element->total);
}
template<STR_A>
inline const T* TStringEnd(const STR* element) {
  return TStackEnd<SCK_P>(element, element->total);
}

/* Calculates the size of the string in bytes. */
template<STR_A>
inline ISZ TStringBytes(const STR* element, ISZ total) {
  return sizeof(STR) + total * sizeof(T);
}
template<STR_A>
inline ISZ TStringBytes(const STR* element) {
  return TStringBytes<STR_P>(element, element->total);
}

template<STR_A>
inline T* TStringReset(STR* element) {
  D_STR_WIPE(element);
  D_ASSERT_PTR(element);
  T* begin = TStringBegin<STR_P>(element);
  *begin = 0;
  element->count = 1;
  return begin;
}

/* Initializes an ASCII String. */
template<STR_A>
inline STR* TStringInit(void* origin, ISZ total) {
  STR* element = TPtr<STR>(origin);
  D_ASSERT_PTR(element);
  D_ASSERT(total >= 0);
  element->total = total;
  TStringReset(element);
  return element;
}

/* Prints this object to the given printer. */
template<typename Printer, STR_A>
Printer& TStringPrint(Printer& o, const STR* element) {
  if (IsError(element)) return o;
  const T* begin = TStringBegin<STR_P>(element);
  ISZ total = element->total,
      count = element->count,
      bytes = TStringBytes<STR_P>(element);
  o << Linef("\n+---\n| TString<CH") << CSizeCodef<T>() << ",IS"
    << CSizeCodef<ISZ>() << "> total:" << total << " count:" << count
    << " bytes:" << bytes << Linef("\n+---\n| \"");
  ISW column_count = AConsoleWidth;
  ISZ length = 0;
  CHL c = 0;
  const T* cursor = SScan(begin, c);
  while (c) {
    ISW column = 2;
    while (++column < column_count) {
      ++length;
      o << c;
      cursor = SScan(cursor, c);
      if (!c) goto PrintBottomLine;
    }
    o << "\n| ";
  }
PrintBottomLine:
  #ifdef D_THIS
  o << Linef("\"\n+---") << Charsf(begin, bytes);
  #endif
  return o << Linef("\"\n+---");
}

template<STR_A>
CHA* TStringFill(STR* obj, CHA fill_char = 0, ISZ count_start = -1) {
  CHA* result = TStackFill<SCK_P>(obj, fill_char);
  *TStringBegin<STR_P>(obj) = 0;
  return result;
}

/* Gets the size from the str at the given origin address. */
template<STR_A>
inline ISZ TStringSize(void* origin) {
  return *TPtr<ISZ>(origin);
}

/* An ASCII String that can auto-grow from stack to heap.

The count of the str is defined as the maximum chars that can fit in the
boofer, including the nil-term CHA.

This class is designed to take advantage of the behavior of the C++ operator
overloads. When you have a str of overloads, the objects get destructed
in the opposite order then where called.

# Initialization

A String may be initialed to print to the socket or to a dynamically allocated
str. This behavior is configured with the constructors. The RAMFactory can
either be configured using the class template argument cFactory1_. If the
obj_.Factory () is nil then it will get replaced with the foo.

#

@code
AString<> (TBOF<64>).Star ("Hello ") << "world!";
@endcode

# Dynamic Allocated Strings

Strings that use dynamic memory use the TC:

@code
AString<IUC, TBOF<64>> () << "Hello world!";
@endcode
*/
template<STR_A, ISZ TotalInit_ = ASTRCount,
          typename BOF=TBOF<TotalInit_, T, ISZ, STR>>
class AString {
  AStack<SCK_P, TotalInit_, BOF> asck_;  //< AutoStack of T(s).

 public:
  static constexpr DTB Type() {
    return CATypeVector(CATypeCH<T>(), _ARY, CATypeSW<ISZ>());
  }

  /* Constructs a String that auto-grows from stack to heap.
  @param factory RAMFactory to call when the String overflows. */
  AString() : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
  }

  /* Constructs a String and prints the given item. */
  AString(CHA item) : 
      asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(const CHA* item) : 
      asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

#if USING_STB == YES_0
  /* Constructs a String and prints the given item. */
  AString(const CHB* item) : 
    asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }
#endif

#if USING_STC == YES_0
  /* Constructs a String and prints the given item. */
  AString(CHC item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }
#endif
  /* Constructs a String and prints the given item. */
  AString(const CHC* item) : 
    asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(ISA item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(IUA item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(ISB item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(IUB item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(ISC item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(IUC item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(ISD item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(IUD item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

#if USING_FPC == YES_0
  /* Constructs a String and prints the given item. */
  AString(FPC item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }
#endif
#if USING_FPD == YES_0
  /* Constructs a String and prints the given item. */
  AString(FPD item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }
#endif

  /* Constructs a String and prints the given item. */
  AString(Hexf item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Constructs a String and prints the given item. */
  AString(Binaryf item) : asck_(TotalInit_, TObjectFactory<ISZ>().template Init<BOF>()) {
    Init();
    Print(item);
  }

  /* Returns the String Stack total. */
  inline ISZ Total() { return This()->total; }

  /* Returns the String Stack count. */
  inline ISZ Count() { return This()->count; }

  /* Returns the String Stack count. */
  inline ISZ CBytes() { return sizeof(STR) + Total() * sizeof(T); }

  /* Gets the total ASCII Object size in words. */
  inline ISZ SizeWords() { return CBytes() >> ACPUBytesLog2; }

  /* Resets the sprinter_ to the given word-aligned boofer. */
  inline T* Init() {
    return TStringBegin<STR_P>(TStringInit<STR_P>(This(), TotalInit_));
  }

  /* Resets the sprinter_ to the given word-aligned boofer. */
  inline T* Reset() {
    return TStringReset<STR_P>(This());
  }

  /* Wipes the given */
  inline void WipeFreeSpace() { TStackFill<SCK_P>(); }

  /* Wipes the given */
  inline void Wipe() {
    Reset();
    WipeFreeSpace();
  }

  /* Gets the UTF.
  inline TSPrinter<SCK_P>& Star() { return sprinter_; } */

  /* Prints the item to this String. */
  template<typename Type>
  inline AString& Print(Type item) {
    Autoject& ajt = asck_.AJT();
    IUW* origin = ajt.origin;
    D_ASSERT(origin);
    STR* element  = TPtr<STR>(origin);
    ISZ total = element->total,
        count = element->count;
    D_COUT("\n\n---\nPrinting \"" << item << "\" to 0x" << Hexf(origin) << 
           " total: " << total << " count : " << count);
    D_ASSERT(total >= 0);
    D_ASSERT(count > 0);
    T* begin = TStringBegin<STR_P>(element),
     * start = begin + count - 1,
     * end = begin + total - 1;
    //D_AVOW(TSCodeCount<T, ISZ>(begin) + 1, count);
    D_ASSERT(count <= total);
    D_COUT("\n         \"" << begin << '\"');
    T* stop = _::TSPrint<T, CHT>(start, end, item);
    D_COUT("\n| delta_begin_stop:" << TDelta<>(begin, stop) << 
           " delta_begin_end:" << TDelta<>(begin, end));
    //D_COUT(Charsf(begin, end));  //< Why does this bug the unit test out?

    if (IsError(stop)) {
      D_COUT("\n\nFailed to print: attempting to grow...");
      STR* str_new = NILP;
      RAMFactory ram = ajt.ram;
      D_ASSERT(ram);
      do {
        D_COUT("\nPrint failed, attempting to auto-grow from " <<
          ((ram != RAMFactoryHeap(ram)) ? "stack-to-heap." : "heap-to-heap."));
        Autoject growth = { NILP, ram };
        ISY total_new = AutojectGrowTotal(total);
        D_COUT(" total_new:" << total_new);
        str_new = Clone(growth, element, total_new, 0, count);
        if (IsError(str_new)) {
          *start = 0;
          D_COUT("\nWarning: Could not grow str!");
          return *this;
        }
        D_COUT("\nCloned with total:" << str_new->total << 
               " count:" << str_new->count);
        begin = TStringBegin<STR_P>(str_new);
        T* start = begin + count - 1,
         * end   = begin + str_new->total;
        stop = _::TSPrint<T, CHT>(start, end, item);
      } while (IsError(stop));
      count = ISZ(stop - begin) + 1;
      str_new->count = count;
      ajt.ram = RAMFactoryHeap(ram);
      ajt.origin = TPtr<IUW>(str_new);
    } else {
      count = ISZ(stop - begin) + 1;
      element->count = count;
    }
    D_COUT("\nPrinted:\"" << begin << "\":" << count << '/' << total);
    //D_COUT_STRING(TPtr<const STR>(ajt.origin));
    return *this;
  }

  /* Prints a item to the str.
  @return A reference to this. */
  inline AString& Print(CHA item) { return Print<CHA>(item); }
  inline AString& Print(const CHA* item) { return Print<const CHA*>(item); }
#if USING_STB == YES_0
  inline AString& Print(CHB item) { return Print<CHB>(item); }
  inline AString& Print(const CHB* item) { return Print<const CHB*>(item); }
#endif
#if USING_STC == YES_0
  inline AString& Print(CHC item) { return Print<CHC>(item); }
  inline AString& Print(const CHC* item) { return Print<const CHC*>(item); }
#endif
  inline AString& Print(ISC item) { return Print<ISC>(item); }
  inline AString& Print(IUC item) { return Print<IUC>(item); }
  inline AString& Print(ISD item) { return Print<ISD>(item); }
  inline AString& Print(IUD item) { return Print<IUD>(item); }
#if USING_FPC == YES_0
  inline AString& Print(FPC item) { return Print<FPC>(item); }
#endif
#if USING_FPD == YES_0
  inline AString& Print(FPD item) { return Print<FPD>(item); }
#endif

  inline AString& Print(Repeatf item) { return Print<FPD>(item); }

  /* Returns the begin char. */
  inline T* Begin() { return TStringBegin<STR_P>(This()); }

  /* Returns the nil-term/start char.
  inline T* Start() { return TStringStart<STR_P>(This()); } */

  /* Returns the stop char.
  inline T* Stop() { return TStringStop<STR_P>(This()); } */

  /* Returns the stop of the obj. */
  inline T* End() { return TStringEnd<STR_P>(This()); }

  /* Calculates the space left in the boofer based on the sprinter_ pointers. */
  inline ISZ SpaceLeft() {
    STR element = This();
    return element.total - element.count;
  }

  /* Searches for the given query, returning the end T of the query in this
  str. */
  inline T* Find(const T* query) { return TSFind<T, CHT>(Begin(), query); }

  /* Checks if this String to the other str are equivalent.
  @return Nil if they Strings are not equivalent and a pointer to the next CHA
  after the end of the equivalent part of this str upon success. */
  inline T Equals(const T* other) { return TStringFind(Begin(), other); }

  /* Checks to see if the sprinter_ is using static memory. */
  inline BOL IsStatic() { 
    RAMFactory ram      = AJT().ram,
               ram_heap = (RAMFactory)ram(NILP, 0);
    return ram != ram_heap;
  }

  /* Gets the obj of the Console obj. */
  inline AStack<SCK_P, TotalInit_, BOF>& Stack() { return asck_; }

  /* Gets the obj of the Console obj. */
  inline Autoject& AJT() { return asck_.AJT(); }

  /* Returns the Autoject origin as the specified Type pointer. */
  template<typename Type = STR>
  inline Type* As() {
    return TPtr<Type>(AJT().origin);
  }

  /* Gets the Autoject origin as a TString<T, ISZ>. */
  inline STR* This() { return As<>(); }

  /* Prints this object to the given printer. */
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return TStringPrint<Printer, T, ISZ>(o, This());
  }

  inline void CPrint() { PrintTo<COut>(StdOut()); }

  /* Creates a clone on the heap of the src Autoject. */
  inline static STR* Clone(Autoject& dest, const STR* src, ISZ total_new = -1, 
                           ISZ count_start = 0, ISZ count_stop = -1) {
    AStack<SCK_P>::Clone(dest, TPtr<TStack<SCK_P>>(src),
                         total_new, count_start, count_stop);
    IUW* origin = dest.origin;
    if (!IsError(origin)) {
      if (dest.origin != origin) {
        Delete(dest);
        dest.origin = origin;
        dest.ram = RAMFactory(dest.ram(NILP, 0));
      }
      return TPtr<STR>(origin);
    }
    return NILP;
  }
};

}  //< namespace _

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj,
    ::_::AString<STR_P, Size, BOF>& item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, const CHA* item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

#if USING_STB == YES_0
template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, const CHB* item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}
#endif

#if USING_STC == YES_0
template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, const CHC* item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, CHC item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}
#endif

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, CHA item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, IUA item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ISB item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, IUB item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ISC item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, IUC item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ISD item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, IUD item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}

#if USING_FPC == YES_0
template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, FPC item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}
#endif
#if USING_FPD == YES_0
template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, FPD item) {
  ::_::AString<STR_P, Size, BOF>& result = obj.Print(item);
  return obj;
}
#endif

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Hexf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Binaryf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Centerf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Rightf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Linef item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
  ::_::AString<STR_P, Size, BOF>& obj, ::_::Repeatf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Headingf item) {
  return obj.Print(item);
}

template<STR_A, ISZ Size, typename BOF>
inline ::_::AString<STR_P, Size, BOF>& operator<<(
    ::_::AString<STR_P, Size, BOF>& obj, ::_::Charsf item) {
  return obj.Print(item);
}

#endif
#endif
