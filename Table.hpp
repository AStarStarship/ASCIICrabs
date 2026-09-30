// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef SCRIPTT2_TABLE_HPP
#define SCRIPTT2_TABLE_HPP
#include <_Config.h>
#if SEAM >= CRABS_TABLE
#include "Array.hpp"
#include "Binary.hpp"
#include "Hash.hpp"
#include "Error.h" //< Temp
#if SEAM == CRABS_TABLE
#include "_Debug.h"
#else
#include "_Release.h"
#endif
#define TBL_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, \
  typename ISY = ISQ, typename HSH = IUN
#define TBL_P CHS, CHT, ISZ, ISY, HSH
#define TBL TTable<CHS, CHT, ISZ, ISY, HSH>
namespace _ {

/* A dense key-index map.
@see ASCII Data Type Specification for DRY documentation.
@link ./Spec/Data/MapTypes/Table.md
@code
         Table Memory Layout
+--------------------------------------+
|_ISY_   pile_size (negative)          |
|_____ | Collision 0          ISB      |
|_____ v Collision N          pile     |
|_____   Collision N+ 1                |
|_____   pie_end                       |
+--------------------------------------+
|_____   Boofer Stop                   |
|_____   Boofer                        |
|_____   Boofer Start                  |
+--------------------------------------+
|_____ ^ Key N                 CHC     |
|_____ | Key 0                Keys     |
+--------------------------------------+
|_____   Boofer                ISY     |
|_____ ^ Unsorted Index N    sort_map  |
|      | Unsorted Index 0              |
+--------------------------------------+
|_____   Boofer                ISY     |
|_____ ^ Collision Index N   pile_map  |
|      | Collision Index 0             |
+--------------------------------------+
|_____   Boofer                ISZ     |
|_____ ^ Key Offset N        keys_map  |
|      | Key Offset 0                  |
+--------------------------------------+
|_____   Boofer                HSH     |
|_____ ^ Sorted Hash N       hash_map  |
|      | Sorted Hash 0                 |
+--------------------------------------+  ^ Up in addresses.
|            TTable Header             |  |
+--------------------------------------+ 0x0
@endcode
*/
template<TBL_A>
struct TTable {
  ISZ bytes,          //< Size of this object in bytes.
      stop;           //< Keys boofer stop offset or start if count == total.
  TStack<ISZ, ISZ, ISY> map;  //< A Stack of offset mappings to strings.
};

enum {
  TableSizeMin = 64,  //< Min size of a Table
};

template<TBL_A>
constexpr ISZ CTableEntryOverhead() {
  return sizeof(HSH) + sizeof(ISZ) + sizeof(ISY) + sizeof(ISY);
}

template<TBL_A>
inline HSH* TTableHashes(TBL* table) {
  return TPtr<HSH>(table, sizeof(TBL));
}
template<TBL_A>
inline const HSH* TTableHashes(const TBL* table) {
  return CPtr<HSH>(TTableHashes<TBL_P>(CPtr<TBL>(table)));
}

template<TBL_A>
inline BOL TTableHashesVerify(const TBL* table) {
  const HSH* hash_map = TTableHashes<TBL_P>(table);
  return THashFindFirstUnsorted<ISY, HSH>(hash_map, hash_map + table->map.count) < 0;
}

template<TBL_A>
inline ISY* TTableCollisionMap(TBL* table, ISY total) {
  return TPtr<ISZ>(&TTableHashes<TBL_P>(table)[total]);
}
template<TBL_A>
inline const ISY* TTableCollisionMap(const TBL* table, ISY total) {
  return CPtr<ISY>(TTableCollisionMap<TBL_P>(CPtr<TBL>(table), 
    total));
}

// Map from sorted indexes to unsorted key index.
template<TBL_A>
inline ISY* TTableSortMap(TBL* table, ISY total) {
  return TPtr<ISY>(TTableHashes<TBL_P>(table),
    total * (sizeof(HSH) + sizeof(ISZ) + sizeof(ISY)));
}
template<TBL_A>
inline const ISY* TTableSortMap(const TBL* table, ISY total) {
  return CPtr<ISY>(TTableSortMap<TBL_P>(CPtr<TBL>(table), total));
}

template<TBL_A>
inline ISY* TTablePile(TBL* table, ISZ bytes) {
  BOL fml = TAlignIs<ISY, ISZ>(bytes);
  D_ASSERT(fml);  //< @todo For some reason this is compile erroring?
  return TPtrDown<ISY>(table, bytes) - 1;
}
template<TBL_A>
inline const ISY* TTablePile(const TBL* table, ISZ bytes) {
  return CPtr<ISY>(TTablePile<TBL_P>(CPtr<TBL>(table), bytes));
}

template<TBL_A>
inline ISY* TTablePile(TBL* table) {
  return TTablePile<TBL_P>(table, table->bytes);
}
template<TBL_A>
inline const ISY* TTablePile(const TBL* table) {
  return TTablePile<TBL_P>(table, table->bytes);
}

/* TTablePointers<TBL_P>(table, bytes, total, keys_map, hash_map, pile_map,
                        sort_map);
*/
template<TBL_A>
inline void TTablePointers(TBL* table, ISZ bytes, ISY total, ISZ*& keys_map, 
  HSH*& hash_map, ISY*& pile_map, ISY*& sort_map) {
  hash_map = TPtr<HSH>(table, sizeof(TBL));
  keys_map = TPtr<ISZ>(hash_map + total);
  pile_map = TPtr<ISY>(keys_map + total);
  sort_map = pile_map + total;
}
template<TBL_A>
inline void TTablePointers(const TBL* table, ISZ bytes, ISY total,
  const ISZ*& keys_map, const HSH*& hash_map, const ISY*& pile_map,
  const ISY*& sort_map) {
  hash_map = TPtr<HSH>(table, sizeof(TBL));
  keys_map = TPtr<ISZ>(hash_map + total);
  pile_map = TPtr<ISY>(keys_map + total);
  sort_map = pile_map + total;
}

template<TBL_A, ISZ Total_ = 32, ISZ AverageStringLength_ = 16>
constexpr ISZ CTableSize() {
  return sizeof(TBL) + 
         2 * Total_ * (sizeof(ISZ) + sizeof(ISY)) + 
         Total_ * (AverageStringLength_ + 1);
}

template<TBL_A>
constexpr ISZ CTableSize(ISY total, ISZ average_string_length = 24) {
  return sizeof(TBL) + 
         2 * total * (sizeof(ISZ) + sizeof(ISY)) +
         total * (average_string_length + 1);
}

template<TBL_A>
void* TTableEnd(TBL* table) {
  return TPtr<void>(table, table->bytes);
}
template<TBL_A>
const void* TTableEnd(const TBL* table) {
  return CPtr<void>(TTableEnd<TBL_P>(CPtr<TBL>(table)));
}

// The offset from the keys_map to the keys boofer start.
template<TBL_A>
inline ISZ TTableKeysMapOffset(ISY total) {
  return (sizeof(HSH) + sizeof(ISZ)) * ISZ(total) +
    sizeof(TBL);
}

/* Points to the string offsets array. */
template<TBL_A>
ISZ* TTableKeysMap(TBL* table, ISY total) {
  return TPtr<ISZ>(table,  sizeof(HSH) * total + sizeof(TBL));
}
template<TBL_A>
const ISZ* TTableKeysMap(const TBL* table, ISY total) {
  return CPtr<ISZ>(TTableKeysMap<TBL_P>(CPtr<TBL>(table), total));
}
template<TBL_A>
ISZ* TTableKeysMap(TBL* table) {
  return TTableKeysMap<TBL_P>(table, ISY(table->map.total));
}
template<TBL_A>
const ISZ* TTableKeysMap(const TBL* table) {
  return TTableKeysMap<TBL_P>(table, ISY(table->map.total));
}

template<TBL_A>
inline CHS* TTableKey(ISZ* keys_map, ISZ offset) {
  return TPtr<CHS>(keys_map, offset);
}
template<TBL_A>
inline const CHS* TTableKey(const ISZ* keys_map, ISZ offset) {
  return CPtr<CHS>(TTableKey<TBL_P>(CPtr<ISZ>(keys_map), offset));
}
template<TBL_A>
CHS* TTableKey(TBL* table, ISY total, ISY count) {
  return TTableKey<TBL_P>(table, table)[count];
}
template<TBL_A>
const ISZ* TTableKey(const TBL* table, ISY total, ISY count) {
  return CPtr<ISZ>(TTableKey<TBL_P>(CPtr<TBL>(table), total, count));
}
template<TBL_A>
inline CHS* TTableKeyAt(ISZ* keys_map, ISY index) {
  return TPtr<CHS>(keys_map, keys_map[index]);
}
template<TBL_A>
inline const CHS* TTableKeyAt(const ISZ* keys_map, ISY index) {
  return CPtr<CHS>(TTableKeyAt<TBL_P>(CPtr<ISZ>(keys_map), index));
}

template<TBL_A>
inline CHS* TTableKeysStop(TBL* table, ISZ bytes) {
  ISY* pile      = TTablePile<TBL_P>(table, bytes);
  ISY  pile_size = *pile;
  D_ASSERT(pile_size < 0);
  return TPtrDown<CHS>(TPtr<ISA>(pile + pile_size) - sizeof(CHS));
}
template<TBL_A>
inline const CHS* TTableKeysStop(const TBL* table, ISZ bytes) {
  return CPtr<CHS>(TTableKeysStop<TBL_P>(CPtr<TBL>(table), bytes));
}
template<TBL_A>
inline ISZ TTableKeysMapStartOffset(ISY total) {
  return ISZ(sizeof(HSH) + sizeof(ISZ) + sizeof(ISY) + sizeof(ISY)) * ISZ(total);
}
template<TBL_A>
inline ISZ TTableKeysStopOffset(const TBL* table, ISZ bytes, const ISZ* keys_map) {
  return TDelta<ISZ>(keys_map, TTableKeysStop<TBL_P>(table, bytes));
}

// The address of the next collision index.
template<TBL_A>
inline CHS* TTablePileBotton(TBL* table, ISZ bytes) {
  ISY* pile = TTablePile<TBL_P>(table, bytes);
  return pile + *TPtr<ISY>(pile);
}
template<TBL_A>
inline const CHS* TTablePileBotton(const TBL* table, ISZ bytes) {
  return CPtr<CHS>(TTablePileBotton<TBL_P>(CPtr<TBL>(table), bytes));
}

/* Free space left in the keys boofer. */
template<TBL_A>
inline ISZ TTableSpace(const TBL* table, ISZ bytes, ISZ stop, 
  ISY total, ISY count) {
  const ISZ* keys_map = TTableKeysMap<TBL_P>(table, total);
  if (count >= total) // start is stored as table->stop.
    return TDelta<ISZ>(keys_map, TTableKeysStop<TBL_P>(table, bytes)) - stop;
  // stop is the stop and start is at keys_map[count].
  return stop - keys_map[count];
}
template<TBL_A>
inline ISZ TTableSpace(const TBL* table) {
  return TTableSpace<TBL_P>(table, table->bytes, table->stop,
    ISY(table->map.total), ISY(table->map.count));
}

/* Prints to the Printer. */
template<typename Printer, TBL_A>
Printer& TTablePrint(Printer& o, const TBL* table) {
  if (AIsError(table)) return o;
  enum {
    HashSize = (sizeof(HSH) * 2) < 10 ? 10 : (sizeof(HSH) * 2),
  };
  ISZ bytes = table->bytes,
      stop  = table->stop;
  ISY count = ISY(table->map.count),
      total = ISY(table->map.total);
  const ISZ* keys_map = NILP;
  const HSH* hash_map = NILP;
  const ISY* pile_map = NILP,
           * sort_map = NILP;
  TTablePointers<TBL_P>(table, bytes, total, keys_map, hash_map, pile_map,
                        sort_map);
  ISN total_length = STRLength(total) + 1;
  const ISY* pile = TTablePile<TBL_P>(table, bytes);
  D_ASSERT((TDelta<ISW>(pile) & ISW(sizeof(ISY) - 1)) == 0);
  ISY pile_size = *pile;
  D_ASSERT(pile_size < 0);
  total_length = (total_length < 2 ? 2 : total_length);
  #ifdef D_THIS
  ISZ start = 0;
  if (count < total) {
    start = keys_map[count];
  } else {
    start = stop;
    stop = TDelta<ISZ>(keys_map, TTableKeysStop<TBL_P>(table, bytes));
  }
  #endif
  o << Linef("\n+---\n| Table<CH") << CSizeCodef<CHS>()
    << ",IS" << CSizeCodef<ISZ>()
    << ",IS" << CSizeCodef<ISY>()
    << ",IU" << CSizeCodef<HSH>() << ">:0x"
    << Hexf(table) << " bytes:" << bytes << " count:" << count
    << " total:" << total
    #ifdef D_THIS
    << "\n| pile_size:" << pile_size << " start:" << start << " stop:" << stop
    #endif
    << Linef("\n+---\n| ") << Rightf("i", total_length) << ' '
    #ifdef D_THIS
    << Centerf("hash_e", HashSize - 2) << ' '
    << Centerf("hash_u", HashSize - 2) << ' '
    #endif
    << Centerf("hash_s", HashSize - 2) << ' '
    << Centerf("index_s", HashSize) << ' '
    << Centerf("offset", 8) << Linef(" \"Key\":{Collisions}\n+---");
  for (ISY i = 0; i < count; ++i) {
    ISZ offset = keys_map[i];
    ISY index_s = sort_map[i];
    HSH hash_u = hash_map[index_s],
        hash_s = hash_map[i];
    const CHS* key = TTableKey<TBL_P>(keys_map, offset);
    o << "\n| " << Rightf(i, total_length) << ' '
      #ifdef D_THIS
      << Centerf().Hex(THashPrime<HSH, CHS>(key), HashSize) << ' '
      << Centerf().Hex(hash_u, HashSize) << ' '
      #endif
      << Centerf().Hex(hash_s, HashSize) << ' '
      << Centerf(index_s, HashSize) << ' '
      << Centerf(offset, 8) << ' '
      << " \"" << key << "\":{";
    
    ISY pile_index = pile_map[i];
    if (pile_index > 0 || pile_index <= pile_size) {
      o << "\nOut of bounds pile_index at pile_map[" << i << "]: " << pile_index 
        << " pile_size:" << pile_size;
      D_ASSERT(pile_index > pile_size);
      return o;
    }
    if (pile_index < 0) {
      const ISY* pile_cursor = pile + pile_index,
               * pile_end    = pile + pile_size;
      ISY collision = *pile_cursor--;
      while (collision >= 0) {
        D_ASSERT(collision < count);
        o << collision;
        collision = *pile_cursor--;
        if (collision >= 0) o << ", ";
        if (pile_cursor < pile_end)
          return o << " Invalid collision" << collision << "}";
      }
    }
    o << '}';
  }
#ifdef D_THIS
  return o << Linef("\n+---") << Charsf(table, table->bytes) << '\n';
#endif
  return o << Linef("\n+---\n");
#endif
}

/* Inits a hash table. */
template<TBL_A>
TBL* TTableInit(TBL* table, ISY total) {
  D_CHECK_PTR_RETURN(table);
  ISZ bytes = table->bytes;
  bytes = TAlignDown<ISZ>(bytes);
  D_OBJ_WIPE(table, ISZ);
  ISZ* keys_map = TTableKeysMap<TBL_P>(table, total);
  ISY* pile = TTablePile<TBL_P>(table, bytes);
  *pile-- = CAInvalidIndex<ISY>();
  ISZ start = TTableKeysMapStartOffset<TBL_P>(total);
  *keys_map = start;
  CHS* stop_ptr = TPtrDown<CHS>(pile, -ISW(sizeof(CHS)));
  D_COUT("\nkeys_stop delta:" << TDelta<>(table, stop_ptr));
  ISZ  stop      = TDelta<ISZ>(keys_map, stop_ptr);
  D_COUT("\nCreating a TTable<> with bytes:" << bytes << " total:" << total << 
         " start:" << start << " stop:" << stop);
  if (start >= stop)
    D_RETURN_TPTR_ERROR(TBL, AErrorKeysBooferOverflow);
  table->stop      = stop;
  table->map.total = ISZ(total);
  table->map.count = 0;
  D_COUT_TABLE(table);
  return table;
}

template<TBL_A>
inline TBL* TTableInit(IUW* socket, ISY total) {
  return TTableInit<TBL_P>(TPtr<TBL>(socket), total);
}

/* Adds the key to the table.
@return An invalid index upon failure or valid index upon success.
@todo Check if pile has enough room to add another. */
template<TBL_A>
ISY TTableAppend(TBL* table, const CHS* key) {
  D_CHECK_PTR_TRETURN(ISY, table);
  D_CHECK_PTR_TRETURN(ISY, key);
  ISZ  bytes     = table->bytes,
       stop      = table->stop;
  ISY  total     = ISY(table->map.total),
       count     = ISY(table->map.count);
  if (count >= total) 
    D_RETURNT(ISY, -AErrorBooferOverflow);
  HSH* hash_map  = NILP;
  ISZ* keys_map  = NILP;
  ISY* pile_map  = NILP,
     * sort_map  = NILP;
  TTablePointers<TBL_P>(table, bytes, total, keys_map, hash_map, pile_map,
    sort_map);
  ISY count_new = count + 1;
  ISZ start = keys_map[count];
  CHS* stop_ptr = TPtr<CHS>(keys_map, stop);
  D_ASSERT(count >= 0 && count < total);
  if (count_new == total) {
    start = stop;
    D_COUT("\nstop:" << stop);
    stop_ptr = TTableKeysStop<TBL_P>(table, bytes);
    stop = TDelta<ISZ>(keys_map, stop_ptr);
  }
  CHS* start_ptr = TPtr<CHS>(keys_map, start);
  auto start_stop_delta = stop - start;
  auto start_stop_ptr_delta = stop_ptr - start_ptr;
  D_ASSERT(start_stop_delta == start_stop_ptr_delta);
  HSH  hash      = 0;
  ISZ  length    = 0;
  CHS* start_next_ptr = THashPrimePrint_NC<HSH, ISZ, CHS, CHS>(start_ptr,
    stop_ptr, key, hash, length);
  D_CHECK_PTR_TRETURN(ISY, start_next_ptr);
  if (++start_next_ptr >= stop_ptr)
    D_RETURNT(ISY, -AErrorKeysBooferOverflow);
  ISZ start_next = TDelta<ISZ>(keys_map, start_next_ptr);
  if (count_new < total)
    keys_map[count_new] = start_next;
  D_COUT(Linef("\n\n---\nAdding key: \"") << key << "\":" << length << ":0x" << Hexf(hash) <<
         " count:" << count << " total:" << total);
  D_COUT("\n\n  Finding insert location... ");
  if (count == 0) {
    *hash_map = hash;
    *pile_map = 0;
    *sort_map = count;
    table->map.count = ISZ(count_new);
    D_COUT_TABLE(table);
    return count;
  }
  ISY low  = 0,
      mid  = 0,
      high = count;
  HSH current_hash = 0;
  while (low <= high) {
    mid = (low + high) >> 1;  //< Shift >> 1 to / 2
    current_hash = hash_map[mid];
    D_COUT("\n     low:" << low << " mid:" << mid << " high:" << high << 
           " hash:0x" << Hexf(current_hash));
    if (hash < current_hash) {
      high = mid - 1;
    } else if (hash > current_hash) {
      low = mid + 1;
    } else { // Hash collision
      hash_map[count] = ~HSH(0);  //< high init value must be the max value.
      ISY sort_index    = sort_map[mid],
          pile_index  = pile_map[sort_index];
      D_COUT("\n         Found hash at sort_index:" << sort_index << 
             " pile_index:" << pile_index);
      if (pile_index < 0) {
        D_COUT("\n       Found collision. Checking the pile...");
        ISY* pile        = TTablePile<TBL_P>(table, bytes);
        ISY* pile_cursor = pile + pile_index;
        ISY  pile_size   = *pile,
             stack_index = *pile_cursor--;
        ISY* pile_end    = pile + pile_size;
        while (stack_index >= 0) {
          if (pile_cursor <= pile_end)
            D_RETURNT(ISY, -AErrorBooferOverflow);
          ISZ key_offset = keys_map[stack_index];
          D_COUT("\n         Comparing to \"" << 
                 TTableKey<TBL_P>(keys_map, key_offset) << 
                 "\":" << stack_index << 
                 "  table_to_pile_cursor_delta:" << TDelta<>(table, pile_cursor));
          if (TSEquals<CHS, CHT>(key, TTableKey<TBL_P>(keys_map, key_offset))) {
            D_COUT(" Found key at stack_index:" << stack_index);
            return stack_index;
          }
          stack_index = *pile_cursor--;
        }
        D_COUT("\n             New collision detected, inserting into pile");
        if (TPtrDown<CHS>(pile - 1) < start_ptr)
          D_RETURNT(ISY, -AErrorBooferOverflow);
        sort_map[count] = count;
        pile_map[count] = pile_index;
        for (ISY i = pile_index; i > pile_index - 5; --i)
          D_COUT("\n               pile[" << i << "]: " << *(pile + i));
        TArrayInsertDown_NC<ISY>(pile + pile_index, pile + pile_size, count);
        *pile = --pile_size;
        D_COUT("\n             Inserted at pile_index:" << pile_index);
        for (ISY i = pile_index; i > pile_index - 5; --i)
          D_COUT("\n               pile[" << i << "]: " << *(pile + i));
        table->map.count = ISZ(count_new);
        if (count_new == total - 1)
          table->stop = start_next;
        D_COUT_TABLE(table);
        return count;
      }
      ISZ key_offset = keys_map[sort_index];
      CHS* other_key = TTableKey<TBL_P>(keys_map, key_offset);
      D_COUT("\n           No collisions exist." <<
             "\n           Comparing to " << sort_index << ":\"" << other_key << 
             "\" at offset:" << key_offset);
      if (!TSEquals<CHS, CHT>(key, other_key)) {
        ISY* pile = TTablePile<TBL_P>(table, bytes);
        ISY  pile_size = *pile;
        D_COUT("\n             New collision with sort_index:" << 
               sort_index << " count:" << count << " pile_size:" << pile_size <<
               "\n               Adding to end of pile:");
        for (ISY i = pile_size; i < 0; ++i)
          D_COUT("\n                          pile[" << i << "]: " << *(pile + i));
        ISY* pile_cursor = pile + pile_size;
        if (TPtr<>(pile_cursor - 3) < TPtr<>(stop))
          D_RETURNT(ISY, -AErrorKeysBooferOverflow);
        sort_map[count     ] = count;
        pile_map[count     ] = pile_size;
        pile_map[sort_index] = pile_size;
        *pile = pile_size - 3;

        *pile_cursor-- = count;
        *pile_cursor-- = sort_index;
        *pile_cursor   = -1;

        table->map.count = ISZ(count_new);
        if (count_new == total - 1) table->stop = start_next;
        D_COUT_TABLE(table);
        return count;
      }
      D_COUT("\nTable already contains the key.");
      return sort_index;
    }
  }
  D_COUT("\n     Hash not found. Inserting into mid:" << mid << 
         " before hash:0x" << Hexf(hash_map[mid]) << '\n');
  if (hash > current_hash) ++mid;
  TArrayInsert_NC<HSH>(hash_map + mid, hash_map + count, hash);
  TArrayInsert_NC<ISY>(sort_map + mid, sort_map + count, count);
  pile_map[count] = 0;
  table->map.count = ISZ(count_new);
  if (count_new == total - 1) table->stop = start_next;
  D_COUT_TABLE(table);
  return count;
}

template<TBL_A>
ISY TTableAppend(TBL* table, TBL* items) {
  ISY count = ISY(items->map.count),
      total = ISY(items->map.total);
  D_ASSERT(count >= 0 && count <= total);
  ISZ* keys_map = TTableKeysMap<TBL_P>(items, total);
  ISY result = 0;
  for (ISY i = 0; i < count; ++i) {
    result = TTableAppend<TBL_P>(table, TPtr<CHS>(keys_map, keys_map[i]));
    if (result < 0) return result;
  }
  return result;
}

/* Doubles the size and total of the table.
@return Returns nil if the size is greater than the amount of memory that
can fit in type ISW, the unaltered socket pointer if the Stack has grown to the
size upper bounds, or a new dynamically allocated socket upon failure. */
template<TBL_A>
BOL TTableGrow(Autoject& obj) {
  D_COUT("\n\nGrowing Table...");
  /* Grow Algorithm.
  1. Check if we can grow and if so, creates a new block of memory.
  2. Copy the stacks. */
  auto origin = obj.origin;
  D_ASSERT(origin);
  auto source = TPtr<TBL>(origin);
  auto size = source->bytes;
  auto stop = source->stop;
  ISY  total = ISY(source->map.total),
    count = ISY(source->map.count);
  D_COUT("\nsize:" << size << " stop:" << stop << " total:" << total <<
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
  IUW* origin_new = obj.ram(NILP, size_new);
  D_COUT("\n\n*TPtr<ISZ>(origin_new):" << *TPtr<ISZ>(origin_new) <<
    " size_new:" << size_new);
  auto destination = TPtr<TBL>(origin_new);
  TTableInit<TBL_P>(destination, ISY(total_new));
  TTableAppend<TBL_P>(destination, source);
  obj.origin = origin_new;
  D_COUT("\n\nFinished growing. :-)\n\n");
  D_COUT_TABLE(TPtr<TBL>(obj.origin));
  return true;
}

/* Adds a string to the end of the Table, auto-growing if necessary.
@return The index upon success or -1 if the obj can't grow anymore.
@todo Verify compile size of this function isolated and in the AArray class. */
template<TBL_A>
ISY TTableAppend(Autoject& obj, const CHS* key) {
  D_CHECK_PTR_TRETURN(ISY, key);
  IUW* origin = obj.origin;
  ISY result = TTableAppend<TBL_P>(TPtr<TBL>(origin), key);
  while (result < 0) {
    if (!TTableGrow<TBL_P>(obj))
      D_RETURNT(ISY, -AErrorBooferOverflow);
    TBL* table = TPtr<TBL>(obj.origin);
    result = TTableAppend<TBL_P>(table, key);
    if (result < 0) {
      D_COUT("\n\n\nFailed to append to table:" << result << ' ' <<
        ASCIIErrorSTA(result));
      D_COUT_TABLE(table);
    }
    D_COUT("\ndez nutz baby!!!\n");
  }
  return result;
}

template<TBL_A>
ISY TTableAppend(Autoject& obj, TBL* items) {
  TBL* table = TPtr<TBL>(obj.origin);
  ISY count = ISY(table->map.count),
      total = ISY(table->map.total);
  D_ASSERT(count >= 0 && count <= total);
  ISZ* keys_map = TTableKeysMap<TBL_P>(table, total);
  ISY result = 0;
  for (ISY i = 0; i < count; ++i) {
    result = TTableAppend<TBL_P>(obj, TPtr<CHS>(keys_map, keys_map[i]));
    if (result < 0) {
      D_ASSERT(result >= 0);
      return result;
    }
  }
  D_COUT("\nFinished appending with result:" << result);
  return result;
}

/* Attempts to find the given key.
@return Returns TTableInvalidIndex<ISY>() upon failure, and valid index upon
success. */
template<TBL_A>
ISY TTableFind(const TBL* table, const CHS* key) {
  D_CHECK_PTR_TRETURN(ISY, table);
  D_CHECK_PTR_TRETURN(ISY, key);
  ISY count = ISY(table->map.count),
      total = ISY(table->map.total);
  if (count == 0) D_RETURNT(ISY, -AErrorInvalidBounds);
  ISZ bytes = table->bytes;
  const ISZ* keys_map = NILP;
  const HSH* hash_map = NILP;
  const ISY* pile_map = NILP,
           * sort_map = NILP;
  TTablePointers<TBL_P>(table, bytes, total, keys_map, hash_map, pile_map,
                        sort_map);
  HSH hash = THashPrime<HSH, CHS>(key);
  D_COUT("\n\nSearching for key:\"" << key << "\"\n             hash:0x" << 
         Hexf(hash) << "...");
  if (count == 1) {
    ISZ offset = keys_map[0];
    D_COUT("\nComparing to only key \"" << 
      TTableKey<TBL_P>(keys_map, offset) << "\":" << offset << "...");
    if (TSEquals<CHS, CHT>(key, TTableKey<TBL_P>(keys_map, offset))) {
      D_COUT("hit at offset:" << offset);
      return 0;
    }
    if (!TSEquals<CHS, CHT>(key, TTableKey<TBL_P>(keys_map, offset))) {
      D_COUT("key not found.");
      return CAInvalidIndex<ISY>();
    }
    D_COUT("Found key:\"" << key << '\"' << Linef('-'));
    return 0;
  }
  ISY low = 0, mid, high = count - 1;
  while (low <= high) { // Binary search through hashes.
    mid = (low + high) >> 1;  //< >> 1 to /2
    HSH current_hash = hash_map[mid];
    D_COUT("\n  low:" << low << " mid:" << mid << " high:" << high << 
           " hashes[mid]:" << Hexf(hash_map[mid]));
    if (current_hash > hash) {
      high = mid - 1;
    } else if (current_hash < hash) {
      low = mid + 1;
    } else {
      ISY sorted_index = sort_map[mid],
          pile_index   = pile_map[sorted_index];
      D_COUT("\n    Found hash[" << mid << "]:0x" << Hexf(hash_map[mid]) << 
        " sort_index:" << sorted_index << " pile_index:" << pile_index);
      if (pile_index < 0) {
        const ISY* pile = TTablePile<TBL_P>(table, bytes);
        ISY pile_offset = pile_map[sorted_index];
        const ISY* pile_cursor = pile + pile_offset;
        D_COUT("\n    Collision detected at pile_index:" << pile_index <<
          ".\n    Checking the pile_map[" << sorted_index << "]:" << pile_offset);
        ISY stack_index = *pile_cursor--;
        // @todo I'm not sure if we care if the collision list is empty.
        //if (stack_index < 0) return D_RETURNT(ISY, -AErrorInvalidInput);
        D_COUT(" pile_cursor:(" << stack_index << ", " << *(pile_cursor - 1) << ')');
        while (stack_index >= 0) {
          if (stack_index >= count)
            D_RETURNT(ISY, -AErrorInvalidInput);
          ISZ offset = keys_map[stack_index];
          const CHS* other = TTableKey<TBL_P>(keys_map, offset);
          D_COUT("\n      Comparing to:\"" << other << "\" at offset:" << 
                 offset << " at " << stack_index);
          if (TSEquals<CHS, CHT>(key, other)) {
            D_COUT("\n      Hit at index:" << stack_index << " offset:" << offset);
            return stack_index;
          }
          stack_index = *pile_cursor--;
        }
        D_COUT("\n      pile does not contain the key.");
        D_RETURNT(ISY, -AErrorUnavailable);
      }
      ISZ offset = keys_map[sorted_index];
      const CHS* other = TTableKeyAt<TBL_P>(keys_map, sorted_index);
      D_COUT("\n    Comparing to key:\"" << other << '"');
      if (!TSEquals<CHS, CHT>(key, other)) {
        D_COUT("\n    Table does not contain key.");
        return CAInvalidIndex<ISY>();
      }
      D_COUT("\n    Found key at sort_index:" << sorted_index);
      return sorted_index;
    }
  }
  D_COUT(Linef("\nTable does not contain the hash.\n---"));
  return CAInvalidIndex<ISY>();
}

/* Gets the given key by index without bounds checking. */
template<TBL_A>
inline CHS* TTableGet_NC(TBL* table, ISY total, ISY index) {
  return TPtr<CHS>(table, TTableKeysMap<TBL_P>(table, total)[index]);
}
template<TBL_A>
inline const CHS* TTableGet_NC(const TBL* table, ISY total, ISY index) {
  return CPtr<CHS>(TTableGet_NC<TBL_P>(CPtr<TBL>(table), index));
}

/* Gets the given key by index without bounds checking. */
template<TBL_A>
inline CHS* TTableGet_NC(TBL* table, ISY index) {
  return TTableGet_NC<TBL_P>(table, ISY(table->map.total), index);
}
template<TBL_A>
inline const CHS* TTableGet_NC(const TBL* table, ISY index) {
  return TTableGet_NC<TBL_P>(table, ISY(table->map.total), index);
}

/* Gets the given key by unsorted index. */
template<TBL_A>
inline CHS* TTableGet(TBL* table, ISY total, ISY index) {
  D_CHECK_TPTR_RETURN(CHS, table);
  ISY count = ISY(table->map.count);
  if (index < 0 || index >= count) return NILP;
  ISZ* keys_map = TTableKeysMap<TBL_P>(table, total);
  return TPtr<CHS>(keys_map, keys_map[index]);
}
template<TBL_A>
inline const CHS* TTableGet(const TBL* table, ISY total, ISY index) {
  return CPtr<CHS>(TTableGet<TBL_P>(CPtr<TBL>(table), total, index));
}

/* Gets the given key by unsorted index. */
template<TBL_A>
inline CHS* TTableGet(TBL* table, ISY index) {
  return TTableGet<TBL_P>(table, ISY(table->map.total), index);
}
template<TBL_A>
inline const CHS* TTableGet(const TBL* table, ISY index) {
  return TTableGet<TBL_P>(table, ISY(table->map.total), index);
}

/* Removes the string at the given sorted index. */
template<TBL_A>
ISY TTableRemove(TBL* table, ISY index, BOL pack = false) {
  D_CHECK_PTR_TRETURN(ISY, table);
  D_COUT("\nBefore:");
  D_COUT_TABLE(table);
  ISY count = ISY(table->map.count);
  if (index < 0 || index >= count)
    D_RETURNT(ISY, -AErrorInvalidIndex);
  ISZ bytes = table->bytes,
      stop  = table->stop;
  ISY total = ISY(table->map.total);
  if (index == count - 1) pack = false;
  ISZ* keys_map = NILP;
  HSH* hash_map = NILP;
  ISY* pile_map = NILP,
     * sort_map = NILP;
  TTablePointers<TBL_P>(table, bytes, total, keys_map, hash_map, pile_map,
                        sort_map);
  CHS* key_start = 0, 
     * key_end   = 0;
  ISY sorted_index = sort_map[index];
  TStackRemove<HSH, ISY>(hash_map, total, sorted_index);
  TStackRemove<ISZ, ISY>(keys_map, count, index);
  ISY pile_offset = pile_map[index];
  if (pile_offset > 0) {
    ISY* pile       = TTablePile<TBL_P>(table, bytes);
    ISY  pile_size  = *pile;
    ISY* cursor     = pile + pile_offset;
    ISY stack_index = *cursor++;
    while (stack_index >= 0) {
      if (stack_index == index) {
        ISY pile_end = *pile;
        ISY* pile_end_ptr = pile + pile_end;
        while (cursor > pile_end_ptr) *cursor-- = *cursor;
      }
      stack_index = *cursor++;
    }
  }
  TStackRemove<ISY, ISY>(pile_map, count, index);
  if (pack) {
    D_ASSERT(false);
    key_start = TPtr<CHS>(keys_map, sort_map[index]);
    key_end = (index == count) ? TPtr<CHS>(keys_map, stop) 
                               : TPtr<CHS>(keys_map, sort_map[index + 1]);
  }
  TStackRemove<ISY, ISY>(sort_map, count, index);
  ISY* end = sort_map + count;
  sort_map += index;
  while (sort_map < end) *sort_map = *sort_map - 1;
  if (count-- == total)
    table->stop = TDelta<ISZ>(keys_map, TTableKeysStop<TBL_P>(table, bytes));
  table->map.count = count;
  return count;
}

/* Removes the given key from the table. */
template<TBL_A>
ISY TTableRemove(TBL* table, const CHS* key) {
  ISY index = TTableFind<TBL_P>(table, key);
  return TTableRemove<TBL_P>(table, index);
}

/* An ASCII Table Autoject. */
template<TBL_A, ISY Total_ = 32, 
  typename BOF = TBOF<CTableSize<TBL_P, Total_>(), ISA, ISZ, TBL>>
class ATable {
  AArray<CHS, ISZ, Total_ * 32, BOF> obj_;  //< Auto-Array of CHS.
 public:
  /* Constructs a Table. */
  ATable() {
    D_OBJ_WIPE(obj_.Origin(), ISZ);
    TTableInit<TBL_P>(obj_.Origin(), Total_);
  }

  /* Constructs a Table. */
  ATable(ISY total) {
    D_OBJ_WIPE(obj_.Origin(), ISZ);
    TTableInit<TBL_P>(obj_.Origin(), total);
  }

  inline ISZ CBytes() { return This()->bytes; }

  /* Deep copies the given string into the Table.
  @return The index of the string in the Table. */
  inline ISY Append(const CHS* string) {
    return TTableAppend<TBL_P>(obj_.AJT(), string);
  }

  /* Removes the string at the given index from the Table. */
  inline ISY Remove(ISY index) {
    return TTableRemove<TBL_P>(This(), index);
  }

  /* Removes the string at the given index from the Table. */
  inline ISY Remove(const CHS* key) {
    return TTableRemove<TBL_P>(This(), key);
  }

  /* Gets the string at the given index. */
  inline CHS* Get(ISY index) {
    return TTableGet<TBL_P>(This(), index);
  }

  inline ISY Find(const CHS* string) {
    return TTableFind<TBL_P>(This(), string);
  }

  /* Gets the ASCII Object. */
  inline TBL* This() { return this->obj_.template As<TBL>(); }

  /* Gets the Autoject. */
  inline Autoject AJT() { return obj_.AJT(); }

  /* Gets the Auto-Array.
  inline AArray<IUA, ISZ, BOF>& AJT_ARY() { return obj_; } */

#if USING_STR
  template<typename Printer>
  inline Printer& PrintTo(Printer& o) {
    return TTablePrint<Printer, TBL_P>(o, This());
  }

  inline void CPrint() { PrintTo<::_::COut>(::_::StdOut()); }
};

}  //< namespace _
#endif
#endif
