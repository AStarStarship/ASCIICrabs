// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsATypeHPP
#define _CrabsATypeHPP
#include "AType.h"
#if _Seam >= _CrabsCore
namespace _ {

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
#define ARY_A typename T = CHA, typename ISZ = ISN
#define ARY_P T, ISZ
#define ARY TArray<ARY_P>
#define AARY AArray<ARY_P>

template<typename T = CHA, typename ISZ = ISW>
struct TArray {
  ISZ total;  //< Number of elements in the array.
};
/* @defgroup ASCII Stack */

/* ASCII Stack
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Stack.md

An Array Stack obj of homogeneous-sized plain-old-data (POD) types.
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Stack.md

# Stack Memory Layout

@code
+---------------+
|    Buffer     |
|---------------|
| C-Style Array |
|---------------|  ^
|    Header     |  |
+---------------+ 0xN
@endcode
*/
#define SCK_A typename T=ISW, typename ISZ=ISW, typename ISY=ISZ
#define SCK_P T, ISZ, ISY
#define SCK TStack<SCK_P>
//@todo Change SCP_A to typename ISZ=ISW, typename ISY=ISZ
template<SCK_A>
struct TStack {
  ISZ total,  //< Size of the Array in elements.
      count;  //< Element count.
};


/* @defgroup AsciiMatrix
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/vector_types/Matrix.md */

/* An n-dimensional matrix of homogeneous-sized plain-old-data (POD) types.
## Memory Layout

@code
    +-----------------+
    |  C-Style Array  |
    |-----------------|
    | Dimension Stack |
 ^  |-----------------|
 |  |     Header      |
0xN +-----------------+
@endcode
*/
#define MTX_A typename T = ISW, typename ISZ = ISC, typename ISY = ISB, \
  typename IUY = IUB
#define MTX_P T, ISZ, ISY, IUY
#define MTX TMatrix<MTX_P>
template<MTX_A>
struct TMatrix {
  ISZ bytes,      //< Size in bytes.
      elems;      //< Total number of matrix elements.
  TStack<SCK_P> dimensions; //< Stack of dimensions.
};

/* A collection of type-value tuples.
@see ASCII Data Type Specification.
@link /Spec/Data/MapTypes/List.md
@code
      List Memory Layout
+============================+
|_______   Buffer            |<--- top
|_______ ^ Value N           |
|        | Value 0           |
|----------------------------|
|_______   Buffer            |
|_______ ^ Type of Value N   |
|        | Type of Value 0   |
|----------------------------|
|_______   Buffer            |
|_______ ^ Offset to Value N |
|        | Offset to Value 0 |
+============================+  ^  Up in addresses.
|        TList Struct        |  |
+============================+ 0xN Base address
@endcode
*/
#define LST_A typename ISZ = ISC, typename ISY = ISB, typename DT = DTB
#define LST_P ISZ, ISY, DT
#define LST TList<LST_P>

template<LST_A>
struct TList {
  ISZ bytes,        //< Size of the List in bytes.
      top;          //< Offset to the top of the data.
  TStack<ISZ, ISZ, ISY> map;  //< Stack of offset mappings to the list items.
};


/* @defgroup Loom
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Loom.md

# Memory Layout

+============================+
|_______ Buffer              | <-- start
|_______ String N            |
|_______ String 0            |
+----------------------------+
|_______ Buffer              |
|_______ String offset N     |
|        String offset 0     |
+============================+  ^ Up in
|       Loom Header          |  |
+============================+  + 0xN

@todo Look into adding variable for average UTF-8 and UTF-16 character length.
@todo Change keys_offset delta from table to keys_map.
*/
#define LOM_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, typename ISY = ISQ
#define LOM_P CHS, CHT, ISZ, ISY
#define LOM TLoom<LOM_P>

/* An array of Strings. */
template<LOM_A>
struct TLoom {
  ISZ bytes,                  //< Size of the Loom in bytes.
      start;                  //< Offset to the start of the string stack.
  TStack<ISZ, ISZ, ISY> map;  //< A Stack of offset mappings to strings.
};

// The Book/Dic value lists use the plain List macro (LST_P = ISZ, ISY, DT) —
// do NOT manually re-enter the List template params here; a 5-arg Book-style
// list was accidentally pasted and broke TList<LST_P> (wrong arg count).
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
|_______   Buffer                    |
|_______ ^ List Value N              |
|_______ | List Value 1              |
|        | Keys Loom (Value 0)       |
|------------------------------------|
|_______   Buffer                    |
|_______ ^ List Value N Type         |
|_______ | List Value 1 Type         |
|        | Loom Type (Type 0)        |
|------------------------------------|
|_______   Buffer                    |
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


/* @defgroup Dic
@brief A contiguous memory Associative List created from a List and a Table.
@code
+-----------------+
|      List       |
|-----------------|
|      Table      |
|-----------------|  ^ Up in addresses
|  TDic Struct    |  |
+-----------------+ 0xN
@endcode */
#define DIC_A typename CHS = CHR, typename CHT = CHR, typename ISZ = ISR,\
  typename ISY = ISQ, typename DT = DTB, typename HSH = IUN
#define DIC_P CHS, CHT, ISZ, ISY, DT, HSH
#define DIC TDic<DIC_P>
template<DIC_A>
struct TDic {
  TList<LST_P> values;  //< A list of values with a Table of keys in index 0.
};

/* B-Input ring buffer Slot.
A B-Input Slot is functionally identical to a input port in TCP. */
template<typename ISZ = ISC>
struct TBIn {
  ISZ bytes,          //< The size of the socket in bytes.
      origin;         //< The starting index of the ring socket data.
  volatile ISZ stop;  //< The stopping index of the ring socket data.
  ISZ read;           //< The read variable to memory align the socket.
};

/* Byte-ring output socket. */
template<typename ISZ = ISC>
struct TBOut {
  ISZ bytes;            //< Size of the B-Output in bytes.
  volatile ISZ origin;  //< Starting index of the ring-socket data.
  ISZ stop,             //< Stopping index of the ring-socket data.
      read;             //< Address that the BOut device is reading from.
};

/* A Crabs Operation. The `in` field is overloaded: either a
const DTB* param header (real pointer to type array) or a
function-index marker (OpFirst('A') reinterprets char as pointer).
Scanner distinguishes by num_ops > CrabsParamsMax (420). */
struct Op {
  const CHR * name;           //< Op name.
  const DTB * in,             //< Input _BSQ params or OpFirst.
            * out;            //< Output _BSQ params or OpLast.
  const CHR * description;    //< Op description.
  CHR         pop,            //< Index of the Pop Operation.
              close,          //< Index of the Close Operation.
              default_op;     //< Index of the Default Operation.
  const TBOut<ISC>* evaluation;  //< Evaluated expression Slot.
};

// @defgroup Slot


/* A Mirror in a Chinese Room.
Okay, then what does a Mirror do? */

// template<typename ISZ = ISC>
// struct TMirror {
//   ISZ size,    //< The size of the socket.
//       origin,  //< The starting index of the ring socket data.
//       stop,    //< The stopping index of the ring socket data.
//       read;    //< The read variable.
// };

/* A door that connects two Chinese rooms.
A door can lead to one or more Rooms. A Door that can
be locked, in which case all of the Rooms behind this door are locked
out. When a door is locked, no messages are processed or sent to or
from the door and its subsidiary agents.

Rooms can be connected by one or more Portal. An example of a
system with one Door and multiple Portals is a WiFi plus USB/Serial
connection. This is a commonly used configuration for debugging the system.

Multiple doors also might share the same Portal. An example of this is a
connection to multiple systems over a WiFi connection.

@code

+=============+
|    slots    |
|=============|
| Door struct |
+=============+

@endcode
*/
#define DOR_P typename ISZ = ISN
#define DOR_A ISZ
template<DOR_P>
struct TDoor {
  ISN  bytes_,          //< Door size in bytes.
       scan_total_;     //< Max bytes to pull through the slot at once.
  IUW* begin_;          //< Pointer to dynamic socket.
  TStack<Slot, ISZ, ISZ>* slots_;  //< Slots in the door.
};

/* A group of Doors that share the same Extended Types.
Only one single wall is required for a Chinese Room, but when more memory is
needed a new Wall may be created and destroyed dynamically.

@code
+--------------+
|  Terminals   |
|      v       |
|vvvvvvvvvvvvvv|
|    Buffer    |
|^^^^^^^^^^^^^^|
|      ^       |
|  TStack of   |
|    Doors     |
|   Offsets    |
|--------------|
|    Header    |
+--------------+
@endcode */
#define WAL_A typename T = ISW, typename ISZ = ISN, typename ISY = ISN
#define WAL_P T, ISZ, ISY
#define WAL TWall<WAL_P>
template<WAL_A>
struct TWall {
  IUD ctx_; //< Plain Context Type remap word.
  TStack<TDoor<ISZ>, ISZ, ISY> doors_;  //< The doors in the room.
	BOL is_dynamic_;	//< Whether this wall was dynamically allocated or not.
};
/* Utility class for givings names to params.
```C++
void Foo(TP<ISZ, "Bar">() bar) {}
Foo(TP<ISZ, "Bar">(69));
```

or maybe it could look like:
```C++
#define TP(tag) struct tag {
};
```
*/
template<typename T, const CHA* name>
struct TP {
  T value;

  TP(T v) : value(v) {}
};
//! @brief Returns the name associated with this type pair.
//! @param tp The type-value pair.
//! @return The name string.
template<typename T, const CHA* Name>
inline const CHA* TPName(const TP<T, Name>& tp) { return Name; }

/*
inline IUA ValueBase2(IUC value) {
  // Example: 0b0000_0000_0000_0000_0111_0111_1001_1101
  return 0;
} */

// Gets the log_2 of the sizeof(T) where A=0 (8-bit), B=1 (16-bit), ..., L=11 (2048-bit).
template<typename T>
constexpr DTB CASizeCode() {
  switch (sizeof(T)) {
    case    1: return 0;  // A
    case    2: return 1;  // B
    case    4: return 2;  // C
    case    8: return 3;  // D
    case   16: return 4;  // E
    case   32: return 5;  // F
    case   64: return 6;  // G
    case  128: return 7;  // H
    case  256: return 8;  // I
    case  512: return 9;  // J
    case 1024: return 10; // K
    case 2048: return 11; // L
  }
  return 0;
}

template<typename T, typename DT=DTB>
constexpr DT CASizeBits() {
  return DT(1) << ATypeSWBit0;
}

// Gets the ASCII Size Width letter A-L
template<typename T>
constexpr CHA CSizeCodef() {
  return 'A' + CHA(CASizeCode<T>());
}

// Gets the ASCII Size Width letter A-L
inline CHA ASizeCodef(ISA sw) {
  return 'A' + CHA(sw);
}
inline CHA ASizeCodef(ISB sw) {
  return 'A' + CHA(sw);
}
inline CHA ASizeCodef(ISC sw) {
  return 'A' + CHA(sw);
}
inline CHA ASizeCodef(ISD sw) {
  return 'A' + CHA(sw);
}

template<typename T>
T TCTXHandle(ACTXHandler actxh, void* begin, void* end, DTW type, IUW value, 
             IUW vmsb) {
  if (actxh == _NilP) return T(0);
  return T(actxh(begin, end, type, value, vmsb));
}

// Strings for the POD types 0-31.
template<typename CH = CHR>
const CH* TATypePODs() {
  static const CH Strings[64][4] = {
      {'N', 'I', 'L', NIL},  //< 00
      {'I', 'U', 'A', NIL},  //< 01
      {'I', 'S', 'A', NIL},  //< 02
      {'C', 'H', 'A', NIL},  //< 03
      {'F', 'P', 'B', NIL},  //< 04
      {'I', 'U', 'B', NIL},  //< 05
      {'I', 'S', 'B', NIL},  //< 06
      {'C', 'H', 'B', NIL},  //< 07
      {'F', 'P', 'C', NIL},  //< 08
      {'I', 'U', 'C', NIL},  //< 09
      {'I', 'S', 'C', NIL},  //< 10
      {'C', 'H', 'C', NIL},  //< 11
      {'F', 'P', 'D', NIL},  //< 12
      {'I', 'U', 'D', NIL},  //< 13
      {'I', 'S', 'D', NIL},  //< 14
      {'T', 'M', 'D', NIL},  //< 15
      {'F', 'P', 'E', NIL},  //< 16
      {'I', 'U', 'E', NIL},  //< 17
      {'I', 'S', 'E', NIL},  //< 18
      {'T', 'M', 'E', NIL},  //< 19
      {'E', 'P', 'a', NIL},  //< 20
      {'E', 'P', 'b', NIL},  //< 21
      {'E', 'P', 'c', NIL},  //< 22
      {'E', 'P', 'd', NIL},  //< 23
      {'E', 'P', 'e', NIL},  //< 24
      {'E', 'P', 'f', NIL},  //< 25
      {'E', 'P', 'g', NIL},  //< 26
      {'E', 'P', 'h', NIL},  //< 27
      {'E', 'P', 'i', NIL},  //< 28
      {'E', 'P', 'j', NIL},  //< 29
      {'E', 'P', 'k', NIL},  //< 30
      {'E', 'P', 'l', NIL},  //< 31
      {'E', 'N', 'V', NIL},  //< 32
  };
  return &Strings[0][0];
}

template<typename CH = CHR, typename IS = ISW>
inline const CH* TATypePODs(IS type, ACTXHandler actxh) {
  type = (type < 0 || type > ATypePODTotal) ? ATypePODTotal : type;
  if (type >= _PCa)
    return (const CH*)ACTXHandle(actxh, _NilP, _NilP, type, 0, 0);
  return &TATypePODs<CH>()[type << 2];
}

template<typename CH = CHR, typename IS = ISW>
inline const CH* TATypePODs(IS type) {
  type = (type < 0 || type > ATypePODTotal) ? ATypePODTotal : type;
  if (type >= _PCa) return (const CH*)ACTXHandle(_NilP, _NilP, type, 0, 0);
  return &TATypePODs<CH>()[type << 2];
}

template<typename CH = CHR>
const CH* TATypeVectors() {
  static const CH Strings[17][4] = {
      {'V', 'H', 'A', NIL},  //< 00
      {'A', 'R', 'A', NIL},  //< 01
      {'S', 'C', 'A', NIL},  //< 02
      {'M', 'A', 'A', NIL},  //< 03
      {'V', 'H', 'B', NIL},  //< 04
      {'A', 'R', 'B', NIL},  //< 05
      {'S', 'C', 'B', NIL},  //< 06
      {'M', 'A', 'B', NIL},  //< 07
      {'V', 'H', 'C', NIL},  //< 08
      {'A', 'R', 'C', NIL},  //< 09
      {'S', 'C', 'C', NIL},  //< 10
      {'M', 'A', 'C', NIL},  //< 11
      {'V', 'H', 'D', NIL},  //< 12
      {'A', 'R', 'D', NIL},  //< 13
      {'S', 'C', 'D', NIL},  //< 14
      {'M', 'A', 'D', NIL},  //< 15
      {'I', 'N', 'V', NIL},  //< 16
  };
  return &Strings[0][0];
}

template<typename CH = CHR, typename IS = ISW>
const CH* TATypeVectors(IS value) {
  value = (value < 0 || value > 16) ? 16 : value;
  return &TATypeVectors<CH>()[value << 2];
}

template<typename CH = CHR, typename IS = ISW>
const CH* TATypeVectors(IS size_width_bits, IS vector_type_bits) {
  IS value = (size_width_bits << 2) | vector_type_bits;
  value = (value < 0 || value > 16) ? 16 : value;
  return &TATypeVectors<CH>()[value << 2];
}

template<typename CH = CHR>
const CH* TATypeVectorClasses() {
  static const CH Strings[5][4] = {
      {'V', 'H', 'T', NIL},  //< 00
      {'A', 'R', 'Y', NIL},  //< 01
      {'S', 'C', 'K', NIL},  //< 02
      {'M', 'T', 'X', NIL},  //< 03
      {'I', 'N', 'V', NIL},  //< 04
  };
  return &Strings[0][0];
}

template<typename CH = CHR, typename IS = ISW>
const CH* TATypeVectorClasses(IS value) {
  value = (value < 0 || value > 16) ? 16 : value;
  return &TATypeVectorClasses<CH>()[value << 2];
}

template<typename CH = CHR>
const CH* TATypeMODs() {
  static const CH Strings[9][8] = {
      {NIL},  //< 0
      {'P', 'T', 'R', NIL},  //< 1
      {'P', 'T', 'I', NIL},  //< 2
      {'C', 'T', 'X', NIL},  //< 3
      {'C', 'N', 'S', NIL},  //< 4
      {'C', 'N', 'S','_', 'P', 'T', 'R', NIL},  //< 5
      {'C', 'N', 'S','_', 'P', 'I', 'X', NIL},  //< 6
      {'C', 'N', 'S','_', 'C', 'T', 'X', NIL},  //< 7
      {'I', 'N', 'V', NIL},  //< 4
  };
  return &Strings[0][0];
}

template<typename CH = CHR, typename IS = ISW>
const CH* TATypeMODs(IS value) {
  value = (value < 0 || value > 8) ? 8 : value;
  return &TATypeMODs<CH>()[value << 3];
}

template<typename CH = CHR>
const CH* TATypeMODsCNS() { return TATypeMODs<CH>()[2 * 4]; }

template<typename CH = CHR>
const CH* TATypeMaps() {
  static const CH Strings[5][4] = {
      {'M', 'A', 'C', NIL},  //< 00
      {'M', 'A', 'D', NIL},  //< 01
      {'I', 'N', 'V', NIL},  //< 02
      {'I', 'N', 'V', NIL},  //< 03
      {'I', 'N', 'V', NIL},  //< 04
  };
  return &Strings[0][0];
}

template<typename CH = CHR, typename IS = ISW>
const CH* TATypeMaps(IS value) {
  value = (value < 0 || value > 2) ? 2 : value;
  return &TATypeMaps<CH>()[value << 2];
}

/* Human-readable Crabs error strings. */
template<typename CH = CHR>
const CH* TAErrors() {
  static const CH error_strings[AErrorTotal + 1][32] = {
    /*00*/{'S','u','c','k',' ','i','t',0,},
    /*01*/{'I','n','v','a','l','i','d',0,},
    /*02*/{'I','n','v','a','l','i','d',' ','b','o','o','f','e','r',0},
    /*03*/{'I','n','v','a','l','i','d',' ','b','o','u','n','d','s',0},
    /*04*/{'I','n','v','a','l','i','d',' ','d','o','o','r',0},
    /*05*/{'I','n','v','a','l','i','d',' ','h','e','a','d','e','r',0},
    /*06*/{'I','n','v','a','l','i','d',' ','h','a','s','h',0},
    /*07*/{'I','n','v','a','l','i','d',' ','i','n','d','e','x',0},
    //--------------------------------------------------------------------------
    /*08*/{'I','n','v','a','l','i','d',' ','i','n','p','u','t',0},
    /*09*/{'I','n','v','a','l','i','d',' ','e','n','q','u','e','r','y',0},
    /*10*/{'I','n','v','a','l','i','d',' ','o','p',0},
    /*11*/{'I','n','v','a','l','i','d',' ','o','p','e','r','a','n','d',0},
    /*12*/{'I','n','v','a','l','i','d',' ','r','o','o','m',0},
    /*13*/{'I','n','v','a','l','i','d',' ','t','y','p','e',0},
    /*14*/{'I','n','v','a','l','i','d',' ','U','T','F','-','8',0},
    /*15*/{'I','n','v','a','l','i','d',' ','U','T','F','-','1','6',0},
    //--------------------------------------------------------------------------
    /*16*/{'A','r','r','a','y',' ','u','n','d','e','r','f','l','o','w',0},
    /*17*/{'B','o','o','f','e','r',' ','u','n','d','e','r','f','l','o','w',0},
    /*18*/{'K', 'e', 'y', 's', ' ', 'b', 'o', 'o', 'f', 'e', 'r', ' ', 'u','n','d',
           'e', 'r', 'f', 'l', 'o', 'w', 0, },
    /*19*/{'P','a','r','a','m',' ','u','n','d','e','r','f','l','o','w',0},
    /*20*/{'S','t','a','c','k',' ','u','n','d','e','r','f','l','o','w',0},
    /*21*/{'T','e','x','t',' ','u','n','d','e','r','f','l','o','w',0},
    /*22*/{'V','a','r','i','n','t',' ','u','n','d','e','r','f','l','o','w',0},
    /*23*/{'Y','o','u','r',' ','m','o','m',' ','u','n','d','e','r','f','l','o','w',
          0},
    //--------------------------------------------------------------------------
    /*24*/{'A','r','r','a','y',' ','o','v','e','r','f','l','o','w',0},
    /*25*/{'B','o','o','f','e','r',' ','o','v','e','r','f','l','o','w',0},
    /*26*/{'K', 'e', 'y', 's', ' ', 'b', 'o', 'o', 'f', 'e', 'r', ' ', 'o', 'v',
           'e', 'r', 'f', 'l', 'o', 'w', 0 },
    /*27*/{'P','a','r','a','m',' ','o','v','e','r','f','l','o','w',0},
    /*28*/{'S','t','a','c','k',' ','o','v','e','r','f','l','o','w',0},
    /*29*/{'T','e','x','t',' ','o','v','e','r','f','l','o','w',0},
    /*30*/{'V','a','r','i','n','t',' ','o','v','e','r','f','l','o','w',0},
    /*31*/{'Y','o','u','r',' ','m','o','m',' ','o','v','e','r','f','l','o','w',
          0},
    //--------------------------------------------------------------------------
    /*32*/{'P','a','r','a','m',' ','n','i','l',0},
    /*33*/{'P','a','r','a','m',' ','i','s',' ','e','r','r','o','r',' ','c','o',
           'd','e',0},
    /*34*/{'P','a','r','a','m',' ','i','n','v','a','l','i','d',0},
    /*35*/{'P','a','r','a','m',' ','e','r','o','n','e','o','u','s',0},
    /*36*/{'R', 'e', 'a', 'd', ' ', 'o', 'n', 'l', 'y', 0},
    /*37*/{'W', 'r', 'i', 't', 'e', ' ', 'o', 'n', 'l', 'y', 0},
    /*38*/{'U','n','a','u','t','h','e','r','i','z', 'e','d',0},
    /*39*/{'U','n','a','u','t','h','e','r','i','z', 'e','d',0},
    //--------------------------------------------------------------------------
    /*40*/{'U','n','a','v','a','i','l','a','b','l','e',0},
    /*41*/{'U','n','a','v','a','i','l','a','b','l','e',' ','k','e','y',0},
    /*42*/{'I','m','p','l','e','m','e','n','t','a','t','i','o','n',0}
  };
  return &error_strings[0][0];
}

/* Stores an ASCII type and pointer to it's value. */
template<typename DT = DTW>
struct TATypeValuePtr {
  DT    type;   //< The ASCII data type word.
  void* value;  //< POint to the value of the type.
};

/* Stores an ASCII data type and offset to value.
This class is preferred over TATypeValuePtr for x86_64 because two 32-bit or 
smaller values may be returned from functions without using the stack. */
template<typename IS = ISW, typename DT = DTW>
struct TATypeValue {
  DT type;      //< ASCII data type.
  IS value;     //< Offset to the value of the type.
};

/* Stores a pointer to the ASCII data type and it's value. */
template<typename IS = ISW, typename DT = DTW, typename CH = CHR>
struct TATypeKV {
  const CH* key;          //< Pointer to the key.
  TATypeValue<IS, DT> tv; //< AType value offset pair.
};

/* Stores a pointer to the ASCII data type and it's value. */
template<typename IS = ISW, typename DT = DTW, typename CH = CHR>
struct TATypeKeyValueIndex {
  IS index;                        //< Index of the key-value pair.
  TATypeKV<IS, DT, CH> type_value; //< Pointer to the value of the type.
};

/* An 8-bit, 16-bit, or 32-bit ASCII Data Type. */
template<typename DT>
class TType {
 public:

  DT type;  //< The ASCII Data Type.

  TType(DT type) : type(type) {} //< Stores the type.
};

constexpr DTB CATypePOD(IUA value) { return _IUA; }
constexpr DTB CATypePOD(ISA value) { return _ISA; }
constexpr DTB CATypePOD(CHA value) { return _CHA; }

//constexpr DTB CAType(FPB value) { return _FPB; }
constexpr DTB CATypePOD(IUB value) { return _IUB; }
constexpr DTB CATypePOD(ISB value) { return _ISB; }
constexpr DTB CATypePOD(CHB value) { return _CHB; }

constexpr DTB CATypePOD(FPC value) { return _FPC; }
constexpr DTB CATypePOD(IUC value) { return _IUC; }
constexpr DTB CATypePOD(ISC value) { return _ISC; }
constexpr DTB CATypePOD(CHC value) { return _CHC; }

constexpr DTB CATypePOD(FPD value) { return _FPD; }
constexpr DTB CATypePOD(IUD value) { return _IUD; }
constexpr DTB CATypePOD(ISD value) { return _ISD; }
//constexpr DTB CAType(TMD value) { return _TMD; }

//constexpr DTB CAType(FPE value) { return _FPE; }
//constexpr DTB CAType(IUE value) { return _IUE; }
//constexpr DTB CAType(ISE value) { return _ISE; }
//constexpr DTB CAType(TME value) { return _TME; }

template<typename T>
constexpr DTB CATypePOD() {
  T foo = 0;
  return CAType(foo);
}

/* Extracts the Vector type. */
template<typename DT = DTB>
inline DT TTypeVector(DTW type) {
  return DT((type >> 5) & 3);
}
template<typename DT = DTB>
constexpr DT CTypeVector(DTW type) {
  return DT((type >> 5) & 3);
}

/* Extracts the Map type 0-3 of the type. */
template<typename DT = DTB>
inline DT TTypeMap(DTW type) {
  return DT((type >> 8) & 3);
}
template<typename DT = DTB>
constexpr DT CTypeMap(DTW type) {
  return DT((type >> 8) & 3);
}

// Returns the memory alignment mask for this type.
ISA ATypeAlignMask(DTB type) {
  DezNutz:
  if (type <= _CHA) return 0;
  if (type <= _CHB) return 1;
  if (type <= _CHC) return ACPUAlignC;
  if (type <= _SSE) return ACPUAlignD;
  if (type <  _ECD) return ACPUAlignD;
  if (type <  _ECC) return ACPUAlignC;
  if (type <  _ECB) return 1;
  if (type <  _ECA) return 0;
  DTB mod = type >> ATypeMDBit0;
  type ^= mod << ATypeMDBit0;
  DTB mt = type >> ATypeMTBit0;
  type ^= mt << ATypeMTBit0;
  DTB sw = type >> ATypeSWBit0;
  type ^= sw << ATypeSWBit0;
  DTB vt = type >> ATypeVTBit0;
  type ^= vt << ATypeVTBit0;
  if (vt == 0) { // Vetor of 2 to 4 Homo-tuples
    type &= (ATypePODTotal - 1);
    goto DezNutz;
  }
  switch (sw) {
    case 0: return 0;
    case 1: return 1;
    case 2: return ACPUAlignC;
    case 3: return ACPUAlignD;
  }
  return ACPUWordMask;
}
ISA ATypeAlignMask(DTC type) {  return ATypeAlignMask(DTB(type)); }
ISA ATypeAlignMask(DTD type) {  return ATypeAlignMask(DTB(type)); }

/* Aligns the pointer up to the word boundry required by the type. */
template<typename T = void>
T* TATypePtrUp(void* pointer, DTB type) {
  ISW align_mask = ATypeAlignMask(type);
  return reinterpret_cast<T*>(ISW(pointer) + (-ISW(pointer) & align_mask));
}
template<typename T = void>
inline const T* TATypePtrUp(const void* pointer, DTB type) {
  return const_cast<const T*>(TATypePtrUp<T>(const_cast<T*>(pointer), type));
}
template<typename T = void>
T* TATypePtrUp(void* pointer, DTC type) {
  return TATypePtrUp<T>(pointer, DTB(type));
}
template<typename T = void>
const T* TATypePtrUp(const void* pointer, DTC type) {
  return TATypePtrUp<T>(pointer, DTB(type));
}
template<typename T = void>
T* TATypePtrUp(void* pointer, DTD type) {
  return TATypePtrUp<T>(pointer, DTB(type));
}
template<typename T = void>
const T* TATypePtrUp(const void* pointer, DTD type) {
  return TATypePtrUp<T>(pointer, DTB(type));
}

/* Aligns the pointer down to the word boundry required by the type. */
template<typename T = void>
T* TATypePtrDown(void* pointer, DTB type) {
  ISW align_mask = ATypeAlignMask(type);
  return reinterpret_cast<T*>(ISW(pointer) - (ISW(pointer) & align_mask));
}
template<typename T = void>
inline const T* TATypePtrDown(const void* pointer, DTB type) {
  return const_cast<const T*>(TATypePtrDown<T>(const_cast<T*>(pointer), type));
}
template<typename T = void>
T* TATypePtrDown(void* pointer, DTC type) {
  return TATypePtrDown<T>(pointer, DTB(type));
}
template<typename T = void>
const T* TATypePtrDown(const void* pointer, DTC type) {
  return TATypePtrDown<T>(pointer, DTB(type));
}
template<typename T = void>
T* TATypePtrDown(void* pointer, DTD type) {
  return TATypePtrDown<T>(pointer, DTB(type));
}
template<typename T = void>
const T* TATypePtrDown(const void* pointer, DTD type) {
  return TATypePtrDown<T>(pointer, DTB(type));
}

/* Creates an ASCII Vector Data Type. */
template<typename DT = DTB>
inline DT TATypeVector(DTW pod, DTW vt, DTW sw = 0) {
  return DT(pod | (vt << 5) | (sw << ATypeSWBit0));
}
template<typename DT = DTB>
constexpr DT CATypeVector(DTW pod, DTW vt,
                         DTW sw = 0) {
  return DT(pod | (vt << 5) | (sw << ATypeSWBit0));
}

/* Creates an ASCII Map Type. */
template<typename DT = DTB>
constexpr DT CATypeMap(DTW pod, DTW mt, DTW sw = 0) {
  return DT(pod | (mt << ATypeMTBit0) |
            (sw << ATypeSWBit0));
}
template<typename DT = DTB>
inline DT TATypeMap(DTW pod, DTW mt, DTW sw = 0) {
  return DT(pod | (mt << ATypeMTBit0) | 
            (sw << ATypeSWBit0));
}

template<typename CH = CHR, typename IS = ISN>
const CH* TAErrors(IS index) {
  if (index < 0) index *= -1;
  if (index >= AErrorTotal) index = AErrorTotal;
  return &TAErrors<CH>()[index << 5];
}

/* The ASCII Data Type mask for the SW (Size Width) bits. */
template<typename IS = CHR, typename DT = DTB>
constexpr DT CATypeSW() {
  return (sizeof(IS) ==  1) ? 0 
       : (sizeof(IS) ==  2) ? 1
       : (sizeof(IS) ==  4) ? 2
       : (sizeof(IS) ==  8) ? 3 : 0;
}

/* The ASCII Data Type mask for the SW (Size Width) bits. */
template<typename T, typename DT = DTB>
constexpr DT CATypeSW(DT pod) {
  return pod | (CATypeSW<T, DT>() << ATypeSWBit0);
}

template<typename T, typename DT = DTB>
inline DT TATypeSW(DT pod) {
  return pod | (CATypeSW<T, DT>() << ATypeSWBit0);
}

/* Returns the ASCII Type for the given floating-point type FP.
FPB:  4   0b00100   (0b001 << 2) | 0b00
FPC:  8   0b01000   (0b010 << 2) | 0b00
FPD: 12   0b01100   (0b011 << 2) | 0b00
FPE: 16   0b10000   (0b100 << 2) | 0b00
*/
template<typename FP, typename DT = DTB>
constexpr DT CATypeFP() {
  return (CATypeSW<FP, DT>() << 2);
}

/* Returns the ASCII Type for the given unsigned integer type IS.
IUA:  1   0b00001   (0b000 << 2) | 0b01
IUB:  5   0b00101   (0b001 << 2) | 0b01
IUC:  9   0b01001   (0b010 << 2) | 0b01
IUD: 13   0b01101   (0b011 << 2) | 0b01
IUE: 17   0b10001   (0b100 << 2) | 0b01
*/
template<typename IU, typename DT = DTB>
constexpr DT CATypeIU() {
  return (CATypeSW<IU, DT>() << 2) | _IUA;
}

/* Returns the ASCII Type for the given signed integer type IS.
ISA:  2   0b00010   (0b000 << 2) | 0b10
ISB:  6   0b00110   (0b001 << 2) | 0b10
ISC: 10   0b01010   (0b010 << 2) | 0b10
ISD: 14   0b01110   (0b011 << 2) | 0b10
ISE: 18   0b10010   (0b100 << 2) | 0b10
*/
template<typename IS, typename DT = DTB>
constexpr DT CATypeIS() {
  return (CATypeSW<IS, DT>() << 2) | _ISA;
}

// Returns the Modifier Bits.
inline DTB ATypeMD(DTB type) {
  return (type >> ATypeMDBit0) & 3;
}
inline DTC ATypeMD(DTC type) {
  return ATypeMD(DTB(type));
}
inline DTD ATypeMD(DTD type) {
  return ATypeMD(DTB(type));
}

// Returns the Extended Block Bits.
inline DTB ATypeEB(DTB type) {
  return (type >> ATypeEBBit0) & 3;
}
inline DTC ATypeEB(DTC type) {
  return ATypeMD(DTB(type));
}
inline DTD ATypeEB(DTD type) {
  return ATypeMD(DTB(type));
}

// Asserts if the type is an Extended Standard Type.
inline BOL ATypeIsES(DTW type) {
  //return ((ATypeMD(type) != 0) && ((type >> ATypeMDBit0) & 7) == EBES);
  return ((type >> ATypeEBBit0) & 7) == EBES;
}

/* Extracts the UTF type.
@return 0 if the type is not a string type or 1, 2, or 4 if it is. */
inline ISA ATypeTextFormat(DTW type) {
  DTW pod = type & ATypePODMask;
  // CHA: 3   0b0011
  // CHB: 7   0b0111
  // CHC: 11  0b1011
  //if (pod > _CHC || type < ATypePODTotal || (pod & 3) != 3)
  //  return -1;
  if (pod > _CHC)
    return -1;
  if (type < ATypePODTotal)
    return -1;
  if ((pod & 3) != 3)
    return -1;
  type >>= ATypePODBits;
  DTW vt = type & 3;
  type >>= 2;
  DTW sw = type & 3;
  type >>= 2;
  DTW mt = type & 15;
  mt >>= 4;
  DTW md = type & 3;
  if (md != 0) {
    DTW extended_block = (md << 1) & (mt >> 3);
    if (extended_block != EBES)
      return -1;
  }
  if (vt == _SCK || type >> 3)
    return ISA(pod >> 2);
  return -1;
}

template<typename IS>
inline BOL TSizeIsValid(IS size) {
  return (size & (sizeof(IS) - 1)) == 0;
}

/* A ROM  for one of the 32 types.
C++11 variadic templates ensure there is only one copy in of the given in ROM.

template<CHA CharA, CHA CharB, CHA CharC>
inline IUC T() {
  return ((IUC)CharA) & (((IUC)CharB) << 8) & (((IUC)CharC) << 16);
}
*/

/* Masks off the primary type. */
inline ISA ATypeMaskPOD(DTW value) { return value & 0x1f; }

/* Returns true if the given type is an Array type. */
inline BOL ATypeIsArray(DTW type) { return type >= ATypePODTotal; }

inline ISN ATypeSizeWidthCode(ISN type) { return type >> 6; }

/* Extracts the Map Type. */
inline DTW ATypeMap(DTW core_type, DTW mt) {
  return core_type | (mt << (ATypePODBits + 2));
}

inline DTW ATypeMap(DTW core_type, DTW mt, DTW size_width) {
  return ATypeMap(core_type, mt) | (size_width << ATypePODBits);
}

inline BOL ATypeIsPOD(DTW type) {
  return !((type >> 5) || (type & ATypePODMask));
}

}  //< namespace _
#endif
#endif  //< _CrabsATypeHPP